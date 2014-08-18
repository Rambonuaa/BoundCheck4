/*
 * jdcolor.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains output colorspace conversion routines.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/* Private subobject */

typedef struct {
  struct jpeg_color_deconverter pub; /* public fields */

  /* Private state for YCC->RGB conversion */
  int * Cr_r_tab;		/* => table for Cr to R conversion */
  int * Cb_b_tab;		/* => table for Cb to B conversion */
  INT32 * Cr_g_tab;		/* => table for Cr to G conversion */
  INT32 * Cb_g_tab;		/* => table for Cb to G conversion */
} my_color_deconverter;

typedef my_color_deconverter * my_cconvert_ptr;


/**************** YCbCr -> RGB conversion: most common case **************/

/*
 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
 * The conversion equations to be implemented are therefore
 *	R = Y                + 1.40200 * Cr
 *	G = Y - 0.34414 * Cb - 0.71414 * Cr
 *	B = Y + 1.77200 * Cb
 * where Cb and Cr represent the incoming values less CENTERJSAMPLE.
 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
 *
 * To avoid floating-point arithmetic, we represent the fractional constants
 * as integers scaled up by 2^16 (about 4 digits precision); we have to divide
 * the products by 2^16, with appropriate rounding, to get the correct answer.
 * Notice that Y, being an integral input, does not contribute any fraction
 * so it need not participate in the rounding.
 *
 * For even more speed, we avoid doing any multiplications in the inner loop
 * by precalculating the constants times Cb and Cr for all possible values.
 * For 8-bit JSAMPLEs this is very reasonable (only 256 entries per table);
 * for 12-bit samples it is still acceptable.  It's not very reasonable for
 * 16-bit samples, but if you want lossless storage you shouldn't be changing
 * colorspace anyway.
 * The Cr=>R and Cb=>B values can be rounded to integers in advance; the
 * values for the G calculation are left scaled up, since we must add them
 * together before rounding.
 */

#define SCALEBITS	16	/* speediest right-shift on some machines */
#define ONE_HALF	((INT32) 1 << (SCALEBITS-1))
#define FIX(x)		((INT32) ((x) * (1L<<SCALEBITS) + 0.5))


/*
 * Initialize tables for YCC->RGB colorspace conversion.
 */

