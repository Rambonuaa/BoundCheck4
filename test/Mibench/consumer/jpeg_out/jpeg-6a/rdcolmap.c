/*
 * rdcolmap.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file implements djpeg's "-map file" switch.  It reads a source image
 * and constructs a colormap to be supplied to the JPEG decompressor.
 *
 * Currently, these file formats are supported for the map file:
 *   GIF: the contents of the GIF's global colormap are used.
 *   PPM (either text or raw flavor): the entire file is read and
 *      each unique pixel value is entered in the map.
 * Note that reading a large PPM file will be horrendously slow.
 * Typically, a PPM-format map file should contain just one pixel
 * of each desired color.  Such a file can be extracted from an
 * ordinary image PPM file with ppmtomap(1).
 *
 * Rescaling a PPM that has a maxval unequal to MAXJSAMPLE is not
 * currently implemented.
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */

#ifdef QUANT_2PASS_SUPPORTED	/* otherwise can't quantize to supplied map */

/* Portions of this code are based on the PBMPLUS library, which is:
**
** Copyright (C) 1988 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/


/*
 * Add a (potentially) new color to the color map.
 */

LOCAL(void)
add_map_entry (j_decompress_ptr cinfo, int R, int G, int B)
{
  JSAMPROW colormap0 = cinfo->colormap[0];
__boundcheck_metadata_store((void *)(&colormap0),(void *)((size_t)(&colormap0)+sizeof(colormap0)*8-1));

  JSAMPROW colormap1 = cinfo->colormap[1];
__boundcheck_metadata_store((void *)(&colormap1),(void *)((size_t)(&colormap1)+sizeof(colormap1)*8-1));

  JSAMPROW colormap2 = cinfo->colormap[2];
__boundcheck_metadata_store((void *)(&colormap2),(void *)((size_t)(&colormap2)+sizeof(colormap2)*8-1));

  int ncolors = cinfo->actual_number_of_colors;
__boundcheck_metadata_store((void *)(&ncolors),(void *)((size_t)(&ncolors)+sizeof(ncolors)*8-1));

  int index;
__boundcheck_metadata_store((void *)(&index),(void *)((size_t)(&index)+sizeof(index)*8-1));


  /* Check for duplicate color. */
  for (index = 0; index < ncolors; index++) {
    if (GETJSAMPLE(colormap0[index]) == R &&
	GETJSAMPLE(colormap1[index]) == G &&
	GETJSAMPLE(colormap2[index]) == B)
      return;			/* color is already in map */
  }

  /* Check for map overflow. */
  if (ncolors >= (MAXJSAMPLE+1))
    ERREXIT1(cinfo, JERR_QUANT_MANY_COLORS, (MAXJSAMPLE+1));

  /* OK, add color to map. */
  (*(JSAMPLE *)(__boundcheck_ptr_reference(67,3,"add_map_entry",(void *)(&colormap0[0]),(void *)(&colormap0[ncolors])))) = (JSAMPLE) R;
  (*(JSAMPLE *)(__boundcheck_ptr_reference(68,3,"add_map_entry",(void *)(&colormap1[0]),(void *)(&colormap1[ncolors])))) = (JSAMPLE) G;
  (*(JSAMPLE *)(__boundcheck_ptr_reference(69,3,"add_map_entry",(void *)(&colormap2[0]),(void *)(&colormap2[ncolors])))) = (JSAMPLE) B;
  cinfo->actual_number_of_colors++;
}


/*
 * Extract color map from a GIF file.
 */

LOCAL(void)
read_gif_map (j_decompress_ptr cinfo, FILE * infile)
{
  int header[13];__boundcheck_metadata_store(&header[0],&header[13-1]);

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  colormaplen;
__boundcheck_metadata_store((void *)(&colormaplen),(void *)((size_t)(&colormaplen)+sizeof(colormaplen)*8-1));

  int R;
__boundcheck_metadata_store((void *)(&R),(void *)((size_t)(&R)+sizeof(R)*8-1));
int  G;
__boundcheck_metadata_store((void *)(&G),(void *)((size_t)(&G)+sizeof(G)*8-1));
int  B;
__boundcheck_metadata_store((void *)(&B),(void *)((size_t)(&B)+sizeof(B)*8-1));


  /* Initial 'G' has already been read by read_color_map */
  /* Read the rest of the GIF header and logical screen descriptor */
  for (i = 1; i < 13; i++) {
    if ((header[_RV_insert_check(0,13,88,10,"read_gif_map",i)] = getc(infile)) == EOF)
      ERREXIT(cinfo, JERR_BAD_CMAP_FILE);
  }

  /* Verify GIF Header */
  if (header[_RV_insert_check(0,13,93,7,"read_gif_map",1)] != 'I' || header[_RV_insert_check(0,13,93,27,"read_gif_map",2)] != 'F')
    ERREXIT(cinfo, JERR_BAD_CMAP_FILE);

  /* There must be a global color map. */
  if ((header[_RV_insert_check(0,13,97,8,"read_gif_map",10)] & 0x80) == 0)
    ERREXIT(cinfo, JERR_BAD_CMAP_FILE);

  /* OK, fetch it. */
  colormaplen = 2 << (header[_RV_insert_check(0,13,101,23,"read_gif_map",10)] & 0x07);

  for (i = 0; i < colormaplen; i++) {
    R = getc(infile);
    G = getc(infile);
    B = getc(infile);
    if (R == EOF || G == EOF || B == EOF)
      ERREXIT(cinfo, JERR_BAD_CMAP_FILE);
    add_map_entry(cinfo,
		  R << (BITS_IN_JSAMPLE-8),
		  G << (BITS_IN_JSAMPLE-8),
		  B << (BITS_IN_JSAMPLE-8));
  }
}


