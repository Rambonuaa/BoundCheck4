/*
 * jcsample.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains downsampling routines.
 *
 * Downsampling input data is counted in "row groups".  A row group
 * is defined to be max_v_samp_factor pixel rows of each component,
 * from which the downsampler produces v_samp_factor sample rows.
 * A single row group is processed in each call to the downsampler module.
 *
 * The downsampler is responsible for edge-expansion of its output data
 * to fill an integral number of DCT blocks horizontally.  The source buffer
 * may be modified if it is helpful for this purpose (the source buffer is
 * allocated wide enough to correspond to the desired output width).
 * The caller (the prep controller) is responsible for vertical padding.
 *
 * The downsampler may request "context rows" by setting need_context_rows
 * during startup.  In this case, the input arrays will contain at least
 * one row group's worth of pixels above and below the passed-in data;
 * the caller will create dummy rows at image top and bottom by replicating
 * the first or last real pixel row.
 *
 * An excellent reference for image resampling is
 *   Digital Image Warping, George Wolberg, 1990.
 *   Pub. by IEEE Computer Society Press, Los Alamitos, CA. ISBN 0-8186-8944-7.
 *
 * The downsampling algorithm used here is a simple average of the source
 * pixels covered by the output pixel.  The hi-falutin sampling literature
 * refers to this as a "box filter".  In general the characteristics of a box
 * filter are not very good, but for the specific cases we normally use (1:1
 * and 2:1 ratios) the box is equivalent to a "triangle filter" which is not
 * nearly so bad.  If you intend to use other sampling ratios, you'd be well
 * advised to improve this code.
 *
 * A simple input-smoothing capability is provided.  This is mainly intended
 * for cleaning up color-dithered GIF input files (if you find it inadequate,
 * we suggest using an external filtering program such as pnmconvol).  When
 * enabled, each input pixel P is replaced by a weighted sum of itself and its
 * eight neighbors.  P's weight is 1-8*SF and each neighbor's weight is SF,
 * where SF = (smoothing_factor / 1024).
 * Currently, smoothing is only supported for 2h2v sampling factors.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/* Pointer to routine to downsample a single component */
typedef JMETHOD(void, downsample1_ptr,
		(j_compress_ptr cinfo, jpeg_component_info * compptr,
		 JSAMPARRAY input_data, JSAMPARRAY output_data));

/* Private subobject */

typedef struct {
  struct jpeg_downsampler pub;	/* public fields */

  /* Downsampling method pointers, one per component */
  downsample1_ptr methods[MAX_COMPONENTS];
} my_downsampler;

typedef my_downsampler * my_downsample_ptr;


/*
 * Initialize for a downsampling pass.
 */

METHODDEF(void)
start_pass_downsample (j_compress_ptr cinfo)
{
  /* no work for now */
}


/*
 * Expand a component horizontally from width input_cols to width output_cols,
 * by duplicating the rightmost samples.
 */

LOCAL(void)
expand_right_edge (JSAMPARRAY image_data, int num_rows,
		   JDIMENSION input_cols, JDIMENSION output_cols)
{
  register JSAMPROW ptr;
  register JSAMPLE pixval;
  register int count;
  int row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));

  int numcols = (int) (output_cols - input_cols);
__boundcheck_metadata_store((void *)(&numcols),(void *)((size_t)(&numcols)+sizeof(numcols)*8-1));


  if (numcols > 0) {
    for (row = 0; row < num_rows; row++) {
      ptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(98,13,"expand_right_edge",(void *)(&image_data[0]),(void *)(&image_data[row])))) + input_cols;
      pixval = (*(JSAMPLE *)(__boundcheck_ptr_reference(99,16,"expand_right_edge",(void *)(&ptr[0]),(void *)(&ptr[-1]))));		/* don't need GETJSAMPLE() here */
      for (count = numcols; count > 0; count--)
	*(JSAMPROW)(__boundcheck_ptr_reference(101,6,"expand_right_edge",(void *)(ptr),(void *)(ptr++))) = pixval;
    }
  }
}


