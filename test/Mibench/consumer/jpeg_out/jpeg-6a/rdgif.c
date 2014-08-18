/*
 * rdgif.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 **************************************************************************
 * WARNING: You will need an LZW patent license from Unisys in order to   *
 * use this file legally in any commercial or shareware application.      *
 **************************************************************************
 *
 * This file contains routines to read input images in GIF format.
 *
 * These routines may need modification for non-Unix environments or
 * specialized applications.  As they stand, they assume input from
 * an ordinary stdio stream.  They further assume that reading begins
 * at the start of the file; input_init may need work if the
 * user interface has already read some data (e.g., to determine that
 * the file is indeed GIF format).
 */

/*
 * This code is loosely based on giftoppm from the PBMPLUS distribution
 * of Feb. 1991.  That file contains the following copyright notice:
 * +-------------------------------------------------------------------+
 * | Copyright 1990, David Koblas.                                     |
 * |   Permission to use, copy, modify, and distribute this software   |
 * |   and its documentation for any purpose and without fee is hereby |
 * |   granted, provided that the above copyright notice appear in all |
 * |   copies and that both that copyright notice and this permission  |
 * |   notice appear in supporting documentation.  This software is    |
 * |   provided "as is" without express or implied warranty.           |
 * +-------------------------------------------------------------------+
 *
 * We are also required to state that
 *    "The Graphics Interchange Format(c) is the Copyright property of
 *    CompuServe Incorporated. GIF(sm) is a Service Mark property of
 *    CompuServe Incorporated."
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */

#ifdef GIF_SUPPORTED


#define	MAXCOLORMAPSIZE	256	/* max # of colors in a GIF colormap */
#define NUMCOLORS	3	/* # of colors */
#define CM_RED		0	/* color component numbers */
#define CM_GREEN	1
#define CM_BLUE		2

#define	MAX_LZW_BITS	12	/* maximum LZW code size */
#define LZW_TABLE_SIZE	(1<<MAX_LZW_BITS) /* # of possible LZW symbols */

/* Macros for extracting header data --- note we assume chars may be signed */

#define LM_to_uint(a,b)		((((b)&0xFF) << 8) | ((a)&0xFF))

#define BitSet(byte, bit)	((byte) & (bit))
#define INTERLACE	0x40	/* mask for bit signifying interlaced image */
#define COLORMAPFLAG	0x80	/* mask for bit signifying colormap presence */

#define	ReadOK(file,buffer,len)	(JFREAD(file,buffer,len) == ((size_t) (len)))

/* LZW decompression tables look like this:
 *   symbol_head[K] = prefix symbol of any LZW symbol K (0..LZW_TABLE_SIZE-1)
 *   symbol_tail[K] = suffix byte   of any LZW symbol K (0..LZW_TABLE_SIZE-1)
 * Note that entries 0..end_code of the above tables are not used,
 * since those symbols represent raw bytes or special codes.
 *
 * The stack represents the not-yet-used expansion of the last LZW symbol.
 * In the worst case, a symbol could expand to as many bytes as there are
 * LZW symbols, so we allocate LZW_TABLE_SIZE bytes for the stack.
 * (This is conservative since that number includes the raw-byte symbols.)
 *
 * The tables are allocated from FAR heap space since they would use up
 * rather a lot of the near data space in a PC.
 */


/* Private version of data source object */

