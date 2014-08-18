/*
 * rdbmp.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains routines to read input images in Microsoft "BMP"
 * format (MS Windows 3.x, OS/2 1.x, and OS/2 2.x flavors).
 * Currently, only 8-bit and 24-bit images are supported, not 1-bit or
 * 4-bit (feeding such low-depth images into JPEG would be silly anyway).
 * Also, we don't support RLE-compressed files.
 *
 * These routines may need modification for non-Unix environments or
 * specialized applications.  As they stand, they assume input from
 * an ordinary stdio stream.  They further assume that reading begins
 * at the start of the file; start_input may need work if the
 * user interface has already read some data (e.g., to determine that
 * the file is indeed BMP format).
 *
 * This code contributed by James Arthur Boucher.
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */

#ifdef BMP_SUPPORTED


/* Macros to deal with unsigned chars as efficiently as compiler allows */

#ifdef HAVE_UNSIGNED_CHAR
typedef unsigned char U_CHAR;
#define UCH(x)	((int) (x))
#else /* !HAVE_UNSIGNED_CHAR */
#ifdef CHAR_IS_UNSIGNED
typedef char U_CHAR;
#define UCH(x)	((int) (x))
#else
typedef char U_CHAR;
#define UCH(x)	((int) (x) & 0xFF)
#endif
#endif /* HAVE_UNSIGNED_CHAR */


#define	ReadOK(file,buffer,len)	(JFREAD(file,buffer,len) == ((size_t) (len)))


/* Private version of data source object */

typedef struct _bmp_source_struct * bmp_source_ptr;

typedef struct _bmp_source_struct {
  struct cjpeg_source_struct pub; /* public fields */

  j_compress_ptr cinfo;		/* back link saves passing separate parm */

  JSAMPARRAY colormap;		/* BMP colormap (converted to my format) */

  jvirt_sarray_ptr whole_image;	/* Needed to reverse row order */
  JDIMENSION source_row;	/* Current source row number */
  JDIMENSION row_width;		/* Physical width of scanlines in file */

  int bits_per_pixel;		/* remembers 8- or 24-bit format */
} bmp_source_struct;


LOCAL(int)
read_byte (bmp_source_ptr sinfo)
/* Read next byte from BMP file */
{
  register FILE *infile = sinfo->pub.input_file;
  register int c;

  if ((c = getc(infile)) == EOF)
    ERREXIT(sinfo->cinfo, JERR_INPUT_EOF);
  return c;
}


LOCAL(void)
read_colormap (bmp_source_ptr sinfo, int cmaplen, int mapentrysize)
/* Read the colormap from a BMP file */
{
  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  switch (mapentrysize) {
  case 3:
    /* BGR format (occurs in OS/2 files) */
    for (i = 0; i < cmaplen; i++) {
      sinfo->colormap[2][i] = (JSAMPLE) read_byte(sinfo);
      sinfo->colormap[1][i] = (JSAMPLE) read_byte(sinfo);
      sinfo->colormap[0][i] = (JSAMPLE) read_byte(sinfo);
    }
    break;
  case 4:
    /* BGR0 format (occurs in MS Windows files) */
    for (i = 0; i < cmaplen; i++) {
      sinfo->colormap[2][i] = (JSAMPLE) read_byte(sinfo);
      sinfo->colormap[1][i] = (JSAMPLE) read_byte(sinfo);
      sinfo->colormap[0][i] = (JSAMPLE) read_byte(sinfo);
      (void) read_byte(sinfo);
    }
    break;
  default:
    ERREXIT(sinfo->cinfo, JERR_BMP_BADCMAP);
    break;
  }
}


/*
 * Read one row of pixels.
 * The image has been read into the whole_image array, but is otherwise
 * unprocessed.  We must read it out in top-to-bottom row order, and if
 * it is an 8-bit image, we must expand colormapped pixels to 24bit format.
 */

