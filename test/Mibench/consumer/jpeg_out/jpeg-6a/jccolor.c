/*
 * jccolor.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains input colorspace conversion routines.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/* Private subobject */

typedef struct {
  struct jpeg_color_converter pub; /* public fields */

  /* Private state for RGB->YCC conversion */
  INT32 * rgb_ycc_tab;		/* => table for RGB to YCbCr conversion */
} my_color_converter;

typedef my_color_converter * my_cconvert_ptr;


/**************** RGB -> YCbCr conversion: most common case **************/

/*
 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
 * The conversion equations to be implemented are therefore
 *	Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *	Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + CENTERJSAMPLE
 *	Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + CENTERJSAMPLE
 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
 * Note: older versions of the IJG code used a zero offset of MAXJSAMPLE/2,
 * rather than CENTERJSAMPLE, for Cb and Cr.  This gave equal positive and
 * negative swings for Cb/Cr, but meant that grayscale values (Cb=Cr=0)
 * were not represented exactly.  Now we sacrifice exact representation of
 * maximum red and maximum blue in order to get exact grayscales.
 *
 * To avoid floating-point arithmetic, we represent the fractional constants
 * as integers scaled up by 2^16 (about 4 digits precision); we have to divide
 * the products by 2^16, with appropriate rounding, to get the correct answer.
 *
 * For even more speed, we avoid doing any multiplications in the inner loop
 * by precalculating the constants times R,G,B for all possible values.
 * For 8-bit JSAMPLEs this is very reasonable (only 256 entries per table);
 * for 12-bit samples it is still acceptable.  It's not very reasonable for
 * 16-bit samples, but if you want lossless storage you shouldn't be changing
 * colorspace anyway.
 * The CENTERJSAMPLE offsets and the rounding fudge-factor of 0.5 are included
 * in the tables to save adding them separately in the inner loop.
 */

#define SCALEBITS	16	/* speediest right-shift on some machines */
#define CBCR_OFFSET	((INT32) CENTERJSAMPLE << SCALEBITS)
#define ONE_HALF	((INT32) 1 << (SCALEBITS-1))
#define FIX(x)		((INT32) ((x) * (1L<<SCALEBITS) + 0.5))

/* We allocate one big table and divide it up into eight parts, instead of
 * doing eight alloc_small requests.  This lets us use a single table base
 * address, which can be held in a register in the inner loops on many
 * machines (more than can hold all eight addresses, anyway).
 */

#define R_Y_OFF		0			/* offset to R => Y section */
#define G_Y_OFF		(1*(MAXJSAMPLE+1))	/* offset to G => Y section */
#define B_Y_OFF		(2*(MAXJSAMPLE+1))	/* etc. */
#define R_CB_OFF	(3*(MAXJSAMPLE+1))
#define G_CB_OFF	(4*(MAXJSAMPLE+1))
#define B_CB_OFF	(5*(MAXJSAMPLE+1))
#define R_CR_OFF	B_CB_OFF		/* B=>Cb, R=>Cr are the same */
#define G_CR_OFF	(6*(MAXJSAMPLE+1))
#define B_CR_OFF	(7*(MAXJSAMPLE+1))
#define TABLE_SIZE	(8*(MAXJSAMPLE+1))


/*
 * Initialize for RGB->YCC colorspace conversion.
 */

METHODDEF(void)
rgb_ycc_start (j_compress_ptr cinfo)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));

  INT32 * rgb_ycc_tab;