typedef struct {
  struct cjpeg_source_struct pub; /* public fields */

  j_compress_ptr cinfo;		/* back link saves passing separate parm */

  JSAMPARRAY colormap;		/* GIF colormap (converted to my format) */

  /* State for GetCode and LZWReadByte */
  char code_buf[256+4];		/* current input data block */
  int last_byte;		/* # of bytes in code_buf */
  int last_bit;			/* # of bits in code_buf */
  int cur_bit;			/* next bit index to read */
  boolean out_of_blocks;	/* TRUE if hit terminator data block */

  int input_code_size;		/* codesize given in GIF file */
  int clear_code,end_code;	/* values for Clear and End codes */

  int code_size;		/* current actual code size */
  int limit_code;		/* 2^code_size */
  int max_code;			/* first unused code value */
  boolean first_time;		/* flags first call to LZWReadByte */

  /* Private state for LZWReadByte */
  int oldcode;			/* previous LZW symbol */
  int firstcode;		/* first byte of oldcode's expansion */

  /* LZW symbol table and expansion stack */
  UINT16 FAR *symbol_head;	/* => table of prefix symbols */
  UINT8  FAR *symbol_tail;	/* => table of suffix bytes */
  UINT8  FAR *symbol_stack;	/* => stack for symbol expansions */
  UINT8  FAR *sp;		/* stack pointer */

  /* State for interlaced image processing */
  boolean is_interlaced;	/* TRUE if have interlaced image */
  jvirt_sarray_ptr interlaced_image; /* full image in interlaced order */
  JDIMENSION cur_row_number;	/* need to know actual row number */
  JDIMENSION pass2_offset;	/* # of pixel rows in pass 1 */
  JDIMENSION pass3_offset;	/* # of pixel rows in passes 1&2 */
  JDIMENSION pass4_offset;	/* # of pixel rows in passes 1,2,3 */
} gif_source_struct;

typedef gif_source_struct * gif_source_ptr;


/* Forward declarations */
METHODDEF(JDIMENSION) get_pixel_rows
	JPP((j_compress_ptr cinfo, cjpeg_source_ptr sinfo));
METHODDEF(JDIMENSION) load_interlaced_image
	JPP((j_compress_ptr cinfo, cjpeg_source_ptr sinfo));
METHODDEF(JDIMENSION) get_interlaced_row
	JPP((j_compress_ptr cinfo, cjpeg_source_ptr sinfo));


LOCAL(int)
ReadByte (gif_source_ptr sinfo)
/* Read next byte from GIF file */
{
  register FILE * infile = sinfo->pub.input_file;
  int c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));


  if ((c = getc(infile)) == EOF)
    ERREXIT(sinfo->cinfo, JERR_INPUT_EOF);
  return c;
}


LOCAL(int)
GetDataBlock (gif_source_ptr sinfo, char *buf)
/* Read a GIF data block, which has a leading count byte */
/* A zero-length block marks the end of a data block sequence */
{
  int count;
__boundcheck_metadata_store((void *)(&count),(void *)((size_t)(&count)+sizeof(count)*8-1));


  count = ReadByte(sinfo);
  if (count > 0) {
    if (! ReadOK(sinfo->pub.input_file, buf, count))
      ERREXIT(sinfo->cinfo, JERR_INPUT_EOF);
  }
  return count;
}


LOCAL(void)
SkipDataBlocks (gif_source_ptr sinfo)
/* Skip a series of data blocks, until a block terminator is found */
{
  char buf[256];__boundcheck_metadata_store(&buf[0],&buf[256-1]);


  while (GetDataBlock(sinfo, buf) > 0)
    /* skip */;
}


LOCAL(void)
ReInitLZW (gif_source_ptr sinfo)
/* (Re)initialize LZW state; shared code for startup and Clear processing */
{
  sinfo->code_size = sinfo->input_code_size + 1;
  sinfo->limit_code = sinfo->clear_code << 1;	/* 2^code_size */
  sinfo->max_code = sinfo->clear_code + 2;	/* first unused code value */
  sinfo->sp = sinfo->symbol_stack;		/* init stack to empty */
}


LOCAL(void)
InitLZWCode (gif_source_ptr sinfo)
/* Initialize for a series of LZWReadByte (and hence GetCode) calls */
{
  /* GetCode initialization */
  sinfo->last_byte = 2;		/* make safe to "recopy last two bytes" */
  sinfo->last_bit = 0;		/* nothing in the buffer */
  sinfo->cur_bit = 0;		/* force buffer load on first call */
  sinfo->out_of_blocks = FALSE;

  /* LZWReadByte initialization: */
  /* compute special code values (note that these do not change later) */
  sinfo->clear_code = 1 << sinfo->input_code_size;
  sinfo->end_code = sinfo->clear_code + 1;
  sinfo->first_time = TRUE;
  ReInitLZW(sinfo);
}


