/*
 * jdsample.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains upsampling routines.
 *
 * Upsampling input data is counted in "row groups".  A row group
 * is defined to be (v_samp_factor * DCT_scaled_size / min_DCT_scaled_size)
 * sample rows of each component.  Upsampling will normally produce
 * max_v_samp_factor pixel rows from each row group (but this could vary
 * if the upsampler is applying a scale factor of its own).
 *
 * An excellent reference for image resampling is
 *   Digital Image Warping, George Wolberg, 1990.
 *   Pub. by IEEE Computer Society Press, Los Alamitos, CA. ISBN 0-8186-8944-7.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/* Pointer to routine to upsample a single component */
typedef JMETHOD(void, upsample1_ptr,
		(j_decompress_ptr cinfo, jpeg_component_info * compptr,
		 JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr));

/* Private subobject */

typedef struct {
  struct jpeg_upsampler pub;	/* public fields */

  /* Color conversion buffer.  When using separate upsampling and color
   * conversion steps, this buffer holds one upsampled row group until it
   * has been color converted and output.
   * Note: we do not allocate any storage for component(s) which are full-size,
   * ie do not need rescaling.  The corresponding entry of color_buf[] is
   * simply set to point to the input data array, thereby avoiding copying.
   */
  JSAMPARRAY color_buf[MAX_COMPONENTS];

  /* Per-component upsampling method pointers */
  upsample1_ptr methods[MAX_COMPONENTS];

  int next_row_out;		/* counts rows emitted from color_buf */
  JDIMENSION rows_to_go;	/* counts rows remaining in image */

  /* Height of an input row group for each component. */
  int rowgroup_height[MAX_COMPONENTS];

  /* These arrays save pixel expansion factors so that int_expand need not
   * recompute them each time.  They are unused for other upsampling methods.
   */
  UINT8 h_expand[MAX_COMPONENTS];
  UINT8 v_expand[MAX_COMPONENTS];
} my_upsampler;

typedef my_upsampler * my_upsample_ptr;


/*
 * Initialize for an upsampling pass.
 */

METHODDEF(void)
start_pass_upsample (j_decompress_ptr cinfo)
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));


  /* Mark the conversion buffer empty */
  upsample->next_row_out = cinfo->max_v_samp_factor;
  /* Initialize total-height counter for detecting bottom of image */
  upsample->rows_to_go = cinfo->output_height;
}


/*
 * Control routine to do upsampling (and color conversion).
 *
 * In this version we upsample each component independently.
 * We upsample one row group into the conversion buffer, then apply
 * color conversion a row at a time.
 */

