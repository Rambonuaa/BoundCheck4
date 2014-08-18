/*
 * wrbmp.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains routines to write output images in Microsoft "BMP"
 * format (MS Windows 3.x and OS/2 1.x flavors).
 * Either 8-bit colormapped or 24-bit full-color format can be written.
 * No compression is supported.
 *
 * These routines may need modification for non-Unix environments or
 * specialized applications.  As they stand, they assume output to
 * an ordinary stdio stream.
 *
 * This code contributed by James Arthur Boucher.
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */

#ifdef BMP_SUPPORTED


/*
 * To support 12-bit JPEG data, we'd have to scale output down to 8 bits.
 * This is not yet implemented.
 */

#if BITS_IN_JSAMPLE != 8
  Sorry, this code only copes with 8-bit JSAMPLEs. /* deliberate syntax err */
#endif

/*
 * Since BMP stores scanlines bottom-to-top, we have to invert the image
 * from JPEG's top-to-bottom order.  To do this, we save the outgoing data
 * in a virtual array during put_pixel_row calls, then actually emit the
 * BMP file during finish_output.  The virtual array contains one JSAMPLE per
 * pixel if the output is grayscale or colormapped, three if it is full color.
 */

/* Private version of data destination object */

typedef struct {
  struct djpeg_dest_struct pub;	/* public fields */

  boolean is_os2;		/* saves the OS2 format request flag */

  jvirt_sarray_ptr whole_image;	/* needed to reverse row order */
  JDIMENSION data_width;	/* JSAMPLEs per row */
  JDIMENSION row_width;		/* physical width of one row in the BMP file */
  int pad_bytes;		/* number of padding bytes needed per row */
  JDIMENSION cur_output_row;	/* next row# to write to virtual array */
} bmp_dest_struct;

typedef bmp_dest_struct * bmp_dest_ptr;


/* Forward declarations */
LOCAL(void) write_colormap
	JPP((j_decompress_ptr cinfo, bmp_dest_ptr dest,
	     int map_colors, int map_entry_size));


/*
 * Write some pixel data.
 * In this module rows_supplied will always be 1.
 */

METHODDEF(void)
put_pixel_rows (j_decompress_ptr cinfo, djpeg_dest_ptr dinfo,
		JDIMENSION rows_supplied)
/* This version is for writing 24-bit pixels */
{
  bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
__boundcheck_metadata_store((void *)(&dest),(void *)((size_t)(&dest)+sizeof(dest)*8-1));

  JSAMPARRAY image_ptr;
__boundcheck_metadata_store((void *)(&image_ptr),(void *)((size_t)(&image_ptr)+sizeof(image_ptr)*8-1));

  register JSAMPROW inptr, outptr;
  register JDIMENSION col;
  int pad;
__boundcheck_metadata_store((void *)(&pad),(void *)((size_t)(&pad)+sizeof(pad)*8-1));


  /* Access next row in virtual array */
  image_ptr = (*(JSAMPARRAY (*)(j_common_ptr, jvirt_sarray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(82,29,"put_pixel_rows",(void *)(cinfo->mem->access_virt_sarray),(void *)cinfo->mem->access_virt_sarray)))
    ((j_common_ptr) cinfo, dest->whole_image,
     dest->cur_output_row, (JDIMENSION) 1, TRUE);
  dest->cur_output_row++;

  /* Transfer data.  Note destination values must be in BGR order
   * (even though Microsoft's own documents say the opposite).
   */
  inptr = dest->pub.buffer[0];
  outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(91,12,"put_pixel_rows",(void *)(&image_ptr[0]),(void *)(&image_ptr[0]))));
  for (col = cinfo->output_width; col > 0; col--) {
    (*(JSAMPLE *)(__boundcheck_ptr_reference(93,5,"put_pixel_rows",(void *)(&outptr[0]),(void *)(&outptr[2])))) = *(JSAMPROW)(__boundcheck_ptr_reference(93,23,"put_pixel_rows",(void *)(inptr),(void *)(inptr++)));	/* can omit GETJSAMPLE() safely */
    (*(JSAMPLE *)(__boundcheck_ptr_reference(94,5,"put_pixel_rows",(void *)(&outptr[0]),(void *)(&outptr[1])))) = *(JSAMPROW)(__boundcheck_ptr_reference(94,23,"put_pixel_rows",(void *)(inptr),(void *)(inptr++)));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(95,5,"put_pixel_rows",(void *)(&outptr[0]),(void *)(&outptr[0])))) = *(JSAMPROW)(__boundcheck_ptr_reference(95,23,"put_pixel_rows",(void *)(inptr),(void *)(inptr++)));
    outptr += 3;
  }

  /* Zero out the pad bytes. */
  pad = dest->pad_bytes;
  while (--pad >= 0)
    *(JSAMPROW)(__boundcheck_ptr_reference(102,12,"put_pixel_rows",(void *)(outptr),(void *)(outptr++))) = 0;
}