__boundcheck_metadata_store((void *)(&rgb_ycc_tab),(void *)((size_t)(&rgb_ycc_tab)+sizeof(rgb_ycc_tab)*8-1));

  INT32 i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  /* Allocate and fill in the conversion tables. */
  cconvert->rgb_ycc_tab = rgb_ycc_tab = (INT32 *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(94,19,"rgb_ycc_start",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(TABLE_SIZE * SIZEOF(INT32)));

  for (i = 0; i <= MAXJSAMPLE; i++) {
    (*(INT32 *)(__boundcheck_ptr_reference(98,5,"rgb_ycc_start",(void *)(&rgb_ycc_tab[0]),(void *)(&rgb_ycc_tab[i + 0])))) = FIX(0.29900) * i;
    (*(INT32 *)(__boundcheck_ptr_reference(99,5,"rgb_ycc_start",(void *)(&rgb_ycc_tab[0]),(void *)(&rgb_ycc_tab[i + (1 * (255 + 1))])))) = FIX(0.58700) * i;
    (*(INT32 *)(__boundcheck_ptr_reference(100,5,"rgb_ycc_start",(void *)(&rgb_ycc_tab[0]),(void *)(&rgb_ycc_tab[i + (2 * (255 + 1))])))) = FIX(0.11400) * i     + ONE_HALF;
    (*(INT32 *)(__boundcheck_ptr_reference(101,5,"rgb_ycc_start",(void *)(&rgb_ycc_tab[0]),(void *)(&rgb_ycc_tab[i + (3 * (255 + 1))])))) = (-FIX(0.16874)) * i;
    (*(INT32 *)(__boundcheck_ptr_reference(102,5,"rgb_ycc_start",(void *)(&rgb_ycc_tab[0]),(void *)(&rgb_ycc_tab[i + (4 * (255 + 1))])))) = (-FIX(0.33126)) * i;
    /* We use a rounding fudge-factor of 0.5-epsilon for Cb and Cr.
     * This ensures that the maximum output will round to MAXJSAMPLE
     * not MAXJSAMPLE+1, and thus that we don't have to range-limit.
     */
    (*(INT32 *)(__boundcheck_ptr_reference(107,5,"rgb_ycc_start",(void *)(&rgb_ycc_tab[0]),(void *)(&rgb_ycc_tab[i + (5 * (255 + 1))])))) = FIX(0.50000) * i    + CBCR_OFFSET + ONE_HALF-1;
/*  B=>Cb and R=>Cr tables are the same
    rgb_ycc_tab[i+R_CR_OFF] = FIX(0.50000) * i    + CBCR_OFFSET + ONE_HALF-1;
*/
    (*(INT32 *)(__boundcheck_ptr_reference(111,5,"rgb_ycc_start",(void *)(&rgb_ycc_tab[0]),(void *)(&rgb_ycc_tab[i + (6 * (255 + 1))])))) = (-FIX(0.41869)) * i;
    (*(INT32 *)(__boundcheck_ptr_reference(112,5,"rgb_ycc_start",(void *)(&rgb_ycc_tab[0]),(void *)(&rgb_ycc_tab[i + (7 * (255 + 1))])))) = (-FIX(0.08131)) * i;
  }
}


/*
 * Convert some rows of samples to the JPEG colorspace.
 *
 * Note that we change from the application's interleaved-pixel format
 * to our internal noninterleaved, one-plane-per-component format.
 * The input buffer is therefore three times as wide as the output buffer.
 *
 * A starting row offset is provided only for the output buffer.  The caller
 * can easily adjust the passed input_buf value to accommodate any row
 * offset required on that side.
 */

METHODDEF(void)
rgb_ycc_convert (j_compress_ptr cinfo,
		 JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
		 JDIMENSION output_row, int num_rows)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));

  register int r, g, b;
  register INT32 * ctab = cconvert->rgb_ycc_tab;
  register JSAMPROW inptr;
  register JSAMPROW outptr0, outptr1, outptr2;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->image_width;