/* Support routines for reading PPM */


LOCAL(int)
pbm_getc (FILE * infile)
/* Read next char, skipping over any comments */
/* A comment/newline sequence is returned as a newline */
{
  register int ch;
  
  ch = getc(infile);
  if (ch == '#') {
    do {
      ch = getc(infile);
    } while (ch != '\n' && ch != EOF);
  }
  return ch;
}


LOCAL(unsigned int)
read_pbm_integer (j_decompress_ptr cinfo, FILE * infile)
/* Read an unsigned decimal integer from the PPM file */
/* Swallows one trailing character after the integer */
/* Note that on a 16-bit-int machine, only values up to 64k can be read. */
/* This should not be a problem in practice. */
{
  register int ch;
  register unsigned int val;
  
  /* Skip any leading whitespace */
  do {
    ch = pbm_getc(infile);
    if (ch == EOF)
      ERREXIT(cinfo, JERR_BAD_CMAP_FILE);
  } while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
  
  if (ch < '0' || ch > '9')
    ERREXIT(cinfo, JERR_BAD_CMAP_FILE);
  
  val = ch - '0';
  while ((ch = pbm_getc(infile)) >= '0' && ch <= '9') {
    val *= 10;
    val += ch - '0';
  }
  return val;
}


/*
 * Extract color map from a PPM file.
 */

LOCAL(void)
read_ppm_map (j_decompress_ptr cinfo, FILE * infile)
{
  int c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));

  unsigned int w;
__boundcheck_metadata_store((void *)(&w),(void *)((size_t)(&w)+sizeof(w)*8-1));
unsigned int  h;
__boundcheck_metadata_store((void *)(&h),(void *)((size_t)(&h)+sizeof(h)*8-1));
unsigned int  maxval;
__boundcheck_metadata_store((void *)(&maxval),(void *)((size_t)(&maxval)+sizeof(maxval)*8-1));
unsigned int  row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));
unsigned int  col;
__boundcheck_metadata_store((void *)(&col),(void *)((size_t)(&col)+sizeof(col)*8-1));

  int R;
__boundcheck_metadata_store((void *)(&R),(void *)((size_t)(&R)+sizeof(R)*8-1));
int  G;
__boundcheck_metadata_store((void *)(&G),(void *)((size_t)(&G)+sizeof(G)*8-1));
int  B;
__boundcheck_metadata_store((void *)(&B),(void *)((size_t)(&B)+sizeof(B)*8-1));


  /* Initial 'P' has already been read by read_color_map */
  c = getc(infile);		/* save format discriminator for a sec */

  /* while we fetch the remaining header info */
  w = read_pbm_integer(cinfo, infile);
  h = read_pbm_integer(cinfo, infile);
  maxval = read_pbm_integer(cinfo, infile);

  if (w <= 0 || h <= 0 || maxval <= 0) /* error check */
    ERREXIT(cinfo, JERR_BAD_CMAP_FILE);

  /* For now, we don't support rescaling from an unusual maxval. */
  if (maxval != (unsigned int) MAXJSAMPLE)
    ERREXIT(cinfo, JERR_BAD_CMAP_FILE);

  switch (c) {
  case '3':			/* it's a text-format PPM file */
    for (row = 0; row < h; row++) {
      for (col = 0; col < w; col++) {
	R = read_pbm_integer(cinfo, infile);
	G = read_pbm_integer(cinfo, infile);
	B = read_pbm_integer(cinfo, infile);
	add_map_entry(cinfo, R, G, B);
      }
    }
    break;

  case '6':			/* it's a raw-format PPM file */
    for (row = 0; row < h; row++) {
      for (col = 0; col < w; col++) {
	R = pbm_getc(infile);
	G = pbm_getc(infile);
	B = pbm_getc(infile);
	if (R == EOF || G == EOF || B == EOF)
	  ERREXIT(cinfo, JERR_BAD_CMAP_FILE);
	add_map_entry(cinfo, R, G, B);
      }
    }
    break;

  default:
    ERREXIT(cinfo, JERR_BAD_CMAP_FILE);
    break;
  }
}


/*
 * Main entry point from djpeg.c.
 *  Input: opened input file (from file name argument on command line).
 *  Output: colormap and actual_number_of_colors fields are set in cinfo.
 */

GLOBAL(void)
read_color_map (j_decompress_ptr cinfo, FILE * infile)
{
  /* Allocate space for a color map of maximum supported size. */
  cinfo->colormap = (*(JSAMPARRAY (*)(j_common_ptr, int, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(234,35,"read_color_map",(void *)(cinfo->mem->alloc_sarray),(void *)cinfo->mem->alloc_sarray)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE,
     (JDIMENSION) (MAXJSAMPLE+1), (JDIMENSION) 3);
  cinfo->actual_number_of_colors = 0; /* initialize map to empty */

  /* Read first byte to determine file format */
  switch (getc(infile)) {
  case 'G':
    read_gif_map(cinfo, infile);
    break;
  case 'P':
    read_ppm_map(cinfo, infile);
    break;
  default:
    ERREXIT(cinfo, JERR_BAD_CMAP_FILE);
    break;
  }
}

#endif /* QUANT_2PASS_SUPPORTED */