METHODDEF(void)
sep_upsample (j_decompress_ptr cinfo,
	      JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
	      JDIMENSION in_row_groups_avail,
	      JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
	      JDIMENSION out_rows_avail)
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  JDIMENSION num_rows;
__boundcheck_metadata_store((void *)(&num_rows),(void *)((size_t)(&num_rows)+sizeof(num_rows)*8-1));


  /* Fill the conversion buffer, if it's empty */
  if (upsample->next_row_out >= cinfo->max_v_samp_factor) {
    for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
	 ci++, compptr++) {
      /* Invoke per-component upsample method.  Notice we pass a POINTER
       * to color_buf[ci], so that fullsize_upsample can change it.
       */
      (*(upsample1_ptr)(__boundcheck_ptr_reference(107,29,"sep_upsample",(void *)(upsample->methods[ci]),(void *)upsample->methods[_RV_insert_check(0,10,107,9,"sep_upsample",ci)]))) (cinfo, compptr,
	(*(JSAMPARRAY *)(__boundcheck_ptr_reference(108,2,"sep_upsample",(void *)(&input_buf[0]),(void *)(&input_buf[ci])))) + (*(JDIMENSION *)(__boundcheck_ptr_reference(108,20,"sep_upsample",(void *)(in_row_group_ctr),(void *)(in_row_group_ctr))) * upsample->rowgroup_height[_RV_insert_check(0,10,108,39,"sep_upsample",ci)]),
	upsample->color_buf + ci);
    }
    upsample->next_row_out = 0;
  }

  /* Color-convert and emit rows */

  /* How many we have in the buffer: */
  num_rows = (JDIMENSION) (cinfo->max_v_samp_factor - upsample->next_row_out);
  /* Not more than the distance to the end of the image.  Need this test
   * in case the image height is not a multiple of max_v_samp_factor:
   */
  if (num_rows > upsample->rows_to_go) 
    num_rows = upsample->rows_to_go;
  /* And not more than what the client can accept: */
  out_rows_avail -= *(JDIMENSION *)(__boundcheck_ptr_reference(124,22,"sep_upsample",(void *)(out_row_ctr),(void *)(out_row_ctr)));
  if (num_rows > out_rows_avail)
    num_rows = out_rows_avail;

  (*(void (*)(j_decompress_ptr, JSAMPIMAGE, JDIMENSION, JSAMPARRAY, int))(__boundcheck_ptr_reference(128,22,"sep_upsample",(void *)(cinfo->cconvert->color_convert),(void *)cinfo->cconvert->color_convert))) (cinfo, upsample->color_buf,
				     (JDIMENSION) upsample->next_row_out,
				     output_buf + *(JDIMENSION *)(__boundcheck_ptr_reference(130,24,"sep_upsample",(void *)(out_row_ctr),(void *)(out_row_ctr))),
				     (int) num_rows);

  /* Adjust counts */
  *(JDIMENSION *)(__boundcheck_ptr_reference(134,4,"sep_upsample",(void *)(out_row_ctr),(void *)(out_row_ctr))) += num_rows;
  upsample->rows_to_go -= num_rows;
  upsample->next_row_out += num_rows;
  /* When the buffer is emptied, declare this input row group consumed */
  if (upsample->next_row_out >= cinfo->max_v_samp_factor)
    (*(JDIMENSION *)(__boundcheck_ptr_reference(139,7,"sep_upsample",(void *)(in_row_group_ctr),(void *)(in_row_group_ctr))))++;
}


/*
 * These are the routines invoked by sep_upsample to upsample pixel values
 * of a single component.  One row group is processed per call.
 */


/*
 * For full-size components, we just make color_buf[ci] point at the
 * input buffer, and thus avoid copying any data.  Note that this is
 * safe only because sep_upsample doesn't declare the input row group
 * "consumed" until we are done color converting and emitting it.
 */

METHODDEF(void)
fullsize_upsample (j_decompress_ptr cinfo, jpeg_component_info * compptr,
		   JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr)
{
  *(JSAMPARRAY *)(__boundcheck_ptr_reference(160,4,"fullsize_upsample",(void *)(output_data_ptr),(void *)(output_data_ptr))) = input_data;
}


/*
 * This is a no-op version used for "uninteresting" components.
 * These components will not be referenced by color conversion.
 */

METHODDEF(void)
noop_upsample (j_decompress_ptr cinfo, jpeg_component_info * compptr,
	       JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr)
{
  *(JSAMPARRAY *)(__boundcheck_ptr_reference(173,4,"noop_upsample",(void *)(output_data_ptr),(void *)(output_data_ptr))) = NULL;	/* safety check */
}


/*
 * This version handles any integral sampling ratios.
 * This is not used for typical JPEG files, so it need not be fast.
 * Nor, for that matter, is it particularly accurate: the algorithm is
 * simple replication of the input pixel onto the corresponding output
 * pixels.  The hi-falutin sampling literature refers to this as a
 * "box filter".  A box filter tends to introduce visible artifacts,
 * so if you are actually going to use 3:1 or 4:1 sampling ratios
 * you would be well advised to improve this code.
 */

METHODDEF(void)
int_upsample (j_decompress_ptr cinfo, jpeg_component_info * compptr,
	      JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr)
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));

  JSAMPARRAY output_data = *(JSAMPARRAY *)(__boundcheck_ptr_reference(193,29,"int_upsample",(void *)(output_data_ptr),(void *)(output_data_ptr)));