METHODDEF(void)
put_gray_rows (j_decompress_ptr cinfo, djpeg_dest_ptr dinfo,
	       JDIMENSION rows_supplied)
/* This version is for grayscale OR quantized color output */
{
  bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
__boundcheck_metadata_store((void *)(&dest),(void *)((size_t)(&dest)+sizeof(dest)*8-1));

  JSAMPARRAY image_ptr;
__boundcheck_metadata_store((void *)(&image_ptr),(void *)((size_t)(&image_ptr)+sizeof(image_ptr)*8-1));

  register JSAMPROW inptr, outptr;
  register JDIMENSION col;
  int pad;
__boundcheck_metadata_store((void *)(&pad),(void *)((size_t)(&pad)+sizeof(pad)*8-1));


  /* Access next row in virtual array */
  image_ptr = (*(JSAMPARRAY (*)(j_common_ptr, jvirt_sarray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(117,29,"put_gray_rows",(void *)(cinfo->mem->access_virt_sarray),(void *)cinfo->mem->access_virt_sarray)))
    ((j_common_ptr) cinfo, dest->whole_image,
     dest->cur_output_row, (JDIMENSION) 1, TRUE);
  dest->cur_output_row++;

  /* Transfer data. */
  inptr = dest->pub.buffer[0];
  outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(124,12,"put_gray_rows",(void *)(&image_ptr[0]),(void *)(&image_ptr[0]))));
  for (col = cinfo->output_width; col > 0; col--) {
    *(JSAMPROW)(__boundcheck_ptr_reference(126,12,"put_gray_rows",(void *)(outptr),(void *)(outptr++))) = *(JSAMPROW)(__boundcheck_ptr_reference(126,23,"put_gray_rows",(void *)(inptr),(void *)(inptr++)));	/* can omit GETJSAMPLE() safely */
  }

  /* Zero out the pad bytes. */
  pad = dest->pad_bytes;
  while (--pad >= 0)
    *(JSAMPROW)(__boundcheck_ptr_reference(132,12,"put_gray_rows",(void *)(outptr),(void *)(outptr++))) = 0;
}


/*
 * Startup: normally writes the file header.
 * In this module we may as well postpone everything until finish_output.
 */

METHODDEF(void)
start_output_bmp (j_decompress_ptr cinfo, djpeg_dest_ptr dinfo)
{
  /* no work here */
}


/*
 * Finish up at the end of the file.
 *
 * Here is where we really output the BMP file.
 *
 * First, routines to write the Windows and OS/2 variants of the file header.
 */