LOCAL(int)
GetCode (gif_source_ptr sinfo)
/* Fetch the next code_size bits from the GIF data */
/* We assume code_size is less than 16 */
{
  register INT32 accum;
  int offs;
__boundcheck_metadata_store((void *)(&offs),(void *)((size_t)(&offs)+sizeof(offs)*8-1));
int  ret;
__boundcheck_metadata_store((void *)(&ret),(void *)((size_t)(&ret)+sizeof(ret)*8-1));
int  count;
__boundcheck_metadata_store((void *)(&count),(void *)((size_t)(&count)+sizeof(count)*8-1));


  while ( (sinfo->cur_bit + sinfo->code_size) > sinfo->last_bit) {
    /* Time to reload the buffer */
    if (sinfo->out_of_blocks) {
      WARNMS(sinfo->cinfo, JWRN_GIF_NOMOREDATA);
      return sinfo->end_code;	/* fake something useful */
    }
    /* preserve last two bytes of what we have -- assume code_size <= 16 */
    sinfo->code_buf[_RV_insert_check(0,260,222,5,"GetCode",0)] = sinfo->code_buf[_RV_insert_check(0,260,222,26,"GetCode",sinfo->last_byte-2)];
    sinfo->code_buf[_RV_insert_check(0,260,223,5,"GetCode",1)] = sinfo->code_buf[_RV_insert_check(0,260,223,26,"GetCode",sinfo->last_byte-1)];
    /* Load more bytes; set flag if we reach the terminator block */
    if ((count = GetDataBlock(sinfo, &sinfo->code_buf[_RV_insert_check(0,260,225,39,"GetCode",2)])) == 0) {
      sinfo->out_of_blocks = TRUE;
      WARNMS(sinfo->cinfo, JWRN_GIF_NOMOREDATA);
      return sinfo->end_code;	/* fake something useful */
    }
    /* Reset counters */
    sinfo->cur_bit = (sinfo->cur_bit - sinfo->last_bit) + 16;
    sinfo->last_byte = 2 + count;
    sinfo->last_bit = sinfo->last_byte * 8;
  }

  /* Form up next 24 bits in accum */
  offs = sinfo->cur_bit >> 3;	/* byte containing cur_bit */
#ifdef CHAR_IS_UNSIGNED
  accum = sinfo->code_buf[offs+2];
  accum <<= 8;
  accum |= sinfo->code_buf[offs+1];
  accum <<= 8;
  accum |= sinfo->code_buf[offs];
#else
  accum = sinfo->code_buf[_RV_insert_check(0,260,245,11,"GetCode",offs+2)] & 0xFF;
  accum <<= 8;
  accum |= sinfo->code_buf[_RV_insert_check(0,260,247,12,"GetCode",offs+1)] & 0xFF;
  accum <<= 8;
  accum |= sinfo->code_buf[_RV_insert_check(0,260,249,12,"GetCode",offs)] & 0xFF;
#endif

  /* Right-align cur_bit in accum, then mask off desired number of bits */
  accum >>= (sinfo->cur_bit & 7);
  ret = ((int) accum) & ((1 << sinfo->code_size) - 1);
  
  sinfo->cur_bit += sinfo->code_size;
  return ret;
}