/*
 * Do downsampling for a whole row group (all components).
 *
 * In this version we simply downsample each component independently.
 */

METHODDEF(void)
sep_downsample (j_compress_ptr cinfo,
		JSAMPIMAGE input_buf, JDIMENSION in_row_index,
		JSAMPIMAGE output_buf, JDIMENSION out_row_group_index)
{
  my_downsample_ptr downsample = (my_downsample_ptr) cinfo->downsample;
__boundcheck_metadata_store((void *)(&downsample),(void *)((size_t)(&downsample)+sizeof(downsample)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  JSAMPARRAY in_ptr;
__boundcheck_metadata_store((void *)(&in_ptr),(void *)((size_t)(&in_ptr)+sizeof(in_ptr)*8-1));
JSAMPARRAY  out_ptr;
__boundcheck_metadata_store((void *)(&out_ptr),(void *)((size_t)(&out_ptr)+sizeof(out_ptr)*8-1));


  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    in_ptr = (*(JSAMPARRAY *)(__boundcheck_ptr_reference(125,14,"sep_downsample",(void *)(&input_buf[0]),(void *)(&input_buf[ci])))) + in_row_index;
    out_ptr = (*(JSAMPARRAY *)(__boundcheck_ptr_reference(126,15,"sep_downsample",(void *)(&output_buf[0]),(void *)(&output_buf[ci])))) + (out_row_group_index * compptr->v_samp_factor);
    (*(downsample1_ptr)(__boundcheck_ptr_reference(127,29,"sep_downsample",(void *)(downsample->methods[ci]),(void *)downsample->methods[_RV_insert_check(0,10,127,7,"sep_downsample",ci)]))) (cinfo, compptr, in_ptr, out_ptr);
  }
}


/*
 * Downsample pixel values of a single component.
 * One row group is processed per call.
 * This version handles arbitrary integral sampling ratios, without smoothing.
 * Note that this version is not actually used for customary sampling ratios.
 */

METHODDEF(void)
int_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
		JSAMPARRAY input_data, JSAMPARRAY output_data)
{
  int inrow;
__boundcheck_metadata_store((void *)(&inrow),(void *)((size_t)(&inrow)+sizeof(inrow)*8-1));
int  outrow;
__boundcheck_metadata_store((void *)(&outrow),(void *)((size_t)(&outrow)+sizeof(outrow)*8-1));
int  h_expand;
__boundcheck_metadata_store((void *)(&h_expand),(void *)((size_t)(&h_expand)+sizeof(h_expand)*8-1));
int  v_expand;
__boundcheck_metadata_store((void *)(&v_expand),(void *)((size_t)(&v_expand)+sizeof(v_expand)*8-1));
int  numpix;
__boundcheck_metadata_store((void *)(&numpix),(void *)((size_t)(&numpix)+sizeof(numpix)*8-1));
int  numpix2;
__boundcheck_metadata_store((void *)(&numpix2),(void *)((size_t)(&numpix2)+sizeof(numpix2)*8-1));
int  h;
__boundcheck_metadata_store((void *)(&h),(void *)((size_t)(&h)+sizeof(h)*8-1));
int  v;
__boundcheck_metadata_store((void *)(&v),(void *)((size_t)(&v)+sizeof(v)*8-1));

  JDIMENSION outcol;
__boundcheck_metadata_store((void *)(&outcol),(void *)((size_t)(&outcol)+sizeof(outcol)*8-1));
JDIMENSION  outcol_h;
__boundcheck_metadata_store((void *)(&outcol_h),(void *)((size_t)(&outcol_h)+sizeof(outcol_h)*8-1));
	/* outcol_h == outcol*h_expand */
  JDIMENSION output_cols = compptr->width_in_blocks * DCTSIZE;
__boundcheck_metadata_store((void *)(&output_cols),(void *)((size_t)(&output_cols)+sizeof(output_cols)*8-1));

  JSAMPROW inptr;
__boundcheck_metadata_store((void *)(&inptr),(void *)((size_t)(&inptr)+sizeof(inptr)*8-1));
JSAMPROW  outptr;
__boundcheck_metadata_store((void *)(&outptr),(void *)((size_t)(&outptr)+sizeof(outptr)*8-1));

  INT32 outvalue;
__boundcheck_metadata_store((void *)(&outvalue),(void *)((size_t)(&outvalue)+sizeof(outvalue)*8-1));


  h_expand = cinfo->max_h_samp_factor / compptr->h_samp_factor;
  v_expand = cinfo->max_v_samp_factor / compptr->v_samp_factor;
  numpix = h_expand * v_expand;
  numpix2 = numpix/2;

  /* Expand input data enough to let all the output samples be generated
   * by the standard loop.  Special-casing padded output would be more
   * efficient.
   */
  expand_right_edge(input_data, cinfo->max_v_samp_factor,
		    cinfo->image_width, output_cols * h_expand);

  inrow = 0;
  for (outrow = 0; outrow < compptr->v_samp_factor; outrow++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(163,14,"int_downsample",(void *)(&output_data[0]),(void *)(&output_data[outrow]))));
    for (outcol = 0, outcol_h = 0; outcol < output_cols;
	 outcol++, outcol_h += h_expand) {
      outvalue = 0;
      for (v = 0; v < v_expand; v++) {
	inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(168,10,"int_downsample",(void *)(&input_data[0]),(void *)(&input_data[inrow + v])))) + outcol_h;
	for (h = 0; h < h_expand; h++) {
	  outvalue += (INT32) GETJSAMPLE(*inptr++);
	}
      }
      *(JSAMPROW)(__boundcheck_ptr_reference(173,14,"int_downsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((outvalue + numpix2) / numpix);
    }
    inrow += v_expand;
  }
}