LOCAL(void)
write_bmp_header (j_decompress_ptr cinfo, bmp_dest_ptr dest)
/* Write a Windows-style BMP file header, including colormap if needed */
{
  char bmpfileheader[14];__boundcheck_metadata_store(&bmpfileheader[0],&bmpfileheader[14-1]);

  char bmpinfoheader[40];__boundcheck_metadata_store(&bmpinfoheader[0],&bmpinfoheader[40-1]);

#define PUT_2B(array,offset,value)  \
	(array[offset] = (char) ((value) & 0xFF), \
	 array[offset+1] = (char) (((value) >> 8) & 0xFF))
#define PUT_4B(array,offset,value)  \
	(array[offset] = (char) ((value) & 0xFF), \
	 array[offset+1] = (char) (((value) >> 8) & 0xFF), \
	 array[offset+2] = (char) (((value) >> 16) & 0xFF), \
	 array[offset+3] = (char) (((value) >> 24) & 0xFF))
  INT32 headersize;
__boundcheck_metadata_store((void *)(&headersize),(void *)((size_t)(&headersize)+sizeof(headersize)*8-1));
INT32  bfSize;
__boundcheck_metadata_store((void *)(&bfSize),(void *)((size_t)(&bfSize)+sizeof(bfSize)*8-1));

  int bits_per_pixel;
__boundcheck_metadata_store((void *)(&bits_per_pixel),(void *)((size_t)(&bits_per_pixel)+sizeof(bits_per_pixel)*8-1));
int  cmap_entries;
__boundcheck_metadata_store((void *)(&cmap_entries),(void *)((size_t)(&cmap_entries)+sizeof(cmap_entries)*8-1));


  /* Compute colormap size and total file size */
  if (cinfo->out_color_space == JCS_RGB) {
    if (cinfo->quantize_colors) {
      /* Colormapped RGB */
      bits_per_pixel = 8;
      cmap_entries = 256;
    } else {
      /* Unquantized, full color RGB */
      bits_per_pixel = 24;
      cmap_entries = 0;
    }
  } else {
    /* Grayscale output.  We need to fake a 256-entry colormap. */
    bits_per_pixel = 8;
    cmap_entries = 256;
  }
  /* File size */
  headersize = 14 + 40 + cmap_entries * 4; /* Header and colormap */
  bfSize = headersize + (INT32) dest->row_width * (INT32) cinfo->output_height;
  
  /* Set unused fields of header to 0 */
  MEMZERO(bmpfileheader, SIZEOF(bmpfileheader));
  MEMZERO(bmpinfoheader, SIZEOF(bmpinfoheader));

  /* Fill the file header */
  bmpfileheader[_RV_insert_check(0,14,198,3,"write_bmp_header",0)] = 0x42;	/* first 2 bytes are ASCII 'B', 'M' */
  bmpfileheader[_RV_insert_check(0,14,199,3,"write_bmp_header",1)] = 0x4D;
  PUT_4B(bmpfileheader, 2, bfSize); /* bfSize */
  /* we leave bfReserved1 & bfReserved2 = 0 */
  PUT_4B(bmpfileheader, 10, headersize); /* bfOffBits */

  /* Fill the info header (Microsoft calls this a BITMAPINFOHEADER) */
  PUT_2B(bmpinfoheader, 0, 40);	/* biSize */
  PUT_4B(bmpinfoheader, 4, cinfo->output_width); /* biWidth */
  PUT_4B(bmpinfoheader, 8, cinfo->output_height); /* biHeight */
  PUT_2B(bmpinfoheader, 12, 1);	/* biPlanes - must be 1 */
  PUT_2B(bmpinfoheader, 14, bits_per_pixel); /* biBitCount */
  /* we leave biCompression = 0, for none */
  /* we leave biSizeImage = 0; this is correct for uncompressed data */
  if (cinfo->density_unit == 2) { /* if have density in dots/cm, then */
    PUT_4B(bmpinfoheader, 24, (INT32) (cinfo->X_density*100)); /* XPels/M */
    PUT_4B(bmpinfoheader, 28, (INT32) (cinfo->Y_density*100)); /* XPels/M */
  }
  PUT_2B(bmpinfoheader, 32, cmap_entries); /* biClrUsed */
  /* we leave biClrImportant = 0 */

  if (JFWRITE(dest->pub.output_file, bmpfileheader, 14) != (size_t) 14)
    ERREXIT(cinfo, JERR_FILE_WRITE);
  if (JFWRITE(dest->pub.output_file, bmpinfoheader, 40) != (size_t) 40)
    ERREXIT(cinfo, JERR_FILE_WRITE);

  if (cmap_entries > 0)
    write_colormap(cinfo, dest, cmap_entries, 4);
}