LOCAL(int)
LZWReadByte (gif_source_ptr sinfo)
/* Read an LZW-compressed byte */
{
  register int code;		/* current working code */
  int incode;
__boundcheck_metadata_store((void *)(&incode),(void *)((size_t)(&incode)+sizeof(incode)*8-1));
			/* saves actual input code */

  /* First time, just eat the expected Clear code(s) and return next code, */
  /* which is expected to be a raw byte. */
  if (sinfo->first_time) {
    sinfo->first_time = FALSE;
    code = sinfo->clear_code;	/* enables sharing code with Clear case */
  } else {

    /* If any codes are stacked from a previously read symbol, return them */
    if (sinfo->sp > sinfo->symbol_stack)
      return (int) *(UINT8 *)(__boundcheck_ptr_reference(277,34,"LZWReadByte",(void *)(sinfo->sp),(void *)((-- sinfo->sp))));

    /* Time to read a new symbol */
    code = GetCode(sinfo);

  }

  if (code == sinfo->clear_code) {
    /* Reinit state, swallow any extra Clear codes, and */
    /* return next code, which is expected to be a raw byte. */
    ReInitLZW(sinfo);
    do {
      code = GetCode(sinfo);
    } while (code == sinfo->clear_code);
    if (code > sinfo->clear_code) { /* make sure it is a raw byte */
      WARNMS(sinfo->cinfo, JWRN_GIF_BADDATA);
      code = 0;			/* use something valid */
    }
    /* make firstcode, oldcode valid! */
    sinfo->firstcode = sinfo->oldcode = code;
    return code;
  }

  if (code == sinfo->end_code) {
    /* Skip the rest of the image, unless GetCode already read terminator */
    if (! sinfo->out_of_blocks) {
      SkipDataBlocks(sinfo);
      sinfo->out_of_blocks = TRUE;
    }
    /* Complain that there's not enough data */
    WARNMS(sinfo->cinfo, JWRN_GIF_ENDCODE);
    /* Pad data with 0's */
    return 0;			/* fake something usable */
  }

  /* Got normal raw byte or LZW symbol */
  incode = code;		/* save for a moment */
  
  if (code >= sinfo->max_code) { /* special case for not-yet-defined symbol */
    /* code == max_code is OK; anything bigger is bad data */
    if (code > sinfo->max_code) {
      WARNMS(sinfo->cinfo, JWRN_GIF_BADDATA);
      incode = 0;		/* prevent creation of loops in symbol table */
    }
    /* this symbol will be defined as oldcode/firstcode */
    *(UINT8 *)(__boundcheck_ptr_reference(322,18,"LZWReadByte",(void *)(sinfo->sp),(void *)((sinfo->sp++)))) = (UINT8) sinfo->firstcode;
    code = sinfo->oldcode;
  }

  /* If it's a symbol, expand it into the stack */
  while (code >= sinfo->clear_code) {
    *(UINT8 *)(__boundcheck_ptr_reference(328,18,"LZWReadByte",(void *)(sinfo->sp),(void *)((sinfo->sp++)))) = sinfo->symbol_tail[code]; /* tail is a byte value */
    code = sinfo->symbol_head[code]; /* head is another LZW symbol */
  }
  /* At this point code just represents a raw byte */
  sinfo->firstcode = code;	/* save for possible future use */

  /* If there's room in table, */
  if ((code = sinfo->max_code) < LZW_TABLE_SIZE) {
    /* Define a new symbol = prev sym + head of this sym's expansion */
    sinfo->symbol_head[code] = sinfo->oldcode;
    sinfo->symbol_tail[code] = (UINT8) sinfo->firstcode;
    sinfo->max_code++;
    /* Is it time to increase code_size? */
    if ((sinfo->max_code >= sinfo->limit_code) &&
	(sinfo->code_size < MAX_LZW_BITS)) {
      sinfo->code_size++;
      sinfo->limit_code <<= 1;	/* keep equal to 2^code_size */
    }
  }
  
  sinfo->oldcode = incode;	/* save last input symbol for future use */
  return sinfo->firstcode;	/* return first byte of symbol's expansion */
}


LOCAL(void)
ReadColorMap (gif_source_ptr sinfo, int cmaplen, JSAMPARRAY cmap)
/* Read a GIF colormap */
{
  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  for (i = 0; i < cmaplen; i++) {
#if BITS_IN_JSAMPLE == 8
#define UPSCALE(x)  (x)
#else
#define UPSCALE(x)  ((x) << (BITS_IN_JSAMPLE-8))
#endif
    (*(JSAMPLE *)(__boundcheck_ptr_reference(365,5,"ReadColorMap",(void *)(&cmap[0][0]),(void *)(&cmap[0][i]))))   = (JSAMPLE) UPSCALE(ReadByte(sinfo));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(366,5,"ReadColorMap",(void *)(&cmap[1][0]),(void *)(&cmap[1][i])))) = (JSAMPLE) UPSCALE(ReadByte(sinfo));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(367,5,"ReadColorMap",(void *)(&cmap[2][0]),(void *)(&cmap[2][i]))))  = (JSAMPLE) UPSCALE(ReadByte(sinfo));
  }
}