__boundcheck_metadata_store((void *)(&output_data),(void *)((size_t)(&output_data)+sizeof(output_data)*8-1));

  register JSAMPROW inptr, outptr;
  register JSAMPLE invalue;
  register int h;
  JSAMPROW outend;
__boundcheck_metadata_store((void *)(&outend),(void *)((size_t)(&outend)+sizeof(outend)*8-1));

  int h_expand;
__boundcheck_metadata_store((void *)(&h_expand),(void *)((size_t)(&h_expand)+sizeof(h_expand)*8-1));
int  v_expand;
__boundcheck_metadata_store((void *)(&v_expand),(void *)((size_t)(&v_expand)+sizeof(v_expand)*8-1));

  int inrow;
__boundcheck_metadata_store((void *)(&inrow),(void *)((size_t)(&inrow)+sizeof(inrow)*8-1));
int  outrow;
__boundcheck_metadata_store((void *)(&outrow),(void *)((size_t)(&outrow)+sizeof(outrow)*8-1));


  h_expand = upsample->h_expand[_RV_insert_check(0,10,201,14,"int_upsample",compptr->component_index)];
  v_expand = upsample->v_expand[_RV_insert_check(0,10,202,14,"int_upsample",compptr->component_index)];

  inrow = outrow = 0;
  while (outrow < cinfo->max_v_samp_factor) {
    /* Generate one output row with proper horizontal expansion */
    inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(207,13,"int_upsample",(void *)(&input_data[0]),(void *)(&input_data[inrow]))));
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(208,14,"int_upsample",(void *)(&output_data[0]),(void *)(&output_data[outrow]))));
    outend = outptr + cinfo->output_width;
__boundcheck_metadata_trans_check((void *)(outend),(void *)(outptr),(void *)(outptr + cinfo->output_width));

    while (outptr < outend) {
      invalue = *(JSAMPROW)(__boundcheck_ptr_reference(211,23,"int_upsample",(void *)(inptr),(void *)(inptr++)));	/* don't need GETJSAMPLE() here */
      for (h = h_expand; h > 0; h--) {
	*(JSAMPROW)(__boundcheck_ptr_reference(213,9,"int_upsample",(void *)(outptr),(void *)(outptr++))) = invalue;
      }
    }
    /* Generate any additional output rows by duplicating the first one */
    if (v_expand > 1) {
      jcopy_sample_rows(output_data, outrow, output_data, outrow+1,
			v_expand-1, cinfo->output_width);
    }
    inrow++;
    outrow += v_expand;
  }
}


/*
 * Fast processing for the common case of 2:1 horizontal and 1:1 vertical.
 * It's still a box filter.
 */

METHODDEF(void)
h2v1_upsample (j_decompress_ptr cinfo, jpeg_component_info * compptr,
	       JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr)
{
  JSAMPARRAY output_data = *(JSAMPARRAY *)(__boundcheck_ptr_reference(236,29,"h2v1_upsample",(void *)(output_data_ptr),(void *)(output_data_ptr)));
__boundcheck_metadata_store((void *)(&output_data),(void *)((size_t)(&output_data)+sizeof(output_data)*8-1));

  register JSAMPROW inptr, outptr;
  register JSAMPLE invalue;
  JSAMPROW outend;
__boundcheck_metadata_store((void *)(&outend),(void *)((size_t)(&outend)+sizeof(outend)*8-1));

  int inrow;
__boundcheck_metadata_store((void *)(&inrow),(void *)((size_t)(&inrow)+sizeof(inrow)*8-1));


  for (inrow = 0; inrow < cinfo->max_v_samp_factor; inrow++) {
    inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(243,13,"h2v1_upsample",(void *)(&input_data[0]),(void *)(&input_data[inrow]))));
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(244,14,"h2v1_upsample",(void *)(&output_data[0]),(void *)(&output_data[inrow]))));
    outend = outptr + cinfo->output_width;