/*
 * Downsample pixel values of a single component.
 * This version handles the special case of a full-size component,
 * without smoothing.
 */

METHODDEF(void)
fullsize_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
		     JSAMPARRAY input_data, JSAMPARRAY output_data)
{
  /* Copy the data */
  jcopy_sample_rows(input_data, 0, output_data, 0,
		    cinfo->max_v_samp_factor, cinfo->image_width);
  /* Edge-expand */
  expand_right_edge(output_data, cinfo->max_v_samp_factor,
		    cinfo->image_width, compptr->width_in_blocks * DCTSIZE);
}


/*
 * Downsample pixel values of a single component.
 * This version handles the common case of 2:1 horizontal and 1:1 vertical,
 * without smoothing.
 *
 * A note about the "bias" calculations: when rounding fractional values to
 * integer, we do not want to always round 0.5 up to the next integer.
 * If we did that, we'd introduce a noticeable bias towards larger values.
 * Instead, this code is arranged so that 0.5 will be rounded up or down at
 * alternate pixel locations (a simple ordered dither pattern).
 */

METHODDEF(void)
h2v1_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
		 JSAMPARRAY input_data, JSAMPARRAY output_data)
{
  int outrow;
__boundcheck_metadata_store((void *)(&outrow),(void *)((size_t)(&outrow)+sizeof(outrow)*8-1));

  JDIMENSION outcol;
__boundcheck_metadata_store((void *)(&outcol),(void *)((size_t)(&outcol)+sizeof(outcol)*8-1));

  JDIMENSION output_cols = compptr->width_in_blocks * DCTSIZE;
__boundcheck_metadata_store((void *)(&output_cols),(void *)((size_t)(&output_cols)+sizeof(output_cols)*8-1));

  register JSAMPROW inptr, outptr;
  register int bias;

  /* Expand input data enough to let all the output samples be generated
   * by the standard loop.  Special-casing padded output would be more
   * efficient.
   */
  expand_right_edge(input_data, cinfo->max_v_samp_factor,
		    cinfo->image_width, output_cols * 2);

  for (outrow = 0; outrow < compptr->v_samp_factor; outrow++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(229,14,"h2v1_downsample",(void *)(&output_data[0]),(void *)(&output_data[outrow]))));
    inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(230,13,"h2v1_downsample",(void *)(&input_data[0]),(void *)(&input_data[outrow]))));
    bias = 0;			/* bias = 0,1,0,1,... for successive samples */
    for (outcol = 0; outcol < output_cols; outcol++) {
      *(JSAMPROW)(__boundcheck_ptr_reference(233,14,"h2v1_downsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((GETJSAMPLE(*inptr) + GETJSAMPLE(inptr[1])
			      + bias) >> 1);
      bias ^= 1;		/* 0=>1, 1=>0 */
      inptr += 2;
    }
  }
}