__boundcheck_metadata_store((void *)(&num_cols),(void *)((size_t)(&num_cols)+sizeof(num_cols)*8-1));


  while (--num_rows >= 0) {
    inptr = *(JSAMPARRAY)(__boundcheck_ptr_reference(143,23,"rgb_ycc_convert",(void *)(input_buf),(void *)(input_buf++)));
    outptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(144,15,"rgb_ycc_convert",(void *)(&output_buf[0][0]),(void *)(&output_buf[0][output_row]))));
    outptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(145,15,"rgb_ycc_convert",(void *)(&output_buf[1][0]),(void *)(&output_buf[1][output_row]))));
    outptr2 = (*(JSAMPROW *)(__boundcheck_ptr_reference(146,15,"rgb_ycc_convert",(void *)(&output_buf[2][0]),(void *)(&output_buf[2][output_row]))));
    output_row++;
    for (col = 0; col < num_cols; col++) {
      r = GETJSAMPLE(inptr[RGB_RED]);
      g = GETJSAMPLE(inptr[RGB_GREEN]);
      b = GETJSAMPLE(inptr[RGB_BLUE]);
      inptr += RGB_PIXELSIZE;
      /* If the inputs are 0..MAXJSAMPLE, the outputs of these equations
       * must be too; we do not need an explicit range-limiting operation.
       * Hence the value being shifted is never negative, and we don't
       * need the general RIGHT_SHIFT macro.
       */
      /* Y */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(159,7,"rgb_ycc_convert",(void *)(&outptr0[0]),(void *)(&outptr0[col])))) = (JSAMPLE)
		(((*(INT32 *)(__boundcheck_ptr_reference(160,5,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[r + 0])))) + (*(INT32 *)(__boundcheck_ptr_reference(160,23,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[g + (1 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(160,41,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[b + (2 * (255 + 1))])))))
		 >> SCALEBITS);
      /* Cb */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(163,7,"rgb_ycc_convert",(void *)(&outptr1[0]),(void *)(&outptr1[col])))) = (JSAMPLE)
		(((*(INT32 *)(__boundcheck_ptr_reference(164,5,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[r + (3 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(164,24,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[g + (4 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(164,43,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[b + (5 * (255 + 1))])))))
		 >> SCALEBITS);
      /* Cr */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(167,7,"rgb_ycc_convert",(void *)(&outptr2[0]),(void *)(&outptr2[col])))) = (JSAMPLE)
		(((*(INT32 *)(__boundcheck_ptr_reference(168,5,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[r + (5 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(168,24,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[g + (6 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(168,43,"rgb_ycc_convert",(void *)(&ctab[0]),(void *)(&ctab[b + (7 * (255 + 1))])))))
		 >> SCALEBITS);
    }
  }
}


/**************** Cases other than RGB -> YCbCr **************/


/*
 * Convert some rows of samples to the JPEG colorspace.
 * This version handles RGB->grayscale conversion, which is the same
 * as the RGB->Y portion of RGB->YCbCr.
 * We assume rgb_ycc_start has been called (we only use the Y tables).
 */

METHODDEF(void)
rgb_gray_convert (j_compress_ptr cinfo,
		  JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
		  JDIMENSION output_row, int num_rows)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));

  register int r, g, b;
  register INT32 * ctab = cconvert->rgb_ycc_tab;
  register JSAMPROW inptr;
  register JSAMPROW outptr;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->image_width;
__boundcheck_metadata_store((void *)(&num_cols),(void *)((size_t)(&num_cols)+sizeof(num_cols)*8-1));


  while (--num_rows >= 0) {
    inptr = *(JSAMPARRAY)(__boundcheck_ptr_reference(199,23,"rgb_gray_convert",(void *)(input_buf),(void *)(input_buf++)));
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(200,14,"rgb_gray_convert",(void *)(&output_buf[0][0]),(void *)(&output_buf[0][output_row]))));
    output_row++;
    for (col = 0; col < num_cols; col++) {
      r = GETJSAMPLE(inptr[RGB_RED]);
      g = GETJSAMPLE(inptr[RGB_GREEN]);
      b = GETJSAMPLE(inptr[RGB_BLUE]);
      inptr += RGB_PIXELSIZE;
      /* Y */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(208,7,"rgb_gray_convert",(void *)(&outptr[0]),(void *)(&outptr[col])))) = (JSAMPLE)
		(((*(INT32 *)(__boundcheck_ptr_reference(209,5,"rgb_gray_convert",(void *)(&ctab[0]),(void *)(&ctab[r + 0])))) + (*(INT32 *)(__boundcheck_ptr_reference(209,23,"rgb_gray_convert",(void *)(&ctab[0]),(void *)(&ctab[g + (1 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(209,41,"rgb_gray_convert",(void *)(&ctab[0]),(void *)(&ctab[b + (2 * (255 + 1))])))))
		 >> SCALEBITS);
    }
  }
}


/*
 * Convert some rows of samples to the JPEG colorspace.
 * This version handles Adobe-style CMYK->YCCK conversion,
 * where we convert R=1-C, G=1-M, and B=1-Y to YCbCr using the same
 * conversion as above, while passing K (black) unchanged.
 * We assume rgb_ycc_start has been called.
 */

METHODDEF(void)
cmyk_ycck_convert (j_compress_ptr cinfo,
		   JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
		   JDIMENSION output_row, int num_rows)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));

  register int r, g, b;
  register INT32 * ctab = cconvert->rgb_ycc_tab;
  register JSAMPROW inptr;
  register JSAMPROW outptr0, outptr1, outptr2, outptr3;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->image_width;
__boundcheck_metadata_store((void *)(&num_cols),(void *)((size_t)(&num_cols)+sizeof(num_cols)*8-1));


  while (--num_rows >= 0) {
    inptr = *(JSAMPARRAY)(__boundcheck_ptr_reference(238,23,"cmyk_ycck_convert",(void *)(input_buf),(void *)(input_buf++)));
    outptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(239,15,"cmyk_ycck_convert",(void *)(&output_buf[0][0]),(void *)(&output_buf[0][output_row]))));
    outptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(240,15,"cmyk_ycck_convert",(void *)(&output_buf[1][0]),(void *)(&output_buf[1][output_row]))));
    outptr2 = (*(JSAMPROW *)(__boundcheck_ptr_reference(241,15,"cmyk_ycck_convert",(void *)(&output_buf[2][0]),(void *)(&output_buf[2][output_row]))));
    outptr3 = (*(JSAMPROW *)(__boundcheck_ptr_reference(242,15,"cmyk_ycck_convert",(void *)(&output_buf[3][0]),(void *)(&output_buf[3][output_row]))));
    output_row++;
    for (col = 0; col < num_cols; col++) {
      r = MAXJSAMPLE - GETJSAMPLE(inptr[0]);
      g = MAXJSAMPLE - GETJSAMPLE(inptr[1]);
      b = MAXJSAMPLE - GETJSAMPLE(inptr[2]);
      /* K passes through as-is */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(249,7,"cmyk_ycck_convert",(void *)(&outptr3[0]),(void *)(&outptr3[col])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(249,22,"cmyk_ycck_convert",(void *)(&inptr[0]),(void *)(&inptr[3]))));	/* don't need GETJSAMPLE here */
      inptr += 4;
      /* If the inputs are 0..MAXJSAMPLE, the outputs of these equations
       * must be too; we do not need an explicit range-limiting operation.
       * Hence the value being shifted is never negative, and we don't
       * need the general RIGHT_SHIFT macro.
       */
      /* Y */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(257,7,"cmyk_ycck_convert",(void *)(&outptr0[0]),(void *)(&outptr0[col])))) = (JSAMPLE)
		(((*(INT32 *)(__boundcheck_ptr_reference(258,5,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[r + 0])))) + (*(INT32 *)(__boundcheck_ptr_reference(258,23,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[g + (1 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(258,41,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[b + (2 * (255 + 1))])))))
		 >> SCALEBITS);
      /* Cb */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(261,7,"cmyk_ycck_convert",(void *)(&outptr1[0]),(void *)(&outptr1[col])))) = (JSAMPLE)
		(((*(INT32 *)(__boundcheck_ptr_reference(262,5,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[r + (3 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(262,24,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[g + (4 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(262,43,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[b + (5 * (255 + 1))])))))
		 >> SCALEBITS);
      /* Cr */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(265,7,"cmyk_ycck_convert",(void *)(&outptr2[0]),(void *)(&outptr2[col])))) = (JSAMPLE)
		(((*(INT32 *)(__boundcheck_ptr_reference(266,5,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[r + (5 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(266,24,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[g + (6 * (255 + 1))])))) + (*(INT32 *)(__boundcheck_ptr_reference(266,43,"cmyk_ycck_convert",(void *)(&ctab[0]),(void *)(&ctab[b + (7 * (255 + 1))])))))
		 >> SCALEBITS);
    }
  }
}


/*
 * Convert some rows of samples to the JPEG colorspace.
 * This version handles grayscale output with no conversion.
 * The source can be either plain grayscale or YCbCr (since Y == gray).
 */

METHODDEF(void)
grayscale_convert (j_compress_ptr cinfo,
		   JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
		   JDIMENSION output_row, int num_rows)
{
  register JSAMPROW inptr;
  register JSAMPROW outptr;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->image_width;
__boundcheck_metadata_store((void *)(&num_cols),(void *)((size_t)(&num_cols)+sizeof(num_cols)*8-1));

  int instride = cinfo->input_components;
__boundcheck_metadata_store((void *)(&instride),(void *)((size_t)(&instride)+sizeof(instride)*8-1));


  while (--num_rows >= 0) {
    inptr = *(JSAMPARRAY)(__boundcheck_ptr_reference(291,23,"grayscale_convert",(void *)(input_buf),(void *)(input_buf++)));
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(292,14,"grayscale_convert",(void *)(&output_buf[0][0]),(void *)(&output_buf[0][output_row]))));
    output_row++;
    for (col = 0; col < num_cols; col++) {
      (*(JSAMPLE *)(__boundcheck_ptr_reference(295,7,"grayscale_convert",(void *)(&outptr[0]),(void *)(&outptr[col])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(295,21,"grayscale_convert",(void *)(&inptr[0]),(void *)(&inptr[0]))));	/* don't need GETJSAMPLE() here */
      inptr += instride;
    }
  }
}


/*
 * Convert some rows of samples to the JPEG colorspace.
 * This version handles multi-component colorspaces without conversion.
 * We assume input_components == num_components.
 */

METHODDEF(void)
null_convert (j_compress_ptr cinfo,
	      JSAMPARRAY input_buf, JSAMPIMAGE output_buf,
	      JDIMENSION output_row, int num_rows)
{
  register JSAMPROW inptr;
  register JSAMPROW outptr;
  register JDIMENSION col;
  register int ci;
  int nc = cinfo->num_components;
__boundcheck_metadata_store((void *)(&nc),(void *)((size_t)(&nc)+sizeof(nc)*8-1));

  JDIMENSION num_cols = cinfo->image_width;
__boundcheck_metadata_store((void *)(&num_cols),(void *)((size_t)(&num_cols)+sizeof(num_cols)*8-1));


  while (--num_rows >= 0) {
    /* It seems fastest to make a separate pass for each component. */
    for (ci = 0; ci < nc; ci++) {
      inptr = *(JSAMPARRAY)(__boundcheck_ptr_reference(323,16,"null_convert",(void *)(input_buf),(void *)(input_buf)));
      outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(324,16,"null_convert",(void *)(&output_buf[ci][0]),(void *)(&output_buf[ci][output_row]))));
      for (col = 0; col < num_cols; col++) {
	(*(JSAMPLE *)(__boundcheck_ptr_reference(326,2,"null_convert",(void *)(&outptr[0]),(void *)(&outptr[col])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(326,16,"null_convert",(void *)(&inptr[0]),(void *)(&inptr[ci])))); /* don't need GETJSAMPLE() here */
	inptr += nc;
      }
    }
    input_buf++;
    output_row++;
  }
}