LOCAL(void)
write_os2_header (j_decompress_ptr cinfo, bmp_dest_ptr dest)
/* Write an OS2-style BMP file header, including colormap if needed */
{
  char bmpfileheader[14];__boundcheck_metadata_store(&bmpfileheader[0],&bmpfileheader[14-1]);

  char bmpcoreheader[12];__boundcheck_metadata_store(&bmpcoreheader[0],&bmpcoreheader[12-1]);

  INT32 headersize;
__boundcheck_metadata_store((void *)(&headersize),(void *)((size_t)(&headersize)+sizeof(headersize)*8-1));
INT32  bfSize;
__boundcheck_metadata_store((void *)(&bfSize),(void *)((size_t)(&bfSize)+sizeof(bfSize)*8-1));

  int bits_per_pixel;
__boundcheck_metadata_store((void *)(&bits_per_pixel),(void *)((size_t)(&bits_per_pixel)+sizeof(bits_per_pixel)*8-1));
int  cmap_entries;
__boundcheck_metadata_store((void *)(&cmap_entries),(void *)((size_t)(&cmap_entries)+sizeof(cmap_entries)*8-1));


  /* Compute colormap size and total file size */
  if (cinfo->out_color_space == JCS_RGB) {
    if (cinfo->quantize_colors) {
      /* Colormapped RGB */
      bits_per_pixel = 8;
      cmap_entries = 256;
    } else {
      /* Unquantized, full color RGB */
      bits_per_pixel = 24;
      cmap_entries = 0;
    }
  } else {
    /* Grayscale output.  We need to fake a 256-entry colormap. */
    bits_per_pixel = 8;
    cmap_entries = 256;
  }
  /* File size */
  headersize = 14 + 12 + cmap_entries * 3; /* Header and colormap */
  bfSize = headersize + (INT32) dest->row_width * (INT32) cinfo->output_height;
  
  /* Set unused fields of header to 0 */
  MEMZERO(bmpfileheader, SIZEOF(bmpfileheader));
  MEMZERO(bmpcoreheader, SIZEOF(bmpcoreheader));

  /* Fill the file header */
  bmpfileheader[_RV_insert_check(0,14,263,3,"write_os2_header",0)] = 0x42;	/* first 2 bytes are ASCII 'B', 'M' */
  bmpfileheader[_RV_insert_check(0,14,264,3,"write_os2_header",1)] = 0x4D;
  PUT_4B(bmpfileheader, 2, bfSize); /* bfSize */
  /* we leave bfReserved1 & bfReserved2 = 0 */
  PUT_4B(bmpfileheader, 10, headersize); /* bfOffBits */

  /* Fill the info header (Microsoft calls this a BITMAPCOREHEADER) */
  PUT_2B(bmpcoreheader, 0, 12);	/* bcSize */
  PUT_2B(bmpcoreheader, 4, cinfo->output_width); /* bcWidth */
  PUT_2B(bmpcoreheader, 6, cinfo->output_height); /* bcHeight */
  PUT_2B(bmpcoreheader, 8, 1);	/* bcPlanes - must be 1 */
  PUT_2B(bmpcoreheader, 10, bits_per_pixel); /* bcBitCount */

  if (JFWRITE(dest->pub.output_file, bmpfileheader, 14) != (size_t) 14)
    ERREXIT(cinfo, JERR_FILE_WRITE);
  if (JFWRITE(dest->pub.output_file, bmpcoreheader, 12) != (size_t) 12)
    ERREXIT(cinfo, JERR_FILE_WRITE);

  if (cmap_entries > 0)
    write_colormap(cinfo, dest, cmap_entries, 3);
}