/*
 * Downsample pixel values of a single component.
 * This version handles the standard case of 2:1 horizontal and 2:1 vertical,
 * without smoothing.
 */

METHODDEF(void)
h2v2_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
		 JSAMPARRAY input_data, JSAMPARRAY output_data)
{
  int inrow;
__boundcheck_metadata_store((void *)(&inrow),(void *)((size_t)(&inrow)+sizeof(inrow)*8-1));
int  outrow;
__boundcheck_metadata_store((void *)(&outrow),(void *)((size_t)(&outrow)+sizeof(outrow)*8-1));

  JDIMENSION outcol;
__boundcheck_metadata_store((void *)(&outcol),(void *)((size_t)(&outcol)+sizeof(outcol)*8-1));

  JDIMENSION output_cols = compptr->width_in_blocks * DCTSIZE;
__boundcheck_metadata_store((void *)(&output_cols),(void *)((size_t)(&output_cols)+sizeof(output_cols)*8-1));

  register JSAMPROW inptr0, inptr1, outptr;
  register int bias;

  /* Expand input data enough to let all the output samples be generated
   * by the standard loop.  Special-casing padded output would be more
   * efficient.
   */
  expand_right_edge(input_data, cinfo->max_v_samp_factor,
		    cinfo->image_width, output_cols * 2);

  inrow = 0;
  for (outrow = 0; outrow < compptr->v_samp_factor; outrow++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(267,14,"h2v2_downsample",(void *)(&output_data[0]),(void *)(&output_data[outrow]))));
    inptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(268,14,"h2v2_downsample",(void *)(&input_data[0]),(void *)(&input_data[inrow]))));
    inptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(269,14,"h2v2_downsample",(void *)(&input_data[0]),(void *)(&input_data[inrow + 1]))));
    bias = 1;			/* bias = 1,2,1,2,... for successive samples */
    for (outcol = 0; outcol < output_cols; outcol++) {
      *(JSAMPROW)(__boundcheck_ptr_reference(272,14,"h2v2_downsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((GETJSAMPLE(*inptr0) + GETJSAMPLE(inptr0[1]) +
			      GETJSAMPLE(*inptr1) + GETJSAMPLE(inptr1[1])
			      + bias) >> 2);
      bias ^= 3;		/* 1=>2, 2=>1 */
      inptr0 += 2; inptr1 += 2;
    }
    inrow += 2;
  }
}


#ifdef INPUT_SMOOTHING_SUPPORTED

/*
 * Downsample pixel values of a single component.
 * This version handles the standard case of 2:1 horizontal and 2:1 vertical,
 * with smoothing.  One row of context is required.
 */