/*
 * Empty method for start_pass.
 */

METHODDEF(void)
null_method (j_compress_ptr cinfo)
{
  /* no work needed */
}


/*
 * Module initialization routine for input colorspace conversion.
 */

GLOBAL(void)
jinit_color_converter (j_compress_ptr cinfo)
{
  my_cconvert_ptr cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));


  cconvert = (my_cconvert_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(357,19,"jinit_color_converter",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_color_converter));
  cinfo->cconvert = (struct jpeg_color_converter *) cconvert;
  /* set start_pass to null method until we find out differently */
  cconvert->pub.start_pass = null_method;

  /* Make sure input_components agrees with in_color_space */
  switch (cinfo->in_color_space) {
  case JCS_GRAYSCALE:
    if (cinfo->input_components != 1)
      ERREXIT(cinfo, JERR_BAD_IN_COLORSPACE);
    break;

  case JCS_RGB:
#if RGB_PIXELSIZE != 3
    if (cinfo->input_components != RGB_PIXELSIZE)
      ERREXIT(cinfo, JERR_BAD_IN_COLORSPACE);
    break;
#endif /* else share code with YCbCr */

  case JCS_YCbCr:
    if (cinfo->input_components != 3)
      ERREXIT(cinfo, JERR_BAD_IN_COLORSPACE);
    break;

  case JCS_CMYK:
  case JCS_YCCK:
    if (cinfo->input_components != 4)
      ERREXIT(cinfo, JERR_BAD_IN_COLORSPACE);
    break;

  default:			/* JCS_UNKNOWN can be anything */
    if (cinfo->input_components < 1)
      ERREXIT(cinfo, JERR_BAD_IN_COLORSPACE);
    break;
  }

  /* Check num_components, set conversion method based on requested space */
  switch (cinfo->jpeg_color_space) {
  case JCS_GRAYSCALE:
    if (cinfo->num_components != 1)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    if (cinfo->in_color_space == JCS_GRAYSCALE)
      cconvert->pub.color_convert = grayscale_convert;
    else if (cinfo->in_color_space == JCS_RGB) {
      cconvert->pub.start_pass = rgb_ycc_start;
      cconvert->pub.color_convert = rgb_gray_convert;
    } else if (cinfo->in_color_space == JCS_YCbCr)
      cconvert->pub.color_convert = grayscale_convert;
    else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  case JCS_RGB:
    if (cinfo->num_components != 3)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    if (cinfo->in_color_space == JCS_RGB && RGB_PIXELSIZE == 3)
      cconvert->pub.color_convert = null_convert;
    else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  case JCS_YCbCr:
    if (cinfo->num_components != 3)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    if (cinfo->in_color_space == JCS_RGB) {
      cconvert->pub.start_pass = rgb_ycc_start;
      cconvert->pub.color_convert = rgb_ycc_convert;
    } else if (cinfo->in_color_space == JCS_YCbCr)
      cconvert->pub.color_convert = null_convert;
    else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  case JCS_CMYK:
    if (cinfo->num_components != 4)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    if (cinfo->in_color_space == JCS_CMYK)
      cconvert->pub.color_convert = null_convert;
    else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  case JCS_YCCK:
    if (cinfo->num_components != 4)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    if (cinfo->in_color_space == JCS_CMYK) {
      cconvert->pub.start_pass = rgb_ycc_start;
      cconvert->pub.color_convert = cmyk_ycck_convert;
    } else if (cinfo->in_color_space == JCS_YCCK)
      cconvert->pub.color_convert = null_convert;
    else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  default:			/* allow null conversion of JCS_UNKNOWN */
    if (cinfo->jpeg_color_space != cinfo->in_color_space ||
	cinfo->num_components != cinfo->input_components)
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    cconvert->pub.color_convert = null_convert;
    break;
  }
}