METHODDEF(JDIMENSION)
get_8bit_row (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
/* This version is for reading 8-bit colormap indexes */
{
  bmp_source_ptr source = (bmp_source_ptr) sinfo;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

  register JSAMPARRAY colormap = source->colormap;
  JSAMPARRAY image_ptr;
__boundcheck_metadata_store((void *)(&image_ptr),(void *)((size_t)(&image_ptr)+sizeof(image_ptr)*8-1));

  register int t;
  register JSAMPROW inptr, outptr;
  register JDIMENSION col;

  /* Fetch next row from virtual array */
  source->source_row--;
  image_ptr = (*(JSAMPARRAY (*)(j_common_ptr, jvirt_sarray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(131,29,"get_8bit_row",(void *)(cinfo->mem->access_virt_sarray),(void *)cinfo->mem->access_virt_sarray)))
    ((j_common_ptr) cinfo, source->whole_image,
     source->source_row, (JDIMENSION) 1, FALSE);

  /* Expand the colormap indexes to real data */
  inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(136,11,"get_8bit_row",(void *)(&image_ptr[0]),(void *)(&image_ptr[0]))));
  outptr = source->pub.buffer[0];
  for (col = cinfo->image_width; col > 0; col--) {
    t = GETJSAMPLE(*inptr++);
    *(JSAMPROW)(__boundcheck_ptr_reference(140,12,"get_8bit_row",(void *)(outptr),(void *)(outptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(140,17,"get_8bit_row",(void *)(&colormap[0][0]),(void *)(&colormap[0][t]))));	/* can omit GETJSAMPLE() safely */
    *(JSAMPROW)(__boundcheck_ptr_reference(141,12,"get_8bit_row",(void *)(outptr),(void *)(outptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(141,17,"get_8bit_row",(void *)(&colormap[1][0]),(void *)(&colormap[1][t]))));
    *(JSAMPROW)(__boundcheck_ptr_reference(142,12,"get_8bit_row",(void *)(outptr),(void *)(outptr++))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(142,17,"get_8bit_row",(void *)(&colormap[2][0]),(void *)(&colormap[2][t]))));
  }

  return 1;
}


METHODDEF(JDIMENSION)
get_24bit_row (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
/* This version is for reading 24-bit pixels */
{
  bmp_source_ptr source = (bmp_source_ptr) sinfo;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

  JSAMPARRAY image_ptr;
__boundcheck_metadata_store((void *)(&image_ptr),(void *)((size_t)(&image_ptr)+sizeof(image_ptr)*8-1));

  register JSAMPROW inptr, outptr;
  register JDIMENSION col;

  /* Fetch next row from virtual array */
  source->source_row--;
  image_ptr = (*(JSAMPARRAY (*)(j_common_ptr, jvirt_sarray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(160,29,"get_24bit_row",(void *)(cinfo->mem->access_virt_sarray),(void *)cinfo->mem->access_virt_sarray)))
    ((j_common_ptr) cinfo, source->whole_image,
     source->source_row, (JDIMENSION) 1, FALSE);

  /* Transfer data.  Note source values are in BGR order
   * (even though Microsoft's own documents say the opposite).
   */
  inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(167,11,"get_24bit_row",(void *)(&image_ptr[0]),(void *)(&image_ptr[0]))));
  outptr = source->pub.buffer[0];
  for (col = cinfo->image_width; col > 0; col--) {
    (*(JSAMPLE *)(__boundcheck_ptr_reference(170,5,"get_24bit_row",(void *)(&outptr[0]),(void *)(&outptr[2])))) = *(JSAMPROW)(__boundcheck_ptr_reference(170,23,"get_24bit_row",(void *)(inptr),(void *)(inptr++)));	/* can omit GETJSAMPLE() safely */
    (*(JSAMPLE *)(__boundcheck_ptr_reference(171,5,"get_24bit_row",(void *)(&outptr[0]),(void *)(&outptr[1])))) = *(JSAMPROW)(__boundcheck_ptr_reference(171,23,"get_24bit_row",(void *)(inptr),(void *)(inptr++)));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(172,5,"get_24bit_row",(void *)(&outptr[0]),(void *)(&outptr[0])))) = *(JSAMPROW)(__boundcheck_ptr_reference(172,23,"get_24bit_row",(void *)(inptr),(void *)(inptr++)));
    outptr += 3;
  }

  return 1;
}


/*
 * This method loads the image into whole_image during the first call on
 * get_pixel_rows.  The get_pixel_rows pointer is then adjusted to call
 * get_8bit_row or get_24bit_row on subsequent calls.
 */

METHODDEF(JDIMENSION)
preload_image (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  bmp_source_ptr source = (bmp_source_ptr) sinfo;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

  register FILE *infile = source->pub.input_file;
  register int c;
  register JSAMPROW out_ptr;
  JSAMPARRAY image_ptr;
__boundcheck_metadata_store((void *)(&image_ptr),(void *)((size_t)(&image_ptr)+sizeof(image_ptr)*8-1));

  JDIMENSION row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));
JDIMENSION  col;
__boundcheck_metadata_store((void *)(&col),(void *)((size_t)(&col)+sizeof(col)*8-1));

  cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