METHODDEF(void)
h2v2_smooth_downsample (j_compress_ptr cinfo, jpeg_component_info * compptr,
			JSAMPARRAY input_data, JSAMPARRAY output_data)
{
  int inrow;
__boundcheck_metadata_store((void *)(&inrow),(void *)((size_t)(&inrow)+sizeof(inrow)*8-1));
int  outrow;
__boundcheck_metadata_store((void *)(&outrow),(void *)((size_t)(&outrow)+sizeof(outrow)*8-1));

  JDIMENSION colctr;
__boundcheck_metadata_store((void *)(&colctr),(void *)((size_t)(&colctr)+sizeof(colctr)*8-1));

  JDIMENSION output_cols = compptr->width_in_blocks * DCTSIZE;
__boundcheck_metadata_store((void *)(&output_cols),(void *)((size_t)(&output_cols)+sizeof(output_cols)*8-1));

  register JSAMPROW inptr0, inptr1, above_ptr, below_ptr, outptr;
  INT32 membersum;
__boundcheck_metadata_store((void *)(&membersum),(void *)((size_t)(&membersum)+sizeof(membersum)*8-1));
INT32  neighsum;
__boundcheck_metadata_store((void *)(&neighsum),(void *)((size_t)(&neighsum)+sizeof(neighsum)*8-1));
INT32  memberscale;
__boundcheck_metadata_store((void *)(&memberscale),(void *)((size_t)(&memberscale)+sizeof(memberscale)*8-1));
INT32  neighscale;
__boundcheck_metadata_store((void *)(&neighscale),(void *)((size_t)(&neighscale)+sizeof(neighscale)*8-1));


  /* Expand input data enough to let all the output samples be generated
   * by the standard loop.  Special-casing padded output would be more
   * efficient.
   */
  expand_right_edge(input_data - 1, cinfo->max_v_samp_factor + 2,
		    cinfo->image_width, output_cols * 2);

  /* We don't bother to form the individual "smoothed" input pixel values;
   * we can directly compute the output which is the average of the four
   * smoothed values.  Each of the four member pixels contributes a fraction
   * (1-8*SF) to its own smoothed image and a fraction SF to each of the three
   * other smoothed pixels, therefore a total fraction (1-5*SF)/4 to the final
   * output.  The four corner-adjacent neighbor pixels contribute a fraction
   * SF to just one smoothed pixel, or SF/4 to the final output; while the
   * eight edge-adjacent neighbors contribute SF to each of two smoothed
   * pixels, or SF/2 overall.  In order to use integer arithmetic, these
   * factors are scaled by 2^16 = 65536.
   * Also recall that SF = smoothing_factor / 1024.
   */

  memberscale = 16384 - cinfo->smoothing_factor * 80; /* scaled (1-5*SF)/4 */
  neighscale = cinfo->smoothing_factor * 16; /* scaled SF/4 */

  inrow = 0;
  for (outrow = 0; outrow < compptr->v_samp_factor; outrow++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(326,14,"h2v2_smooth_downsample",(void *)(&output_data[0]),(void *)(&output_data[outrow]))));
    inptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(327,14,"h2v2_smooth_downsample",(void *)(&input_data[0]),(void *)(&input_data[inrow]))));
    inptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(328,14,"h2v2_smooth_downsample",(void *)(&input_data[0]),(void *)(&input_data[inrow + 1]))));
    above_ptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(329,17,"h2v2_smooth_downsample",(void *)(&input_data[0]),(void *)(&input_data[inrow - 1]))));
    below_ptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(330,17,"h2v2_smooth_downsample",(void *)(&input_data[0]),(void *)(&input_data[inrow + 2]))));

    /* Special case for first column: pretend column -1 is same as column 0 */
    membersum = GETJSAMPLE(*inptr0) + GETJSAMPLE(inptr0[1]) +
		GETJSAMPLE(*inptr1) + GETJSAMPLE(inptr1[1]);
    neighsum = GETJSAMPLE(*above_ptr) + GETJSAMPLE(above_ptr[1]) +
	       GETJSAMPLE(*below_ptr) + GETJSAMPLE(below_ptr[1]) +
	       GETJSAMPLE(*inptr0) + GETJSAMPLE(inptr0[2]) +
	       GETJSAMPLE(*inptr1) + GETJSAMPLE(inptr1[2]);
    neighsum += neighsum;
    neighsum += GETJSAMPLE(*above_ptr) + GETJSAMPLE(above_ptr[2]) +
		GETJSAMPLE(*below_ptr) + GETJSAMPLE(below_ptr[2]);
    membersum = membersum * memberscale + neighsum * neighscale;
    *(JSAMPROW)(__boundcheck_ptr_reference(343,12,"h2v2_smooth_downsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((membersum + 32768) >> 16);
    inptr0 += 2; inptr1 += 2; above_ptr += 2; below_ptr += 2;

    for (colctr = output_cols - 2; colctr > 0; colctr--) {
      /* sum of pixels directly mapped to this output element */
      membersum = GETJSAMPLE(*inptr0) + GETJSAMPLE(inptr0[1]) +
		  GETJSAMPLE(*inptr1) + GETJSAMPLE(inptr1[1]);
      /* sum of edge-neighbor pixels */
      neighsum = GETJSAMPLE(*above_ptr) + GETJSAMPLE(above_ptr[1]) +
		 GETJSAMPLE(*below_ptr) + GETJSAMPLE(below_ptr[1]) +
		 GETJSAMPLE(inptr0[-1]) + GETJSAMPLE(inptr0[2]) +
		 GETJSAMPLE(inptr1[-1]) + GETJSAMPLE(inptr1[2]);
      /* The edge-neighbors count twice as much as corner-neighbors */
      neighsum += neighsum;
      /* Add in the corner-neighbors */
      neighsum += GETJSAMPLE(above_ptr[-1]) + GETJSAMPLE(above_ptr[2]) +
		  GETJSAMPLE(below_ptr[-1]) + GETJSAMPLE(below_ptr[2]);
      /* form final output scaled up by 2^16 */
      membersum = membersum * memberscale + neighsum * neighscale;
      /* round, descale and output it */
      *(JSAMPROW)(__boundcheck_ptr_reference(363,14,"h2v2_smooth_downsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((membersum + 32768) >> 16);
      inptr0 += 2; inptr1 += 2; above_ptr += 2; below_ptr += 2;
    }

    /* Special case for last column */
    membersum = GETJSAMPLE(*inptr0) + GETJSAMPLE(inptr0[1]) +
		GETJSAMPLE(*inptr1) + GETJSAMPLE(inptr1[1]);
    neighsum = GETJSAMPLE(*above_ptr) + GETJSAMPLE(above_ptr[1]) +
	       GETJSAMPLE(*below_ptr) + GETJSAMPLE(below_ptr[1]) +
	       GETJSAMPLE(inptr0[-1]) + GETJSAMPLE(inptr0[1]) +
	       GETJSAMPLE(inptr1[-1]) + GETJSAMPLE(inptr1[1]);
    neighsum += neighsum;
    neighsum += GETJSAMPLE(above_ptr[-1]) + GETJSAMPLE(above_ptr[1]) +
		GETJSAMPLE(below_ptr[-1]) + GETJSAMPLE(below_ptr[1]);
    membersum = membersum * memberscale + neighsum * neighscale;
    *(JSAMPROW)(__boundcheck_ptr_reference(378,6,"h2v2_smooth_downsample",(void *)(outptr),(void *)(outptr))) = (JSAMPLE) ((membersum + 32768) >> 16);

    inrow += 2;
  }
}


/*
 * Downsample pixel values of a single component.
 * This version handles the special case of a full-size component,
 * with smoothing.  One row of context is required.
 */

METHODDEF(void)
fullsize_smooth_downsample (j_compress_ptr cinfo, jpeg_component_info *compptr,
			    JSAMPARRAY input_data, JSAMPARRAY output_data)
{
  int outrow;
__boundcheck_metadata_store((void *)(&outrow),(void *)((size_t)(&outrow)+sizeof(outrow)*8-1));

  JDIMENSION colctr;
__boundcheck_metadata_store((void *)(&colctr),(void *)((size_t)(&colctr)+sizeof(colctr)*8-1));

  JDIMENSION output_cols = compptr->width_in_blocks * DCTSIZE;
__boundcheck_metadata_store((void *)(&output_cols),(void *)((size_t)(&output_cols)+sizeof(output_cols)*8-1));

  register JSAMPROW inptr, above_ptr, below_ptr, outptr;
  INT32 membersum;
__boundcheck_metadata_store((void *)(&membersum),(void *)((size_t)(&membersum)+sizeof(membersum)*8-1));
INT32  neighsum;
__boundcheck_metadata_store((void *)(&neighsum),(void *)((size_t)(&neighsum)+sizeof(neighsum)*8-1));
INT32  memberscale;
__boundcheck_metadata_store((void *)(&memberscale),(void *)((size_t)(&memberscale)+sizeof(memberscale)*8-1));
INT32  neighscale;
__boundcheck_metadata_store((void *)(&neighscale),(void *)((size_t)(&neighscale)+sizeof(neighscale)*8-1));

  int colsum;
__boundcheck_metadata_store((void *)(&colsum),(void *)((size_t)(&colsum)+sizeof(colsum)*8-1));
int  lastcolsum;
__boundcheck_metadata_store((void *)(&lastcolsum),(void *)((size_t)(&lastcolsum)+sizeof(lastcolsum)*8-1));
int  nextcolsum;
__boundcheck_metadata_store((void *)(&nextcolsum),(void *)((size_t)(&nextcolsum)+sizeof(nextcolsum)*8-1));


  /* Expand input data enough to let all the output samples be generated
   * by the standard loop.  Special-casing padded output would be more
   * efficient.
   */
  expand_right_edge(input_data - 1, cinfo->max_v_samp_factor + 2,
		    cinfo->image_width, output_cols);

  /* Each of the eight neighbor pixels contributes a fraction SF to the
   * smoothed pixel, while the main pixel contributes (1-8*SF).  In order
   * to use integer arithmetic, these factors are multiplied by 2^16 = 65536.
   * Also recall that SF = smoothing_factor / 1024.
   */

  memberscale = 65536L - cinfo->smoothing_factor * 512L; /* scaled 1-8*SF */
  neighscale = cinfo->smoothing_factor * 64; /* scaled SF */

  for (outrow = 0; outrow < compptr->v_samp_factor; outrow++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(419,14,"fullsize_smooth_downsample",(void *)(&output_data[0]),(void *)(&output_data[outrow]))));
    inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(420,13,"fullsize_smooth_downsample",(void *)(&input_data[0]),(void *)(&input_data[outrow]))));
    above_ptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(421,17,"fullsize_smooth_downsample",(void *)(&input_data[0]),(void *)(&input_data[outrow - 1]))));
    below_ptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(422,17,"fullsize_smooth_downsample",(void *)(&input_data[0]),(void *)(&input_data[outrow + 1]))));

    /* Special case for first column */
    colsum = GETJSAMPLE(*above_ptr++) + GETJSAMPLE(*below_ptr++) +
	     GETJSAMPLE(*inptr);
    membersum = GETJSAMPLE(*inptr++);
    nextcolsum = GETJSAMPLE(*above_ptr) + GETJSAMPLE(*below_ptr) +
		 GETJSAMPLE(*inptr);
    neighsum = colsum + (colsum - membersum) + nextcolsum;
    membersum = membersum * memberscale + neighsum * neighscale;
    *(JSAMPROW)(__boundcheck_ptr_reference(432,12,"fullsize_smooth_downsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((membersum + 32768) >> 16);
    lastcolsum = colsum; colsum = nextcolsum;

    for (colctr = output_cols - 2; colctr > 0; colctr--) {
      membersum = GETJSAMPLE(*inptr++);
      above_ptr++; below_ptr++;
      nextcolsum = GETJSAMPLE(*above_ptr) + GETJSAMPLE(*below_ptr) +
		   GETJSAMPLE(*inptr);
      neighsum = lastcolsum + (colsum - membersum) + nextcolsum;
      membersum = membersum * memberscale + neighsum * neighscale;
      *(JSAMPROW)(__boundcheck_ptr_reference(442,14,"fullsize_smooth_downsample",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) ((membersum + 32768) >> 16);
      lastcolsum = colsum; colsum = nextcolsum;
    }

    /* Special case for last column */
    membersum = GETJSAMPLE(*inptr);
    neighsum = lastcolsum + (colsum - membersum) + colsum;
    membersum = membersum * memberscale + neighsum * neighscale;
    *(JSAMPROW)(__boundcheck_ptr_reference(450,6,"fullsize_smooth_downsample",(void *)(outptr),(void *)(outptr))) = (JSAMPLE) ((membersum + 32768) >> 16);

  }
}

#endif /* INPUT_SMOOTHING_SUPPORTED */


/*
 * Module initialization routine for downsampling.
 * Note that we must select a routine for each component.
 */

GLOBAL(void)
jinit_downsampler (j_compress_ptr cinfo)
{
  my_downsample_ptr downsample;
__boundcheck_metadata_store((void *)(&downsample),(void *)((size_t)(&downsample)+sizeof(downsample)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  boolean smoothok = TRUE;
__boundcheck_metadata_store((void *)(&smoothok),(void *)((size_t)(&smoothok)+sizeof(smoothok)*8-1));


  downsample = (my_downsample_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(472,19,"jinit_downsampler",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_downsampler));
  cinfo->downsample = (struct jpeg_downsampler *) downsample;
  downsample->pub.start_pass = start_pass_downsample;
  downsample->pub.downsample = sep_downsample;
  downsample->pub.need_context_rows = FALSE;

  if (cinfo->CCIR601_sampling)
    ERREXIT(cinfo, JERR_CCIR601_NOTIMPL);

  /* Verify we can handle the sampling factors, and set up method pointers */
  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    if (compptr->h_samp_factor == cinfo->max_h_samp_factor &&
	compptr->v_samp_factor == cinfo->max_v_samp_factor) {
#ifdef INPUT_SMOOTHING_SUPPORTED
      if (cinfo->smoothing_factor) {
	downsample->methods[_RV_insert_check(0,10,489,2,"jinit_downsampler",ci)] = fullsize_smooth_downsample;
	downsample->pub.need_context_rows = TRUE;
      } else
#endif
	downsample->methods[_RV_insert_check(0,10,493,2,"jinit_downsampler",ci)] = fullsize_downsample;
    } else if (compptr->h_samp_factor * 2 == cinfo->max_h_samp_factor &&
	       compptr->v_samp_factor == cinfo->max_v_samp_factor) {
      smoothok = FALSE;
      downsample->methods[_RV_insert_check(0,10,497,7,"jinit_downsampler",ci)] = h2v1_downsample;
    } else if (compptr->h_samp_factor * 2 == cinfo->max_h_samp_factor &&
	       compptr->v_samp_factor * 2 == cinfo->max_v_samp_factor) {
#ifdef INPUT_SMOOTHING_SUPPORTED
      if (cinfo->smoothing_factor) {
	downsample->methods[_RV_insert_check(0,10,502,2,"jinit_downsampler",ci)] = h2v2_smooth_downsample;
	downsample->pub.need_context_rows = TRUE;
      } else
#endif
	downsample->methods[_RV_insert_check(0,10,506,2,"jinit_downsampler",ci)] = h2v2_downsample;
    } else if ((cinfo->max_h_samp_factor % compptr->h_samp_factor) == 0 &&
	       (cinfo->max_v_samp_factor % compptr->v_samp_factor) == 0) {
      smoothok = FALSE;
      downsample->methods[_RV_insert_check(0,10,510,7,"jinit_downsampler",ci)] = int_downsample;
    } else
      ERREXIT(cinfo, JERR_FRACT_SAMPLE_NOTIMPL);
  }

#ifdef INPUT_SMOOTHING_SUPPORTED
  if (cinfo->smoothing_factor && !smoothok)
    TRACEMS(cinfo, 0, JTRC_SMOOTH_NOTIMPL);
#endif
}