/*
 * Write the colormap.
 * Windows uses BGR0 map entries; OS/2 uses BGR entries.
 */

LOCAL(void)
write_colormap (j_decompress_ptr cinfo, bmp_dest_ptr dest,
		int map_colors, int map_entry_size)
{
  JSAMPARRAY colormap = cinfo->colormap;
__boundcheck_metadata_store((void *)(&colormap),(void *)((size_t)(&colormap)+sizeof(colormap)*8-1));

  int num_colors = cinfo->actual_number_of_colors;
__boundcheck_metadata_store((void *)(&num_colors),(void *)((size_t)(&num_colors)+sizeof(num_colors)*8-1));

  FILE * outfile = dest->pub.output_file;
__boundcheck_metadata_store((void *)(&outfile),(void *)((size_t)(&outfile)+sizeof(outfile)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  if (colormap != NULL) {
    if (cinfo->out_color_components == 3) {
      /* Normal case with RGB colormap */
      for (i = 0; i < num_colors; i++) {
	putc(GETJSAMPLE(colormap[2][i]), outfile);
	putc(GETJSAMPLE(colormap[1][i]), outfile);
	putc(GETJSAMPLE(colormap[0][i]), outfile);
	if (map_entry_size == 4)
	  putc(0, outfile);
      }
    } else {
      /* Grayscale colormap (only happens with grayscale quantization) */
      for (i = 0; i < num_colors; i++) {
	putc(GETJSAMPLE(colormap[0][i]), outfile);
	putc(GETJSAMPLE(colormap[0][i]), outfile);
	putc(GETJSAMPLE(colormap[0][i]), outfile);
	if (map_entry_size == 4)
	  putc(0, outfile);
      }
    }
  } else {
    /* If no colormap, must be grayscale data.  Generate a linear "map". */
    for (i = 0; i < 256; i++) {
      putc(i, outfile);
      putc(i, outfile);
      putc(i, outfile);
      if (map_entry_size == 4)
	putc(0, outfile);
    }
  }
  /* Pad colormap with zeros to ensure specified number of colormap entries */ 
  if (i > map_colors)
    ERREXIT1(cinfo, JERR_TOO_MANY_COLORS, i);
  for (; i < map_colors; i++) {
    putc(0, outfile);
    putc(0, outfile);
    putc(0, outfile);
    if (map_entry_size == 4)
      putc(0, outfile);
  }
}


METHODDEF(void)
finish_output_bmp (j_decompress_ptr cinfo, djpeg_dest_ptr dinfo)
{
  bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
__boundcheck_metadata_store((void *)(&dest),(void *)((size_t)(&dest)+sizeof(dest)*8-1));

  register FILE * outfile = dest->pub.output_file;
  JSAMPARRAY image_ptr;
__boundcheck_metadata_store((void *)(&image_ptr),(void *)((size_t)(&image_ptr)+sizeof(image_ptr)*8-1));

  register JSAMPROW data_ptr;
  JDIMENSION row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));

  register JDIMENSION col;
  cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
__boundcheck_metadata_store((void *)(&progress),(void *)((size_t)(&progress)+sizeof(progress)*8-1));


  /* Write the header and colormap */
  if (dest->is_os2)
    write_os2_header(cinfo, dest);
  else
    write_bmp_header(cinfo, dest);

  /* Write the file body from our virtual array */
  for (row = cinfo->output_height; row > 0; row--) {
    if (progress != NULL) {
      progress->pub.pass_counter = (long) (cinfo->output_height - row);
      progress->pub.pass_limit = (long) cinfo->output_height;
      (*(void (*)(j_common_ptr))(__boundcheck_ptr_reference(365,23,"finish_output_bmp",(void *)(progress->pub.progress_monitor),(void *)progress->pub.progress_monitor))) ((j_common_ptr) cinfo);
    }
    image_ptr = (*(JSAMPARRAY (*)(j_common_ptr, jvirt_sarray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(367,31,"finish_output_bmp",(void *)(cinfo->mem->access_virt_sarray),(void *)cinfo->mem->access_virt_sarray)))
      ((j_common_ptr) cinfo, dest->whole_image, row-1, (JDIMENSION) 1, FALSE);
    data_ptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(369,16,"finish_output_bmp",(void *)(&image_ptr[0]),(void *)(&image_ptr[0]))));
    for (col = dest->row_width; col > 0; col--) {
      putc(GETJSAMPLE(*data_ptr), outfile);
      data_ptr++;
    }
  }
  if (progress != NULL)
    progress->completed_extra_passes++;

  /* Make sure we wrote the output file OK */
  fflush(outfile);
  if (ferror(outfile))
    ERREXIT(cinfo, JERR_FILE_WRITE);
}