__boundcheck_metadata_store((void *)(&progress),(void *)((size_t)(&progress)+sizeof(progress)*8-1));


  /* Read the data into a virtual array in input-file row order. */
  for (row = 0; row < cinfo->image_height; row++) {
    if (progress != NULL) {
      progress->pub.pass_counter = (long) row;
      progress->pub.pass_limit = (long) cinfo->image_height;
      (*(void (*)(j_common_ptr))(__boundcheck_ptr_reference(202,23,"preload_image",(void *)(progress->pub.progress_monitor),(void *)progress->pub.progress_monitor))) ((j_common_ptr) cinfo);
    }
    image_ptr = (*(JSAMPARRAY (*)(j_common_ptr, jvirt_sarray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(204,31,"preload_image",(void *)(cinfo->mem->access_virt_sarray),(void *)cinfo->mem->access_virt_sarray)))
      ((j_common_ptr) cinfo, source->whole_image,
       row, (JDIMENSION) 1, TRUE);
    out_ptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(207,15,"preload_image",(void *)(&image_ptr[0]),(void *)(&image_ptr[0]))));
    for (col = source->row_width; col > 0; col--) {
      /* inline copy of read_byte() for speed */
      if ((c = getc(infile)) == EOF)
	ERREXIT(cinfo, JERR_INPUT_EOF);
      *(JSAMPROW)(__boundcheck_ptr_reference(212,15,"preload_image",(void *)(out_ptr),(void *)(out_ptr++))) = (JSAMPLE) c;
    }
  }
  if (progress != NULL)
    progress->completed_extra_passes++;

  /* Set up to read from the virtual array in top-to-bottom order */
  switch (source->bits_per_pixel) {
  case 8:
    source->pub.get_pixel_rows = get_8bit_row;
    break;
  case 24:
    source->pub.get_pixel_rows = get_24bit_row;
    break;
  default:
    ERREXIT(cinfo, JERR_BMP_BADDEPTH);
  }
  source->source_row = cinfo->image_height;

  /* And read the first row */
  return (*(JDIMENSION (*)(j_compress_ptr, cjpeg_source_ptr))(__boundcheck_ptr_reference(232,24,"preload_image",(void *)(source->pub.get_pixel_rows),(void *)source->pub.get_pixel_rows))) (cinfo, sinfo);
}


/*
 * Read the file header; return image size and component count.
 */

METHODDEF(void)
start_input_bmp (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  bmp_source_ptr source = (bmp_source_ptr) sinfo;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

  U_CHAR bmpfileheader[14];__boundcheck_metadata_store(&bmpfileheader[0],&bmpfileheader[14-1]);

  U_CHAR bmpinfoheader[64];__boundcheck_metadata_store(&bmpinfoheader[0],&bmpinfoheader[64-1]);

#define GET_2B(array,offset)  ((unsigned int) UCH(array[offset]) + \
			       (((unsigned int) UCH(array[offset+1])) << 8))
#define GET_4B(array,offset)  ((INT32) UCH(array[offset]) + \
			       (((INT32) UCH(array[offset+1])) << 8) + \
			       (((INT32) UCH(array[offset+2])) << 16) + \
			       (((INT32) UCH(array[offset+3])) << 24))
  INT32 bfOffBits;
__boundcheck_metadata_store((void *)(&bfOffBits),(void *)((size_t)(&bfOffBits)+sizeof(bfOffBits)*8-1));

  INT32 headerSize;
__boundcheck_metadata_store((void *)(&headerSize),(void *)((size_t)(&headerSize)+sizeof(headerSize)*8-1));

  INT32 biWidth = 0;
__boundcheck_metadata_store((void *)(&biWidth),(void *)((size_t)(&biWidth)+sizeof(biWidth)*8-1));
		/* initialize to avoid compiler warning */
  INT32 biHeight = 0;
__boundcheck_metadata_store((void *)(&biHeight),(void *)((size_t)(&biHeight)+sizeof(biHeight)*8-1));

  unsigned int biPlanes;
__boundcheck_metadata_store((void *)(&biPlanes),(void *)((size_t)(&biPlanes)+sizeof(biPlanes)*8-1));

  INT32 biCompression;
__boundcheck_metadata_store((void *)(&biCompression),(void *)((size_t)(&biCompression)+sizeof(biCompression)*8-1));

  INT32 biXPelsPerMeter;