__boundcheck_metadata_trans_check((void *)(outend),(void *)(outptr),(void *)(outptr + cinfo->output_width));

    while (outptr < outend) {
      invalue = *(JSAMPROW)(__boundcheck_ptr_reference(247,23,"h2v1_upsample",(void *)(inptr),(void *)(inptr++)));	/* don't need GETJSAMPLE() here */
      *(JSAMPROW)(__boundcheck_ptr_reference(248,14,"h2v1_upsample",(void *)(outptr),(void *)(outptr++))) = invalue;
      *(JSAMPROW)(__boundcheck_ptr_reference(249,14,"h2v1_upsample",(void *)(outptr),(void *)(outptr++))) = invalue;
    }
  }
}


/*
 * Fast processing for the common case of 2:1 horizontal and 2:1 vertical.
 * It's still a box filter.
 */

METHODDEF(void)
h2v2_upsample (j_decompress_ptr cinfo, jpeg_component_info * compptr,
	       JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr)
{
  JSAMPARRAY output_data = *(JSAMPARRAY *)(__boundcheck_ptr_reference(264,29,"h2v2_upsample",(void *)(output_data_ptr),(void *)(output_data_ptr)));
__boundcheck_metadata_store((void *)(&output_data),(void *)((size_t)(&output_data)+sizeof(output_data)*8-1));

  register JSAMPROW inptr, outptr;
  register JSAMPLE invalue;
  JSAMPROW outend;
__boundcheck_metadata_store((void *)(&outend),(void *)((size_t)(&outend)+sizeof(outend)*8-1));

  int inrow;
__boundcheck_metadata_store((void *)(&inrow),(void *)((size_t)(&inrow)+sizeof(inrow)*8-1));
int  outrow;
__boundcheck_metadata_store((void *)(&outrow),(void *)((size_t)(&outrow)+sizeof(outrow)*8-1));


  inrow = outrow = 0;
  while (outrow < cinfo->max_v_samp_factor) {
    inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(272,13,"h2v2_upsample",(void *)(&input_data[0]),(void *)(&input_data[inrow]))));
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(273,14,"h2v2_upsample",(void *)(&output_data[0]),(void *)(&output_data[outrow]))));
    outend = outptr + cinfo->output_width;
__boundcheck_metadata_trans_check((void *)(outend),(void *)(outptr),(void *)(outptr + cinfo->output_width));

    while (outptr < outend) {
      invalue = *(JSAMPROW)(__boundcheck_ptr_reference(276,23,"h2v2_upsample",(void *)(inptr),(void *)(inptr++)));	/* don't need GETJSAMPLE() here */
      *(JSAMPROW)(__boundcheck_ptr_reference(277,14,"h2v2_upsample",(void *)(outptr),(void *)(outptr++))) = invalue;
      *(JSAMPROW)(__boundcheck_ptr_reference(278,14,"h2v2_upsample",(void *)(outptr),(void *)(outptr++))) = invalue;
    }
    jcopy_sample_rows(output_data, outrow, output_data, outrow+1,
		      1, cinfo->output_width);
    inrow++;
    outrow += 2;
  }
}


/*
 * Fancy processing for the common case of 2:1 horizontal and 1:1 vertical.
 *
 * The upsampling algorithm is linear interpolation between pixel centers,
 * also known as a "triangle filter".  This is a good compromise between
 * speed and visual quality.  The centers of the output pixels are 1/4 and 3/4
 * of the way between input pixel centers.
 *
 * A note about the "bias" calculations: when rounding fractional values to
 * integer, we do not want to always round 0.5 up to the next integer.
 * If we did that, we'd introduce a noticeable bias towards larger values.
 * Instead, this code is arranged so that 0.5 will be rounded up or down at
 * alternate pixel locations (a simple ordered dither pattern).
 */