/*
 * The module selection routine for BMP format output.
 */

GLOBAL(djpeg_dest_ptr)
jinit_write_bmp (j_decompress_ptr cinfo, boolean is_os2)
{
  bmp_dest_ptr dest;
__boundcheck_metadata_store((void *)(&dest),(void *)((size_t)(&dest)+sizeof(dest)*8-1));

  JDIMENSION row_width;
__boundcheck_metadata_store((void *)(&row_width),(void *)((size_t)(&row_width)+sizeof(row_width)*8-1));


  /* Create module interface object, fill in method pointers */
  dest = (bmp_dest_ptr)
      (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(397,21,"jinit_write_bmp",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  SIZEOF(bmp_dest_struct));
  dest->pub.start_output = start_output_bmp;
  dest->pub.finish_output = finish_output_bmp;
  dest->is_os2 = is_os2;

  if (cinfo->out_color_space == JCS_GRAYSCALE) {
    dest->pub.put_pixel_rows = put_gray_rows;
  } else if (cinfo->out_color_space == JCS_RGB) {
    if (cinfo->quantize_colors)
      dest->pub.put_pixel_rows = put_gray_rows;
    else
      dest->pub.put_pixel_rows = put_pixel_rows;
  } else {
    ERREXIT(cinfo, JERR_BMP_COLORSPACE);
  }

  /* Calculate output image dimensions so we can allocate space */
  jpeg_calc_output_dimensions(cinfo);

  /* Determine width of rows in the BMP file (padded to 4-byte boundary). */
  row_width = cinfo->output_width * cinfo->output_components;
  dest->data_width = row_width;
  while ((row_width & 3) != 0) row_width++;
  dest->row_width = row_width;
  dest->pad_bytes = (int) (row_width - dest->data_width);

  /* Allocate space for inversion array, prepare for write pass */
  dest->whole_image = (*(jvirt_sarray_ptr (*)(j_common_ptr, int, boolean, JDIMENSION, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(425,37,"jinit_write_bmp",(void *)(cinfo->mem->request_virt_sarray),(void *)cinfo->mem->request_virt_sarray)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
     row_width, cinfo->output_height, (JDIMENSION) 1);
  dest->cur_output_row = 0;
  if (cinfo->progress != NULL) {
    cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
__boundcheck_metadata_store((void *)(&progress),(void *)((size_t)(&progress)+sizeof(progress)*8-1));

    progress->total_extra_passes++; /* count file input as separate pass */
  }

  /* Create decompressor output buffer. */
  dest->pub.buffer = (*(JSAMPARRAY (*)(j_common_ptr, int, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(435,36,"jinit_write_bmp",(void *)(cinfo->mem->alloc_sarray),(void *)cinfo->mem->alloc_sarray)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE, row_width, (JDIMENSION) 1);
  dest->pub.buffer_height = 1;

  return (djpeg_dest_ptr) dest;
}

#endif /* BMP_SUPPORTED */