__boundcheck_metadata_store((void *)(&biXPelsPerMeter),(void *)((size_t)(&biXPelsPerMeter)+sizeof(biXPelsPerMeter)*8-1));
INT32 biYPelsPerMeter;
__boundcheck_metadata_store((void *)(&biYPelsPerMeter),(void *)((size_t)(&biYPelsPerMeter)+sizeof(biYPelsPerMeter)*8-1));

  INT32 biClrUsed = 0;
__boundcheck_metadata_store((void *)(&biClrUsed),(void *)((size_t)(&biClrUsed)+sizeof(biClrUsed)*8-1));

  int mapentrysize = 0;
__boundcheck_metadata_store((void *)(&mapentrysize),(void *)((size_t)(&mapentrysize)+sizeof(mapentrysize)*8-1));
		/* 0 indicates no colormap */
  INT32 bPad;
__boundcheck_metadata_store((void *)(&bPad),(void *)((size_t)(&bPad)+sizeof(bPad)*8-1));

  JDIMENSION row_width;
__boundcheck_metadata_store((void *)(&row_width),(void *)((size_t)(&row_width)+sizeof(row_width)*8-1));


  /* Read and verify the bitmap file header */
  if (! ReadOK(source->pub.input_file, bmpfileheader, 14))
    ERREXIT(cinfo, JERR_INPUT_EOF);
  if (GET_2B(bmpfileheader,0) != 0x4D42) /* 'BM' */
    ERREXIT(cinfo, JERR_BMP_NOT);
  bfOffBits = (INT32) GET_4B(bmpfileheader,10);
  /* We ignore the remaining fileheader fields */

  /* The infoheader might be 12 bytes (OS/2 1.x), 40 bytes (Windows),
   * or 64 bytes (OS/2 2.x).  Check the first 4 bytes to find out which.
   */
  if (! ReadOK(source->pub.input_file, bmpinfoheader, 4))
    ERREXIT(cinfo, JERR_INPUT_EOF);
  headerSize = (INT32) GET_4B(bmpinfoheader,0);
  if (headerSize < 12 || headerSize > 64)
    ERREXIT(cinfo, JERR_BMP_BADHEADER);
  if (! ReadOK(source->pub.input_file, bmpinfoheader+4, headerSize-4))
    ERREXIT(cinfo, JERR_INPUT_EOF);

  switch ((int) headerSize) {
  case 12:
    /* Decode OS/2 1.x header (Microsoft calls this a BITMAPCOREHEADER) */
    biWidth = (INT32) GET_2B(bmpinfoheader,4);
    biHeight = (INT32) GET_2B(bmpinfoheader,6);
    biPlanes = GET_2B(bmpinfoheader,8);
    source->bits_per_pixel = (int) GET_2B(bmpinfoheader,10);

    switch (source->bits_per_pixel) {
    case 8:			/* colormapped image */
      mapentrysize = 3;		/* OS/2 uses RGBTRIPLE colormap */
      TRACEMS2(cinfo, 1, JTRC_BMP_OS2_MAPPED, (int) biWidth, (int) biHeight);
      break;
    case 24:			/* RGB image */
      TRACEMS2(cinfo, 1, JTRC_BMP_OS2, (int) biWidth, (int) biHeight);
      break;
    default:
      ERREXIT(cinfo, JERR_BMP_BADDEPTH);
      break;
    }
    if (biPlanes != 1)
      ERREXIT(cinfo, JERR_BMP_BADPLANES);
    break;
  case 40:
  case 64:
    /* Decode Windows 3.x header (Microsoft calls this a BITMAPINFOHEADER) */
    /* or OS/2 2.x header, which has additional fields that we ignore */
    biWidth = GET_4B(bmpinfoheader,4);
    biHeight = GET_4B(bmpinfoheader,8);
    biPlanes = GET_2B(bmpinfoheader,12);
    source->bits_per_pixel = (int) GET_2B(bmpinfoheader,14);
    biCompression = GET_4B(bmpinfoheader,16);
    biXPelsPerMeter = GET_4B(bmpinfoheader,24);
    biYPelsPerMeter = GET_4B(bmpinfoheader,28);
    biClrUsed = GET_4B(bmpinfoheader,32);
    /* biSizeImage, biClrImportant fields are ignored */

    switch (source->bits_per_pixel) {
    case 8:			/* colormapped image */
      mapentrysize = 4;		/* Windows uses RGBQUAD colormap */
      TRACEMS2(cinfo, 1, JTRC_BMP_MAPPED, (int) biWidth, (int) biHeight);
      break;
    case 24:			/* RGB image */
      TRACEMS2(cinfo, 1, JTRC_BMP, (int) biWidth, (int) biHeight);
      break;
    default:
      ERREXIT(cinfo, JERR_BMP_BADDEPTH);
      break;
    }
    if (biPlanes != 1)
      ERREXIT(cinfo, JERR_BMP_BADPLANES);
    if (biCompression != 0)
      ERREXIT(cinfo, JERR_BMP_COMPRESSED);

    if (biXPelsPerMeter > 0 && biYPelsPerMeter > 0) {
      /* Set JFIF density parameters from the BMP data */
      cinfo->X_density = (UINT16) (biXPelsPerMeter/100); /* 100 cm per meter */
      cinfo->Y_density = (UINT16) (biYPelsPerMeter/100);
      cinfo->density_unit = 2;	/* dots/cm */
    }
    break;
  default:
    ERREXIT(cinfo, JERR_BMP_BADHEADER);
    break;
  }

  /* Compute distance to bitmap data --- will adjust for colormap below */
  bPad = bfOffBits - (headerSize + 14);

  /* Read the colormap, if any */
  if (mapentrysize > 0) {
    if (biClrUsed <= 0)
      biClrUsed = 256;		/* assume it's 256 */
    else if (biClrUsed > 256)
      ERREXIT(cinfo, JERR_BMP_BADCMAP);
    /* Allocate space to store the colormap */
    source->colormap = (*(JSAMPARRAY (*)(j_common_ptr, int, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(359,38,"start_input_bmp",(void *)(cinfo->mem->alloc_sarray),(void *)cinfo->mem->alloc_sarray)))
      ((j_common_ptr) cinfo, JPOOL_IMAGE,
       (JDIMENSION) biClrUsed, (JDIMENSION) 3);
    /* and read it from the file */
    read_colormap(source, (int) biClrUsed, mapentrysize);
    /* account for size of colormap */
    bPad -= biClrUsed * mapentrysize;
  }

  /* Skip any remaining pad bytes */
  if (bPad < 0)			/* incorrect bfOffBits value? */
    ERREXIT(cinfo, JERR_BMP_BADHEADER);
  while (--bPad >= 0) {
    (void) read_byte(source);
  }

  /* Compute row width in file, including padding to 4-byte boundary */
  if (source->bits_per_pixel == 24)
    row_width = (JDIMENSION) (biWidth * 3);
  else
    row_width = (JDIMENSION) biWidth;
  while ((row_width & 3) != 0) row_width++;
  source->row_width = row_width;

  /* Allocate space for inversion array, prepare for preload pass */
  source->whole_image = (*(jvirt_sarray_ptr (*)(j_common_ptr, int, boolean, JDIMENSION, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(384,39,"start_input_bmp",(void *)(cinfo->mem->request_virt_sarray),(void *)cinfo->mem->request_virt_sarray)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
     row_width, (JDIMENSION) biHeight, (JDIMENSION) 1);
  source->pub.get_pixel_rows = preload_image;
  if (cinfo->progress != NULL) {
    cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
__boundcheck_metadata_store((void *)(&progress),(void *)((size_t)(&progress)+sizeof(progress)*8-1));

    progress->total_extra_passes++; /* count file input as separate pass */
  }

  /* Allocate one-row buffer for returned data */
  source->pub.buffer = (*(JSAMPARRAY (*)(j_common_ptr, int, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(394,38,"start_input_bmp",(void *)(cinfo->mem->alloc_sarray),(void *)cinfo->mem->alloc_sarray)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE,
     (JDIMENSION) (biWidth * 3), (JDIMENSION) 1);
  source->pub.buffer_height = 1;

  cinfo->in_color_space = JCS_RGB;
  cinfo->input_components = 3;
  cinfo->data_precision = 8;
  cinfo->image_width = (JDIMENSION) biWidth;
  cinfo->image_height = (JDIMENSION) biHeight;
}


/*
 * Finish up at the end of the file.
 */

METHODDEF(void)
finish_input_bmp (j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
{
  /* no work */
}


/*
 * The module selection routine for BMP format input.
 */

GLOBAL(cjpeg_source_ptr)
jinit_read_bmp (j_compress_ptr cinfo)
{
  bmp_source_ptr source;
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));


  /* Create module interface object */
  source = (bmp_source_ptr)
      (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(429,21,"jinit_read_bmp",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  SIZEOF(bmp_source_struct));
  source->cinfo = cinfo;	/* make back link for subroutines */
  /* Fill in method ptrs, except get_pixel_rows which start_input sets */
  source->pub.start_input = start_input_bmp;
  source->pub.finish_input = finish_input_bmp;

  return (cjpeg_source_ptr) source;
}

#endif /* BMP_SUPPORTED */