METHODDEF(void)
h2v1_fancy_upsample (j_decompress_ptr cinfo, jpeg_component_info * compptr,
		     JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr)
{
  JSAMPARRAY output_data = *(JSAMPARRAY *)(__boundcheck_ptr_reference(307,29,"h2v1_fancy_upsample",(void *)(output_data_ptr),(void *)(output_data_ptr)));
__boundcheck_metadata_store((void *)(&output_data),(void *)((size_t)(&output_data)+sizeof(output_data)*8-1));

  register JSAMPROW inptr, outptr;
  register int invalue;
  register JDIMENSION colctr;
  int inrow;
__boundcheck_metadata_store((void *)(&inrow),(void *)((size_t)(&inrow)+sizeof(inrow)*8-1));


  for (inrow = 0; inrow < cinfo->max_v_samp_factor; inrow++) {
    inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(314,13,"h2v1_fancy_upsample",(void *)(&input_data[0]),(void *)(&input_data[inrow]))));
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(315,14,"h2v1_fancy_upsample",(void *)(&output_data[0]),(void *)(&output_data[inrow]))));
    /* Special case for first column */
    invalue = GETJSAMPLE(*inptr++);
    *(JSAMPROW)(__boundcheck_ptr_reference(318,12,"h2v1_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) invalue;
    *(JSAMPROW)(__boundcheck_ptr_reference(319,12,"h2v1_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((invalue * 3 + GETJSAMPLE(*inptr) + 2) >> 2);

    for (colctr = compptr->downsampled_width - 2; colctr > 0; colctr--) {
      /* General case: 3/4 * nearer pixel + 1/4 * further pixel */
      invalue = GETJSAMPLE(*inptr++) * 3;
      *(JSAMPROW)(__boundcheck_ptr_reference(324,14,"h2v1_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((invalue + GETJSAMPLE(inptr[-2]) + 1) >> 2);
      *(JSAMPROW)(__boundcheck_ptr_reference(325,14,"h2v1_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((invalue + GETJSAMPLE(*inptr) + 2) >> 2);
    }

    /* Special case for last column */
    invalue = GETJSAMPLE(*inptr);
    *(JSAMPROW)(__boundcheck_ptr_reference(330,12,"h2v1_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((invalue * 3 + GETJSAMPLE(inptr[-1]) + 1) >> 2);
    *(JSAMPROW)(__boundcheck_ptr_reference(331,12,"h2v1_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) invalue;
  }
}


/*
 * Fancy processing for the common case of 2:1 horizontal and 2:1 vertical.
 * Again a triangle filter; see comments for h2v1 case, above.
 *
 * It is OK for us to reference the adjacent input rows because we demanded
 * context from the main buffer controller (see initialization code).
 */

METHODDEF(void)
h2v2_fancy_upsample (j_decompress_ptr cinfo, jpeg_component_info * compptr,
		     JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr)
{
  JSAMPARRAY output_data = *(JSAMPARRAY *)(__boundcheck_ptr_reference(348,29,"h2v2_fancy_upsample",(void *)(output_data_ptr),(void *)(output_data_ptr)));
__boundcheck_metadata_store((void *)(&output_data),(void *)((size_t)(&output_data)+sizeof(output_data)*8-1));

  register JSAMPROW inptr0, inptr1, outptr;
#if BITS_IN_JSAMPLE == 8
  register int thiscolsum, lastcolsum, nextcolsum;
#else
  register INT32 thiscolsum, lastcolsum, nextcolsum;
#endif
  register JDIMENSION colctr;
  int inrow;
__boundcheck_metadata_store((void *)(&inrow),(void *)((size_t)(&inrow)+sizeof(inrow)*8-1));
int  outrow;
__boundcheck_metadata_store((void *)(&outrow),(void *)((size_t)(&outrow)+sizeof(outrow)*8-1));
int  v;
__boundcheck_metadata_store((void *)(&v),(void *)((size_t)(&v)+sizeof(v)*8-1));


  inrow = outrow = 0;
  while (outrow < cinfo->max_v_samp_factor) {
    for (v = 0; v < 2; v++) {
      /* inptr0 points to nearest input row, inptr1 points to next nearest */
      inptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(362,16,"h2v2_fancy_upsample",(void *)(&input_data[0]),(void *)(&input_data[inrow]))));
      if (v == 0)		/* next nearest is row above */
	inptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(364,11,"h2v2_fancy_upsample",(void *)(&input_data[0]),(void *)(&input_data[inrow - 1]))));
      else			/* next nearest is row below */
	inptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(366,11,"h2v2_fancy_upsample",(void *)(&input_data[0]),(void *)(&input_data[inrow + 1]))));
      outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(367,16,"h2v2_fancy_upsample",(void *)(&output_data[0]),(void *)(&output_data[outrow++]))));

      /* Special case for first column */
      thiscolsum = GETJSAMPLE(*inptr0++) * 3 + GETJSAMPLE(*inptr1++);
      nextcolsum = GETJSAMPLE(*inptr0++) * 3 + GETJSAMPLE(*inptr1++);
      *(JSAMPROW)(__boundcheck_ptr_reference(372,14,"h2v2_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((thiscolsum * 4 + 8) >> 4);
      *(JSAMPROW)(__boundcheck_ptr_reference(373,14,"h2v2_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((thiscolsum * 3 + nextcolsum + 7) >> 4);
      lastcolsum = thiscolsum; thiscolsum = nextcolsum;

      for (colctr = compptr->downsampled_width - 2; colctr > 0; colctr--) {
	/* General case: 3/4 * nearer pixel + 1/4 * further pixel in each */
	/* dimension, thus 9/16, 3/16, 3/16, 1/16 overall */
	nextcolsum = GETJSAMPLE(*inptr0++) * 3 + GETJSAMPLE(*inptr1++);
	*(JSAMPROW)(__boundcheck_ptr_reference(380,9,"h2v2_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((thiscolsum * 3 + lastcolsum + 8) >> 4);
	*(JSAMPROW)(__boundcheck_ptr_reference(381,9,"h2v2_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((thiscolsum * 3 + nextcolsum + 7) >> 4);
	lastcolsum = thiscolsum; thiscolsum = nextcolsum;
      }

      /* Special case for last column */
      *(JSAMPROW)(__boundcheck_ptr_reference(386,14,"h2v2_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((thiscolsum * 3 + lastcolsum + 8) >> 4);
      *(JSAMPROW)(__boundcheck_ptr_reference(387,14,"h2v2_fancy_upsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((thiscolsum * 4 + 7) >> 4);
    }
    inrow++;
  }
}


/*
 * Module initialization routine for upsampling.
 */

GLOBAL(void)
jinit_upsampler (j_decompress_ptr cinfo)
{
  my_upsample_ptr upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  boolean need_buffer;
__boundcheck_metadata_store((void *)(&need_buffer),(void *)((size_t)(&need_buffer)+sizeof(need_buffer)*8-1));
boolean  do_fancy;
__boundcheck_metadata_store((void *)(&do_fancy),(void *)((size_t)(&do_fancy)+sizeof(do_fancy)*8-1));

  int h_in_group;
__boundcheck_metadata_store((void *)(&h_in_group),(void *)((size_t)(&h_in_group)+sizeof(h_in_group)*8-1));
int  v_in_group;
__boundcheck_metadata_store((void *)(&v_in_group),(void *)((size_t)(&v_in_group)+sizeof(v_in_group)*8-1));
int  h_out_group;
__boundcheck_metadata_store((void *)(&h_out_group),(void *)((size_t)(&h_out_group)+sizeof(h_out_group)*8-1));
int  v_out_group;
__boundcheck_metadata_store((void *)(&v_out_group),(void *)((size_t)(&v_out_group)+sizeof(v_out_group)*8-1));


  upsample = (my_upsample_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(408,19,"jinit_upsampler",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_upsampler));
  cinfo->upsample = (struct jpeg_upsampler *) upsample;
  upsample->pub.start_pass = start_pass_upsample;
  upsample->pub.upsample = sep_upsample;
  upsample->pub.need_context_rows = FALSE; /* until we find out differently */

  if (cinfo->CCIR601_sampling)	/* this isn't supported */
    ERREXIT(cinfo, JERR_CCIR601_NOTIMPL);

  /* jdmainct.c doesn't support context rows when min_DCT_scaled_size = 1,
   * so don't ask for it.
   */
  do_fancy = cinfo->do_fancy_upsampling && cinfo->min_DCT_scaled_size > 1;

  /* Verify we can handle the sampling factors, select per-component methods,
   * and create storage as needed.
   */
  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    /* Compute size of an "input group" after IDCT scaling.  This many samples
     * are to be converted to max_h_samp_factor * max_v_samp_factor pixels.
     */
    h_in_group = (compptr->h_samp_factor * compptr->DCT_scaled_size) /
		 cinfo->min_DCT_scaled_size;
    v_in_group = (compptr->v_samp_factor * compptr->DCT_scaled_size) /
		 cinfo->min_DCT_scaled_size;
    h_out_group = cinfo->max_h_samp_factor;
    v_out_group = cinfo->max_v_samp_factor;
    upsample->rowgroup_height[_RV_insert_check(0,10,437,5,"jinit_upsampler",ci)] = v_in_group; /* save for use later */
    need_buffer = TRUE;
    if (! compptr->component_needed) {
      /* Don't bother to upsample an uninteresting component. */
      upsample->methods[_RV_insert_check(0,10,441,7,"jinit_upsampler",ci)] = noop_upsample;
      need_buffer = FALSE;
    } else if (h_in_group == h_out_group && v_in_group == v_out_group) {
      /* Fullsize components can be processed without any work. */
      upsample->methods[_RV_insert_check(0,10,445,7,"jinit_upsampler",ci)] = fullsize_upsample;
      need_buffer = FALSE;
    } else if (h_in_group * 2 == h_out_group &&
	       v_in_group == v_out_group) {
      /* Special cases for 2h1v upsampling */
      if (do_fancy && compptr->downsampled_width > 2)
	upsample->methods[_RV_insert_check(0,10,451,2,"jinit_upsampler",ci)] = h2v1_fancy_upsample;
      else
	upsample->methods[_RV_insert_check(0,10,453,2,"jinit_upsampler",ci)] = h2v1_upsample;
    } else if (h_in_group * 2 == h_out_group &&
	       v_in_group * 2 == v_out_group) {
      /* Special cases for 2h2v upsampling */
      if (do_fancy && compptr->downsampled_width > 2) {
	upsample->methods[_RV_insert_check(0,10,458,2,"jinit_upsampler",ci)] = h2v2_fancy_upsample;
	upsample->pub.need_context_rows = TRUE;
      } else
	upsample->methods[_RV_insert_check(0,10,461,2,"jinit_upsampler",ci)] = h2v2_upsample;
    } else if ((h_out_group % h_in_group) == 0 &&
	       (v_out_group % v_in_group) == 0) {
      /* Generic integral-factors upsampling method */
      upsample->methods[_RV_insert_check(0,10,465,7,"jinit_upsampler",ci)] = int_upsample;
      upsample->h_expand[_RV_insert_check(0,10,466,7,"jinit_upsampler",ci)] = (UINT8) (h_out_group / h_in_group);
      upsample->v_expand[_RV_insert_check(0,10,467,7,"jinit_upsampler",ci)] = (UINT8) (v_out_group / v_in_group);
    } else
      ERREXIT(cinfo, JERR_FRACT_SAMPLE_NOTIMPL);
    if (need_buffer) {
      upsample->color_buf[_RV_insert_check(0,10,471,7,"jinit_upsampler",ci)] = (*(JSAMPARRAY (*)(j_common_ptr, int, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(471,47,"jinit_upsampler",(void *)(cinfo->mem->alloc_sarray),(void *)cinfo->mem->alloc_sarray)))
	((j_common_ptr) cinfo, JPOOL_IMAGE,
	 (JDIMENSION) jround_up((long) cinfo->output_width,
				(long) cinfo->max_h_samp_factor),
	 (JDIMENSION) cinfo->max_v_samp_factor);
    }
  }
}