LOCAL(void)
DoExtension (gif_source_ptr sinfo)
/* Process an extension block */
/* Currently we ignore 'em all */
{
  int extlabel;
__boundcheck_metadata_store((void *)(&extlabel),(void *)((size_t)(&extlabel)+sizeof(extlabel)*8-1));


  /* Read extension label byte */
  extlabel = ReadByte(sinfo);
  TRACEMS1(sinfo->cinfo, 1, JTRC_GIF_EXTENSION, extlabel);
  /* Skip the data block(s) associated with the extension */
  SkipDataBlocks(sinfo);
}


/*
 * Read the file header; return image size and component count.
 */

METHODDEF(void)
start_input_gif (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  gif_source_ptr source = (gif_source_ptr) sinfo;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

  char hdrbuf[10];__boundcheck_metadata_store(&hdrbuf[0],&hdrbuf[10-1]);
		/* workspace for reading control blocks */
  unsigned int width;
__boundcheck_metadata_store((void *)(&width),(void *)((size_t)(&width)+sizeof(width)*8-1));
unsigned int  height;
__boundcheck_metadata_store((void *)(&height),(void *)((size_t)(&height)+sizeof(height)*8-1));
	/* image dimensions */
  int colormaplen;
__boundcheck_metadata_store((void *)(&colormaplen),(void *)((size_t)(&colormaplen)+sizeof(colormaplen)*8-1));
int  aspectRatio;
__boundcheck_metadata_store((void *)(&aspectRatio),(void *)((size_t)(&aspectRatio)+sizeof(aspectRatio)*8-1));

  int c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));


  /* Allocate space to store the colormap */
  source->colormap = (*(JSAMPARRAY (*)(j_common_ptr, int, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(401,36,"start_input_gif",(void *)(cinfo->mem->alloc_sarray),(void *)cinfo->mem->alloc_sarray)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE,
     (JDIMENSION) MAXCOLORMAPSIZE, (JDIMENSION) NUMCOLORS);

  /* Read and verify GIF Header */
  if (! ReadOK(source->pub.input_file, hdrbuf, 6))
    ERREXIT(cinfo, JERR_GIF_NOT);
  if (hdrbuf[_RV_insert_check(0,10,408,7,"start_input_gif",0)] != 'G' || hdrbuf[_RV_insert_check(0,10,408,27,"start_input_gif",1)] != 'I' || hdrbuf[_RV_insert_check(0,10,408,47,"start_input_gif",2)] != 'F')
    ERREXIT(cinfo, JERR_GIF_NOT);
  /* Check for expected version numbers.
   * If unknown version, give warning and try to process anyway;
   * this is per recommendation in GIF89a standard.
   */
  if ((hdrbuf[_RV_insert_check(0,10,414,8,"start_input_gif",3)] != '8' || hdrbuf[_RV_insert_check(0,10,414,28,"start_input_gif",4)] != '7' || hdrbuf[_RV_insert_check(0,10,414,48,"start_input_gif",5)] != 'a') &&
      (hdrbuf[_RV_insert_check(0,10,415,8,"start_input_gif",3)] != '8' || hdrbuf[_RV_insert_check(0,10,415,28,"start_input_gif",4)] != '9' || hdrbuf[_RV_insert_check(0,10,415,48,"start_input_gif",5)] != 'a'))
    TRACEMS3(
__boundcheck_metadata_store((void *)(&_mp),(void *)((size_t)(&_mp)+sizeof(_mp)*8-1));
cinfo, 1, JTRC_GIF_BADVERSION, hdrbuf[3], hdrbuf[4], hdrbuf[5]);

  /* Read and decipher Logical Screen Descriptor */
  if (! ReadOK(source->pub.input_file, hdrbuf, 7))
    ERREXIT(cinfo, JERR_INPUT_EOF);
  width = LM_to_uint(hdrbuf[0],hdrbuf[1]);
  height = LM_to_uint(hdrbuf[2],hdrbuf[3]);
  colormaplen = 2 << (hdrbuf[_RV_insert_check(0,10,423,23,"start_input_gif",4)] & 0x07);
  /* we ignore the color resolution, sort flag, and background color index */
  aspectRatio = hdrbuf[_RV_insert_check(0,10,425,17,"start_input_gif",6)] & 0xFF;
  if (aspectRatio != 0 && aspectRatio != 49)
    TRACEMS(cinfo, 1, JTRC_GIF_NONSQUARE);

  /* Read global colormap if header indicates it is present */
  if (BitSet(hdrbuf[4], COLORMAPFLAG))
    ReadColorMap(source, colormaplen, source->colormap);

  /* Scan until we reach start of desired image.
   * We don't currently support skipping images, but could add it easily.
   */
  for (;;) {
    c = ReadByte(source);

    if (c == ';')		/* GIF terminator?? */
      ERREXIT(cinfo, JERR_GIF_IMAGENOTFOUND);

    if (c == '!') {		/* Extension */
      DoExtension(source);
      continue;
    }
    
    if (c != ',') {		/* Not an image separator? */
      WARNMS1(cinfo, JWRN_GIF_CHAR, c);
      continue;
    }

    /* Read and decipher Local Image Descriptor */
    if (! ReadOK(source->pub.input_file, hdrbuf, 9))
      ERREXIT(cinfo, JERR_INPUT_EOF);
    /* we ignore top/left position info, also sort flag */
    width = LM_to_uint(hdrbuf[4],hdrbuf[5]);
    height = LM_to_uint(hdrbuf[6],hdrbuf[7]);
    source->is_interlaced = BitSet(hdrbuf[8], INTERLACE);

    /* Read local colormap if header indicates it is present */
    /* Note: if we wanted to support skipping images, */
    /* we'd need to skip rather than read colormap for ignored images */
    if (BitSet(hdrbuf[8], COLORMAPFLAG)) {
      colormaplen = 2 << (hdrbuf[_RV_insert_check(0,10,464,27,"start_input_gif",8)] & 0x07);
      ReadColorMap(source, colormaplen, source->colormap);
    }

    source->input_code_size = ReadByte(source); /* get min-code-size byte */
    if (source->input_code_size < 2 || source->input_code_size >= MAX_LZW_BITS)
      ERREXIT1(cinfo, JERR_GIF_CODESIZE, source->input_code_size);

    /* Reached desired image, so break out of loop */
    /* If we wanted to skip this image, */
    /* we'd call SkipDataBlocks and then continue the loop */
    break;
  }

  /* Prepare to read selected image: first initialize LZW decompressor */
  source->symbol_head = (UINT16 FAR *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(480,19,"start_input_gif",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				LZW_TABLE_SIZE * SIZEOF(UINT16));
  source->symbol_tail = (UINT8 FAR *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(483,19,"start_input_gif",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				LZW_TABLE_SIZE * SIZEOF(UINT8));
  source->symbol_stack = (UINT8 FAR *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(486,19,"start_input_gif",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				LZW_TABLE_SIZE * SIZEOF(UINT8));
  InitLZWCode(source);

  /*
   * If image is interlaced, we read it into a full-size sample array,
   * decompressing as we go; then get_interlaced_row selects rows from the
   * sample array in the proper order.
   */
  if (source->is_interlaced) {
    /* We request the virtual array now, but can't access it until virtual
     * arrays have been allocated.  Hence, the actual work of reading the
     * image is postponed until the first call to get_pixel_rows.
     */
    source->interlaced_image = (*(jvirt_sarray_ptr (*)(j_common_ptr, int, boolean, JDIMENSION, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(500,46,"start_input_gif",(void *)(cinfo->mem->request_virt_sarray),(void *)cinfo->mem->request_virt_sarray)))
      ((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
       (JDIMENSION) width, (JDIMENSION) height, (JDIMENSION) 1);
    if (cinfo->progress != NULL) {
      cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
__boundcheck_metadata_store((void *)(&progress),(void *)((size_t)(&progress)+sizeof(progress)*8-1));

      progress->total_extra_passes++; /* count file input as separate pass */
    }
    source->pub.get_pixel_rows = load_interlaced_image;
  } else {
    source->pub.get_pixel_rows = get_pixel_rows;
  }

  /* Create compressor input buffer. */
  source->pub.buffer = (*(JSAMPARRAY (*)(j_common_ptr, int, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(513,38,"start_input_gif",(void *)(cinfo->mem->alloc_sarray),(void *)cinfo->mem->alloc_sarray)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE,
     (JDIMENSION) width * NUMCOLORS, (JDIMENSION) 1);
  source->pub.buffer_height = 1;

  /* Return info about the image. */
  cinfo->in_color_space = JCS_RGB;
  cinfo->input_components = NUMCOLORS;
  cinfo->data_precision = BITS_IN_JSAMPLE; /* we always rescale data to this */
  cinfo->image_width = width;
  cinfo->image_height = height;

  TRACEMS3(
__boundcheck_metadata_store((void *)(&_mp),(void *)((size_t)(&_mp)+sizeof(_mp)*8-1));
cinfo, 1, JTRC_GIF, width, height, colormaplen);
}


/*
 * Read one row of pixels.
 * This version is used for noninterlaced GIF images:
 * we read directly from the GIF file.
 */

METHODDEF(JDIMENSION)
get_pixel_rows (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  gif_source_ptr source = (gif_source_ptr) sinfo;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

  register int c;
  register JSAMPROW ptr;
  register JDIMENSION col;
  register JSAMPARRAY colormap = source->colormap;
  
  ptr = source->pub.buffer[0];
  for (col = cinfo->image_width; col > 0; col--) {
    c = LZWReadByte(source);
    *(JSAMPROW)(__boundcheck_ptr_reference(547,9,"get_pixel_rows",(void *)(ptr),(void *)(ptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(547,14,"get_pixel_rows",(void *)(&colormap[0][0]),(void *)(&colormap[0][c]))));
    *(JSAMPROW)(__boundcheck_ptr_reference(548,9,"get_pixel_rows",(void *)(ptr),(void *)(ptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(548,14,"get_pixel_rows",(void *)(&colormap[1][0]),(void *)(&colormap[1][c]))));
    *(JSAMPROW)(__boundcheck_ptr_reference(549,9,"get_pixel_rows",(void *)(ptr),(void *)(ptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(549,14,"get_pixel_rows",(void *)(&colormap[2][0]),(void *)(&colormap[2][c]))));
  }
  return 1;
}


/*
 * Read one row of pixels.
 * This version is used for the first call on get_pixel_rows when
 * reading an interlaced GIF file: we read the whole image into memory.
 */

METHODDEF(JDIMENSION)
load_interlaced_image (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  gif_source_ptr source = (gif_source_ptr) sinfo;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

  JSAMPARRAY image_ptr;
__boundcheck_metadata_store((void *)(&image_ptr),(void *)((size_t)(&image_ptr)+sizeof(image_ptr)*8-1));

  register JSAMPROW sptr;
  register JDIMENSION col;
  JDIMENSION row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));

  cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
__boundcheck_metadata_store((void *)(&progress),(void *)((size_t)(&progress)+sizeof(progress)*8-1));


  /* Read the interlaced image into the virtual array we've created. */
  for (row = 0; row < cinfo->image_height; row++) {
    if (progress != NULL) {
      progress->pub.pass_counter = (long) row;
      progress->pub.pass_limit = (long) cinfo->image_height;
      (*(void (*)(j_common_ptr))(__boundcheck_ptr_reference(576,23,"load_interlaced_image",(void *)(progress->pub.progress_monitor),(void *)progress->pub.progress_monitor))) ((j_common_ptr) cinfo);
    }
    image_ptr = (*(JSAMPARRAY (*)(j_common_ptr, jvirt_sarray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(578,31,"load_interlaced_image",(void *)(cinfo->mem->access_virt_sarray),(void *)cinfo->mem->access_virt_sarray)))
      ((j_common_ptr) cinfo, source->interlaced_image,
       row, (JDIMENSION) 1, TRUE);
    sptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(581,12,"load_interlaced_image",(void *)(&image_ptr[0]),(void *)(&image_ptr[0]))));
    for (col = cinfo->image_width; col > 0; col--) {
      *(JSAMPROW)(__boundcheck_ptr_reference(583,12,"load_interlaced_image",(void *)(sptr),(void *)(sptr++))) = (JSAMPLE) LZWReadByte(source);
    }
  }
  if (progress != NULL)
    progress->completed_extra_passes++;

  /* Replace method pointer so subsequent calls don't come here. */
  source->pub.get_pixel_rows = get_interlaced_row;
  /* Initialize for get_interlaced_row, and perform first call on it. */
  source->cur_row_number = 0;
  source->pass2_offset = (cinfo->image_height + 7) / 8;
  source->pass3_offset = source->pass2_offset + (cinfo->image_height + 3) / 8;
  source->pass4_offset = source->pass3_offset + (cinfo->image_height + 1) / 4;

  return get_interlaced_row(cinfo, sinfo);
}


/*
 * Read one row of pixels.
 * This version is used for interlaced GIF images:
 * we read from the virtual array.
 */

METHODDEF(JDIMENSION)
get_interlaced_row (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  gif_source_ptr source = (gif_source_ptr) sinfo;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

  JSAMPARRAY image_ptr;
__boundcheck_metadata_store((void *)(&image_ptr),(void *)((size_t)(&image_ptr)+sizeof(image_ptr)*8-1));

  register int c;
  register JSAMPROW sptr, ptr;
  register JDIMENSION col;
  register JSAMPARRAY colormap = source->colormap;
  JDIMENSION irow;
__boundcheck_metadata_store((void *)(&irow),(void *)((size_t)(&irow)+sizeof(irow)*8-1));


  /* Figure out which row of interlaced image is needed, and access it. */
  switch ((int) (source->cur_row_number & 7)) {
  case 0:			/* first-pass row */
    irow = source->cur_row_number >> 3;
    break;
  case 4:			/* second-pass row */
    irow = (source->cur_row_number >> 3) + source->pass2_offset;
    break;
  case 2:			/* third-pass row */
  case 6:
    irow = (source->cur_row_number >> 2) + source->pass3_offset;
    break;
  default:			/* fourth-pass row */
    irow = (source->cur_row_number >> 1) + source->pass4_offset;
    break;
  }
  image_ptr = (*(JSAMPARRAY (*)(j_common_ptr, jvirt_sarray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(634,29,"get_interlaced_row",(void *)(cinfo->mem->access_virt_sarray),(void *)cinfo->mem->access_virt_sarray)))
    ((j_common_ptr) cinfo, source->interlaced_image,
     irow, (JDIMENSION) 1, FALSE);
  /* Scan the row, expand colormap, and output */
  sptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(638,10,"get_interlaced_row",(void *)(&image_ptr[0]),(void *)(&image_ptr[0]))));
  ptr = source->pub.buffer[0];
  for (col = cinfo->image_width; col > 0; col--) {
    c = GETJSAMPLE(*sptr++);
    *(JSAMPROW)(__boundcheck_ptr_reference(642,9,"get_interlaced_row",(void *)(ptr),(void *)(ptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(642,14,"get_interlaced_row",(void *)(&colormap[0][0]),(void *)(&colormap[0][c]))));
    *(JSAMPROW)(__boundcheck_ptr_reference(643,9,"get_interlaced_row",(void *)(ptr),(void *)(ptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(643,14,"get_interlaced_row",(void *)(&colormap[1][0]),(void *)(&colormap[1][c]))));
    *(JSAMPROW)(__boundcheck_ptr_reference(644,9,"get_interlaced_row",(void *)(ptr),(void *)(ptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(644,14,"get_interlaced_row",(void *)(&colormap[2][0]),(void *)(&colormap[2][c]))));
  }
  source->cur_row_number++;	/* for next time */
  return 1;
}


/*
 * Finish up at the end of the file.
 */

METHODDEF(void)
finish_input_gif (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  /* no work */
}


/*
 * The module selection routine for GIF format input.
 */

GLOBAL(cjpeg_source_ptr)
jinit_read_gif (j_compress_ptr cinfo)
{
  gif_source_ptr source;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));


  /* Create module interface object */
  source = (gif_source_ptr)
      (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(673,21,"jinit_read_gif",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  SIZEOF(gif_source_struct));
  source->cinfo = cinfo;	/* make back link for subroutines */
  /* Fill in method ptrs, except get_pixel_rows which start_input sets */
  source->pub.start_input = start_input_gif;
  source->pub.finish_input = finish_input_gif;

  return (cjpeg_source_ptr) source;
}

#endif /* GIF_SUPPORTED */