LOCAL(void)
build_ycc_rgb_table (j_decompress_ptr cinfo)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  INT32 x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));

  SHIFT_TEMPS

  cconvert->Cr_r_tab = (int *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(78,19,"build_ycc_rgb_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(MAXJSAMPLE+1) * SIZEOF(int));
  cconvert->Cb_b_tab = (int *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(81,19,"build_ycc_rgb_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(MAXJSAMPLE+1) * SIZEOF(int));
  cconvert->Cr_g_tab = (INT32 *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(84,19,"build_ycc_rgb_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(MAXJSAMPLE+1) * SIZEOF(INT32));
  cconvert->Cb_g_tab = (INT32 *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(87,19,"build_ycc_rgb_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(MAXJSAMPLE+1) * SIZEOF(INT32));

  for (i = 0, x = -CENTERJSAMPLE; i <= MAXJSAMPLE; i++, x++) {
    /* i is the actual input pixel value, in the range 0..MAXJSAMPLE */
    /* The Cb or Cr value we are thinking of is x = i - CENTERJSAMPLE */
    /* Cr=>R value is nearest int to 1.40200 * x */
    cconvert->Cr_r_tab[i] = (int)
		    RIGHT_SHIFT(FIX(1.40200) * x + ONE_HALF, SCALEBITS);
    /* Cb=>B value is nearest int to 1.77200 * x */
    cconvert->Cb_b_tab[i] = (int)
		    RIGHT_SHIFT(FIX(1.77200) * x + ONE_HALF, SCALEBITS);
    /* Cr=>G value is scaled-up -0.71414 * x */
    cconvert->Cr_g_tab[i] = (- FIX(0.71414)) * x;
    /* Cb=>G value is scaled-up -0.34414 * x */
    /* We also add in ONE_HALF so that need not do it in inner loop */
    cconvert->Cb_g_tab[i] = (- FIX(0.34414)) * x + ONE_HALF;
  }
}


/*
 * Convert some rows of samples to the output colorspace.
 *
 * Note that we change from noninterleaved, one-plane-per-component format
 * to interleaved-pixel format.  The output buffer is therefore three times
 * as wide as the input buffer.
 * A starting row offset is provided only for the input buffer.  The caller
 * can easily adjust the passed output_buf value to accommodate any row
 * offset required on that side.
 */

METHODDEF(void)
ycc_rgb_convert (j_decompress_ptr cinfo,
		 JSAMPIMAGE input_buf, JDIMENSION input_row,
		 JSAMPARRAY output_buf, int num_rows)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));

  register int y, cb, cr;
  register JSAMPROW outptr;
  register JSAMPROW inptr0, inptr1, inptr2;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->output_width;
__boundcheck_metadata_store((void *)(&num_cols),(void *)((size_t)(&num_cols)+sizeof(num_cols)*8-1));

  /* copy these pointers into registers if possible */
  register JSAMPLE * range_limit = cinfo->sample_range_limit;
  register int * Crrtab = cconvert->Cr_r_tab;
  register int * Cbbtab = cconvert->Cb_b_tab;
  register INT32 * Crgtab = cconvert->Cr_g_tab;
  register INT32 * Cbgtab = cconvert->Cb_g_tab;
  SHIFT_TEMPS

  while (--num_rows >= 0) {
    inptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(139,14,"ycc_rgb_convert",(void *)(&input_buf[0][0]),(void *)(&input_buf[0][input_row]))));
    inptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(140,14,"ycc_rgb_convert",(void *)(&input_buf[1][0]),(void *)(&input_buf[1][input_row]))));
    inptr2 = (*(JSAMPROW *)(__boundcheck_ptr_reference(141,14,"ycc_rgb_convert",(void *)(&input_buf[2][0]),(void *)(&input_buf[2][input_row]))));
    input_row++;
    outptr = *(JSAMPARRAY)(__boundcheck_ptr_reference(143,25,"ycc_rgb_convert",(void *)(output_buf),(void *)(output_buf++)));
    for (col = 0; col < num_cols; col++) {
      y  = GETJSAMPLE(inptr0[col]);
      cb = GETJSAMPLE(inptr1[col]);
      cr = GETJSAMPLE(inptr2[col]);
      /* Range-limiting is essential due to noise introduced by DCT losses. */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(149,7,"ycc_rgb_convert",(void *)(&outptr[0]),(void *)(&outptr[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(149,27,"ycc_rgb_convert",(void *)(&range_limit[0]),(void *)(&range_limit[y + Crrt(*(int *)(__boundcheck_ptr_reference(149,43,"ycc_rgb_convert",(void *)(&Crrtab[0]),(void *)(&Crrtab[cr])))));
      (*(JSAMPLE *)(__boundcheck_ptr_reference(150,7,"ycc_rgb_convert",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(150,27,"ycc_rgb_convert",(void *)(&range_limit[0]),(void *)(&range_limit[y + ((int)((Cbgtab[cb] + Crgtab[cr]) >> (16)))]))));
      (*(JSAMPLE *)(__boundcheck_ptr_reference(153,7,"ycc_rgb_convert",(void *)(&outptr[0]),(void *)(&outptr[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(153,27,"ycc_rgb_convert",(void *)(&range_limit[0]),(void *)(&range_limit[y + Cbbt(*(int *)(__boundcheck_ptr_reference(153,43,"ycc_rgb_convert",(void *)(&Cbbtab[0]),(void *)(&Cbbtab[cb])))));
      outptr += RGB_PIXELSIZE;
    }
  }
}


/**************** Cases other than YCbCr -> RGB **************/


/*
 * Color conversion for no colorspace change: just copy the data,
 * converting from separate-planes to interleaved representation.
 */

METHODDEF(void)
null_convert (j_decompress_ptr cinfo,
	      JSAMPIMAGE input_buf, JDIMENSION input_row,
	      JSAMPARRAY output_buf, int num_rows)
{
  register JSAMPROW inptr, outptr;
  register JDIMENSION count;
  register int num_components = cinfo->num_components;
  JDIMENSION num_cols = cinfo->output_width;
__boundcheck_metadata_store((void *)(&num_cols),(void *)((size_t)(&num_cols)+sizeof(num_cols)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));


  while (--num_rows >= 0) {
    for (ci = 0; ci < num_components; ci++) {
      inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(181,15,"null_convert",(void *)(&input_buf[ci][0]),(void *)(&input_buf[ci][input_row]))));
      outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(182,16,"null_convert",(void *)(&output_buf[0]),(void *)(&output_buf[0])))) + ci;
      for (count = num_cols; count > 0; count--) {
	*(JSAMPROW)(__boundcheck_ptr_reference(184,3,"null_convert",(void *)(outptr),(void *)(outptr))) = *(JSAMPROW)(__boundcheck_ptr_reference(184,18,"null_convert",(void *)(inptr),(void *)(inptr++)));	/* needn't bother with GETJSAMPLE() here */
	outptr += num_components;
      }
    }
    input_row++;
    output_buf++;
  }
}


/*
 * Color conversion for grayscale: just copy the data.
 * This also works for YCbCr -> grayscale conversion, in which
 * we just copy the Y (luminance) component and ignore chrominance.
 */

METHODDEF(void)
grayscale_convert (j_decompress_ptr cinfo,
		   JSAMPIMAGE input_buf, JDIMENSION input_row,
		   JSAMPARRAY output_buf, int num_rows)
{
  jcopy_sample_rows((*(JSAMPARRAY *)(__boundcheck_ptr_reference(205,21,"grayscale_convert",(void *)(&input_buf[0]),(void *)(&input_buf[0])))), (int) input_row, output_buf, 0,
		    num_rows, cinfo->output_width);
}


/*
 * Adobe-style YCCK->CMYK conversion.
 * We convert YCbCr to R=1-C, G=1-M, and B=1-Y using the same
 * conversion as above, while passing K (black) unchanged.
 * We assume build_ycc_rgb_table has been called.
 */

METHODDEF(void)
ycck_cmyk_convert (j_decompress_ptr cinfo,
		   JSAMPIMAGE input_buf, JDIMENSION input_row,
		   JSAMPARRAY output_buf, int num_rows)
{
  my_cconvert_ptr cconvert = (my_cconvert_ptr) cinfo->cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));

  register int y, cb, cr;
  register JSAMPROW outptr;
  register JSAMPROW inptr0, inptr1, inptr2, inptr3;
  register JDIMENSION col;
  JDIMENSION num_cols = cinfo->output_width;
__boundcheck_metadata_store((void *)(&num_cols),(void *)((size_t)(&num_cols)+sizeof(num_cols)*8-1));

  /* copy these pointers into registers if possible */
  register JSAMPLE * range_limit = cinfo->sample_range_limit;
  register int * Crrtab = cconvert->Cr_r_tab;
  register int * Cbbtab = cconvert->Cb_b_tab;
  register INT32 * Crgtab = cconvert->Cr_g_tab;
  register INT32 * Cbgtab = cconvert->Cb_g_tab;
  SHIFT_TEMPS

  while (--num_rows >= 0) {
    inptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(237,14,"ycck_cmyk_convert",(void *)(&input_buf[0][0]),(void *)(&input_buf[0][input_row]))));
    inptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(238,14,"ycck_cmyk_convert",(void *)(&input_buf[1][0]),(void *)(&input_buf[1][input_row]))));
    inptr2 = (*(JSAMPROW *)(__boundcheck_ptr_reference(239,14,"ycck_cmyk_convert",(void *)(&input_buf[2][0]),(void *)(&input_buf[2][input_row]))));
    inptr3 = (*(JSAMPROW *)(__boundcheck_ptr_reference(240,14,"ycck_cmyk_convert",(void *)(&input_buf[3][0]),(void *)(&input_buf[3][input_row]))));
    input_row++;
    outptr = *(JSAMPARRAY)(__boundcheck_ptr_reference(242,25,"ycck_cmyk_convert",(void *)(output_buf),(void *)(output_buf++)));
    for (col = 0; col < num_cols; col++) {
      y  = GETJSAMPLE(inptr0[col]);
      cb = GETJSAMPLE(inptr1[col]);
      cr = GETJSAMPLE(inptr2[col]);
      /* Range-limiting is essential due to noise introduced by DCT losses. */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(248,7,"ycck_cmyk_convert",(void *)(&outptr[0]),(void *)(&outptr[0])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(248,19,"ycck_cmyk_convert",(void *)(&range_limit[0]),(void *)(&range_limit[255 - (y + Crrt(*(int *)(__boundcheck_ptr_reference(248,49,"ycck_cmyk_convert",(void *)(&Crrtab[0]),(void *)(&Crrtab[cr]))))));	/* red */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(249,7,"ycck_cmyk_convert",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(249,19,"ycck_cmyk_convert",(void *)(&range_limit[0]),(void *)(&range_limit[255 - (y + ((int)((Cbgtab[cb] + Crgtab[cr]) >> (16))))]))));
      (*(JSAMPLE *)(__boundcheck_ptr_reference(252,7,"ycck_cmyk_convert",(void *)(&outptr[0]),(void *)(&outptr[2])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(252,19,"ycck_cmyk_convert",(void *)(&range_limit[0]),(void *)(&range_limit[255 - (y + Cbbt(*(int *)(__boundcheck_ptr_reference(252,49,"ycck_cmyk_convert",(void *)(&Cbbtab[0]),(void *)(&Cbbtab[cb]))))));	/* blue */
      /* K passes through unchanged */
      (*(JSAMPLE *)(__boundcheck_ptr_reference(254,7,"ycck_cmyk_convert",(void *)(&outptr[0]),(void *)(&outptr[3])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(254,19,"ycck_cmyk_convert",(void *)(&inptr3[0]),(void *)(&inptr3[col]))));	/* don't need GETJSAMPLE here */
      outptr += 4;
    }
  }
}


/*
 * Empty method for start_pass.
 */

METHODDEF(void)
start_pass_dcolor (j_decompress_ptr cinfo)
{
  /* no work needed */
}


/*
 * Module initialization routine for output colorspace conversion.
 */

GLOBAL(void)
jinit_color_deconverter (j_decompress_ptr cinfo)
{
  my_cconvert_ptr cconvert;
__boundcheck_metadata_store((void *)(&cconvert),(void *)((size_t)(&cconvert)+sizeof(cconvert)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));


  cconvert = (my_cconvert_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(283,19,"jinit_color_deconverter",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_color_deconverter));
  cinfo->cconvert = (struct jpeg_color_deconverter *) cconvert;
  cconvert->pub.start_pass = start_pass_dcolor;

  /* Make sure num_components agrees with jpeg_color_space */
  switch (cinfo->jpeg_color_space) {
  case JCS_GRAYSCALE:
    if (cinfo->num_components != 1)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    break;

  case JCS_RGB:
  case JCS_YCbCr:
    if (cinfo->num_components != 3)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    break;

  case JCS_CMYK:
  case JCS_YCCK:
    if (cinfo->num_components != 4)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    break;

  default:			/* JCS_UNKNOWN can be anything */
    if (cinfo->num_components < 1)
      ERREXIT(cinfo, JERR_BAD_J_COLORSPACE);
    break;
  }

  /* Set out_color_components and conversion method based on requested space.
   * Also clear the component_needed flags for any unused components,
   * so that earlier pipeline stages can avoid useless computation.
   */

  switch (cinfo->out_color_space) {
  case JCS_GRAYSCALE:
    cinfo->out_color_components = 1;
    if (cinfo->jpeg_color_space == JCS_GRAYSCALE ||
	cinfo->jpeg_color_space == JCS_YCbCr) {
      cconvert->pub.color_convert = grayscale_convert;
      /* For color->grayscale conversion, only the Y (0) component is needed */
      for (ci = 1; ci < cinfo->num_components; ci++)
	cinfo->comp_info[ci].component_needed = FALSE;
    } else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  case JCS_RGB:
    cinfo->out_color_components = RGB_PIXELSIZE;
    if (cinfo->jpeg_color_space == JCS_YCbCr) {
      cconvert->pub.color_convert = ycc_rgb_convert;
      build_ycc_rgb_table(cinfo);
    } else if (cinfo->jpeg_color_space == JCS_RGB && RGB_PIXELSIZE == 3) {
      cconvert->pub.color_convert = null_convert;
    } else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  case JCS_CMYK:
    cinfo->out_color_components = 4;
    if (cinfo->jpeg_color_space == JCS_YCCK) {
      cconvert->pub.color_convert = ycck_cmyk_convert;
      build_ycc_rgb_table(cinfo);
    } else if (cinfo->jpeg_color_space == JCS_CMYK) {
      cconvert->pub.color_convert = null_convert;
    } else
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;

  default:
    /* Permit null conversion to same output space */
    if (cinfo->out_color_space == cinfo->jpeg_color_space) {
      cinfo->out_color_components = cinfo->num_components;
      cconvert->pub.color_convert = null_convert;
    } else			/* unsupported non-null conversion */
      ERREXIT(cinfo, JERR_CONVERSION_NOTIMPL);
    break;
  }

  if (cinfo->quantize_colors)
    cinfo->output_components = 1; /* single colormapped output component */
  else
    cinfo->output_components = cinfo->out_color_components;
}
