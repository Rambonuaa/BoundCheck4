/*
 * jdmerge.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains code for merged upsampling/color conversion.
 *
 * This file combines functions from jdsample.c and jdcolor.c;
 * read those files first to understand what's going on.
 *
 * When the chroma components are to be upsampled by simple replication
 * (ie, box filtering), we can save some work in color conversion by
 * calculating all the output pixels corresponding to a pair of chroma
 * samples at one time.  In the conversion equations
 *	R = Y           + K1 * Cr
 *	G = Y + K2 * Cb + K3 * Cr
 *	B = Y + K4 * Cb
 * only the Y term varies among the group of pixels corresponding to a pair
 * of chroma samples, so the rest of the terms can be calculated just once.
 * At typical sampling ratios, this eliminates half or three-quarters of the
 * multiplications needed for color conversion.
 *
 * This file currently provides implementations for the following cases:
 *	YCbCr => RGB color conversion only.
 *	Sampling ratios of 2h1v or 2h2v.
 *	No scaling needed at upsample time.
 *	Corner-aligned (non-CCIR601) sampling alignment.
 * Other special cases could be added, but in most applications these are
 * the only common cases.  (For uncommon cases we fall back on the more
 * general code in jdsample.c and jdcolor.c.)
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"

#ifdef UPSAMPLE_MERGING_SUPPORTED


/* Private subobject */

typedef struct {
  struct jpeg_upsampler pub;	/* public fields */

  /* Pointer to routine to do actual upsampling/conversion of one row group */
  JMETHOD(void, upmethod, (j_decompress_ptr cinfo,
			   JSAMPIMAGE input_buf, JDIMENSION in_row_group_ctr,
			   JSAMPARRAY output_buf));

  /* Private state for YCC->RGB conversion */
  int * Cr_r_tab;		/* => table for Cr to R conversion */
  int * Cb_b_tab;		/* => table for Cb to B conversion */
  INT32 * Cr_g_tab;		/* => table for Cr to G conversion */
  INT32 * Cb_g_tab;		/* => table for Cb to G conversion */

  /* For 2:1 vertical sampling, we produce two output rows at a time.
   * We need a "spare" row buffer to hold the second output row if the
   * application provides just a one-row buffer; we also use the spare
   * to discard the dummy last row if the image height is odd.
   */
  JSAMPROW spare_row;
  boolean spare_full;		/* T if spare buffer is occupied */

  JDIMENSION out_row_width;	/* samples per output row */
  JDIMENSION rows_to_go;	/* counts rows remaining in image */
} my_upsampler;

typedef my_upsampler * my_upsample_ptr;

#define SCALEBITS	16	/* speediest right-shift on some machines */
#define ONE_HALF	((INT32) 1 << (SCALEBITS-1))
#define FIX(x)		((INT32) ((x) * (1L<<SCALEBITS) + 0.5))


/*
 * Initialize tables for YCC->RGB colorspace conversion.
 * This is taken directly from jdcolor.c; see that file for more info.
 */

LOCAL(void)
build_ycc_rgb_table (j_decompress_ptr cinfo)
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  INT32 x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));

  SHIFT_TEMPS

  upsample->Cr_r_tab = (int *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(91,19,"build_ycc_rgb_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(MAXJSAMPLE+1) * SIZEOF(int));
  upsample->Cb_b_tab = (int *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(94,19,"build_ycc_rgb_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(MAXJSAMPLE+1) * SIZEOF(int));
  upsample->Cr_g_tab = (INT32 *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(97,19,"build_ycc_rgb_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(MAXJSAMPLE+1) * SIZEOF(INT32));
  upsample->Cb_g_tab = (INT32 *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(100,19,"build_ycc_rgb_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				(MAXJSAMPLE+1) * SIZEOF(INT32));

  for (i = 0, x = -CENTERJSAMPLE; i <= MAXJSAMPLE; i++, x++) {
    /* i is the actual input pixel value, in the range 0..MAXJSAMPLE */
    /* The Cb or Cr value we are thinking of is x = i - CENTERJSAMPLE */
    /* Cr=>R value is nearest int to 1.40200 * x */
    upsample->Cr_r_tab[i] = (int)
		    RIGHT_SHIFT(FIX(1.40200) * x + ONE_HALF, SCALEBITS);
    /* Cb=>B value is nearest int to 1.77200 * x */
    upsample->Cb_b_tab[i] = (int)
		    RIGHT_SHIFT(FIX(1.77200) * x + ONE_HALF, SCALEBITS);
    /* Cr=>G value is scaled-up -0.71414 * x */
    upsample->Cr_g_tab[i] = (- FIX(0.71414)) * x;
    /* Cb=>G value is scaled-up -0.34414 * x */
    /* We also add in ONE_HALF so that need not do it in inner loop */
    upsample->Cb_g_tab[i] = (- FIX(0.34414)) * x + ONE_HALF;
  }
}


/*
 * Initialize for an upsampling pass.
 */

METHODDEF(void)
start_pass_merged_upsample (j_decompress_ptr cinfo)
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));


  /* Mark the spare buffer empty */
  upsample->spare_full = FALSE;
  /* Initialize total-height counter for detecting bottom of image */
  upsample->rows_to_go = cinfo->output_height;
}


/*
 * Control routine to do upsampling (and color conversion).
 *
 * The control routine just handles the row buffering considerations.
 */

METHODDEF(void)
merged_2v_upsample (j_decompress_ptr cinfo,
		    JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
		    JDIMENSION in_row_groups_avail,
		    JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
		    JDIMENSION out_rows_avail)
/* 2:1 vertical sampling case: may need a spare row. */
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));

  JSAMPROW work_ptrs[2];__boundcheck_metadata_store(&work_ptrs[0],&work_ptrs[2-1]);

  JDIMENSION num_rows;
__boundcheck_metadata_store((void *)(&num_rows),(void *)((size_t)(&num_rows)+sizeof(num_rows)*8-1));
		/* number of rows returned to caller */

  if (upsample->spare_full) {
    /* If we have a spare row saved from a previous cycle, just return it. */
    jcopy_sample_rows(& upsample->spare_row, 0, output_buf + *(JDIMENSION *)(__boundcheck_ptr_reference(157,63,"merged_2v_upsample",(void *)(out_row_ctr),(void *)(out_row_ctr))), 0,
		      1, upsample->out_row_width);
    num_rows = 1;
    upsample->spare_full = FALSE;
  } else {
    /* Figure number of rows to return to caller. */
    num_rows = 2;
    /* Not more than the distance to the end of the image. */
    if (num_rows > upsample->rows_to_go)
      num_rows = upsample->rows_to_go;
    /* And not more than what the client can accept: */
    out_rows_avail -= *(JDIMENSION *)(__boundcheck_ptr_reference(168,24,"merged_2v_upsample",(void *)(out_row_ctr),(void *)(out_row_ctr)));
    if (num_rows > out_rows_avail)
      num_rows = out_rows_avail;
    /* Create output pointer array for upsampler. */
    work_ptrs[_RV_insert_check(0,2,172,5,"merged_2v_upsample",0)] = (*(JSAMPROW *)(__boundcheck_ptr_reference(172,20,"merged_2v_upsample",(void *)(&output_buf[0]),(void *)(&output_buf[*out_(JDIMENSION *)(__boundcheck_ptr_reference(172,32,"merged_2v_upsample",(void *)(out_row_ctr),(void *)(row_ctr])))))));
    if (num_rows > 1) {
      work_ptrs[_RV_insert_check(0,2,174,7,"merged_2v_upsample",1)] = (*(JSAMPROW *)(__boundcheck_ptr_reference(174,22,"merged_2v_upsample",(void *)(&output_buf[0]),(void *)(&output_buf[*out_(JDIMENSION *)(__boundcheck_ptr_reference(174,34,"merged_2v_upsample",(void *)(out_row_ctr),(void *)(row_ctr + 1)))]))));
    } else {
      work_ptrs[_RV_insert_check(0,2,176,7,"merged_2v_upsample",1)] = upsample->spare_row;
      upsample->spare_full = TRUE;
    }
    /* Now do the upsampling. */
    (*(void (*)(j_decompress_ptr, JSAMPIMAGE, JDIMENSION, JSAMPARRAY))(__boundcheck_ptr_reference(180,17,"merged_2v_upsample",(void *)(upsample->upmethod),(void *)upsample->upmethod))) (cinfo, input_buf, *(JDIMENSION *)(__boundcheck_ptr_reference(180,47,"merged_2v_upsample",(void *)(in_row_group_ctr),(void *)(in_row_group_ctr))), work_ptrs);
  }

  /* Adjust counts */
  *(JDIMENSION *)(__boundcheck_ptr_reference(184,4,"merged_2v_upsample",(void *)(out_row_ctr),(void *)(out_row_ctr))) += num_rows;
  upsample->rows_to_go -= num_rows;
  /* When the buffer is emptied, declare this input row group consumed */
  if (! upsample->spare_full)
    (*(JDIMENSION *)(__boundcheck_ptr_reference(188,7,"merged_2v_upsample",(void *)(in_row_group_ctr),(void *)(in_row_group_ctr))))++;
}


METHODDEF(void)
merged_1v_upsample (j_decompress_ptr cinfo,
		    JSAMPIMAGE input_buf, JDIMENSION *in_row_group_ctr,
		    JDIMENSION in_row_groups_avail,
		    JSAMPARRAY output_buf, JDIMENSION *out_row_ctr,
		    JDIMENSION out_rows_avail)
/* 1:1 vertical sampling case: much easier, never need a spare row. */
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));


  /* Just do the upsampling. */
  (*(void (*)(j_decompress_ptr, JSAMPIMAGE, JDIMENSION, JSAMPARRAY))(__boundcheck_ptr_reference(203,15,"merged_1v_upsample",(void *)(upsample->upmethod),(void *)upsample->upmethod))) (cinfo, input_buf, *(JDIMENSION *)(__boundcheck_ptr_reference(203,45,"merged_1v_upsample",(void *)(in_row_group_ctr),(void *)(in_row_group_ctr))),
			 output_buf + *(JDIMENSION *)(__boundcheck_ptr_reference(204,19,"merged_1v_upsample",(void *)(out_row_ctr),(void *)(out_row_ctr))));
  /* Adjust counts */
  (*(JDIMENSION *)(__boundcheck_ptr_reference(206,5,"merged_1v_upsample",(void *)(out_row_ctr),(void *)(out_row_ctr))))++;
  (*(JDIMENSION *)(__boundcheck_ptr_reference(207,5,"merged_1v_upsample",(void *)(in_row_group_ctr),(void *)(in_row_group_ctr))))++;
}


/*
 * These are the routines invoked by the control routines to do
 * the actual upsampling/conversion.  One row group is processed per call.
 *
 * Note: since we may be writing directly into application-supplied buffers,
 * we have to be honest about the output width; we can't assume the buffer
 * has been rounded up to an even width.
 */


/*
 * Upsample and color convert for the case of 2:1 horizontal and 1:1 vertical.
 */

METHODDEF(void)
h2v1_merged_upsample (j_decompress_ptr cinfo,
		      JSAMPIMAGE input_buf, JDIMENSION in_row_group_ctr,
		      JSAMPARRAY output_buf)
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));

  register int y, cred, cgreen, cblue;
  int cb;
__boundcheck_metadata_store((void *)(&cb),(void *)((size_t)(&cb)+sizeof(cb)*8-1));
int  cr;
__boundcheck_metadata_store((void *)(&cr),(void *)((size_t)(&cr)+sizeof(cr)*8-1));

  register JSAMPROW outptr;
  JSAMPROW inptr0;
__boundcheck_metadata_store((void *)(&inptr0),(void *)((size_t)(&inptr0)+sizeof(inptr0)*8-1));
JSAMPROW  inptr1;
__boundcheck_metadata_store((void *)(&inptr1),(void *)((size_t)(&inptr1)+sizeof(inptr1)*8-1));
JSAMPROW  inptr2;
__boundcheck_metadata_store((void *)(&inptr2),(void *)((size_t)(&inptr2)+sizeof(inptr2)*8-1));

  JDIMENSION col;
__boundcheck_metadata_store((void *)(&col),(void *)((size_t)(&col)+sizeof(col)*8-1));

  /* copy these pointers into registers if possible */
  register JSAMPLE * range_limit = cinfo->sample_range_limit;
  int * Crrtab = upsample->Cr_r_tab;
__boundcheck_metadata_store((void *)(&Crrtab),(void *)((size_t)(&Crrtab)+sizeof(Crrtab)*8-1));

  int * Cbbtab = upsample->Cb_b_tab;
__boundcheck_metadata_store((void *)(&Cbbtab),(void *)((size_t)(&Cbbtab)+sizeof(Cbbtab)*8-1));

  INT32 * Crgtab = upsample->Cr_g_tab;
__boundcheck_metadata_store((void *)(&Crgtab),(void *)((size_t)(&Crgtab)+sizeof(Crgtab)*8-1));

  INT32 * Cbgtab = upsample->Cb_g_tab;
__boundcheck_metadata_store((void *)(&Cbgtab),(void *)((size_t)(&Cbgtab)+sizeof(Cbgtab)*8-1));

  SHIFT_TEMPS

  inptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(244,12,"h2v1_merged_upsample",(void *)(&input_buf[0][0]),(void *)(&input_buf[0][in_row_group_ctr]))));
  inptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(245,12,"h2v1_merged_upsample",(void *)(&input_buf[1][0]),(void *)(&input_buf[1][in_row_group_ctr]))));
  inptr2 = (*(JSAMPROW *)(__boundcheck_ptr_reference(246,12,"h2v1_merged_upsample",(void *)(&input_buf[2][0]),(void *)(&input_buf[2][in_row_group_ctr]))));
  outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(247,12,"h2v1_merged_upsample",(void *)(&output_buf[0]),(void *)(&output_buf[0]))));
  /* Loop for each pair of output pixels */
  for (col = cinfo->output_width >> 1; col > 0; col--) {
    /* Do the chroma part of the calculation */
    cb = GETJSAMPLE(*inptr1++);
    cr = GETJSAMPLE(*inptr2++);
    cred = (*(int *)(__boundcheck_ptr_reference(253,12,"h2v1_merged_upsample",(void *)(&Crrtab[0]),(void *)(&Crrtab[cr]))));
    cgreen = (int) RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr], SCALEBITS);
    cblue = (*(int *)(__boundcheck_ptr_reference(255,13,"h2v1_merged_upsample",(void *)(&Cbbtab[0]),(void *)(&Cbbtab[cb]))));
    /* Fetch 2 Y values and emit 2 pixels */
    y  = GETJSAMPLE(*inptr0++);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(258,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(258,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(259,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(259,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(260,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(260,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
    outptr += RGB_PIXELSIZE;
    y  = GETJSAMPLE(*inptr0++);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(263,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(263,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(264,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(264,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(265,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(265,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
    outptr += RGB_PIXELSIZE;
  }
  /* If image width is odd, do the last output column separately */
  if (cinfo->output_width & 1) {
    cb = GETJSAMPLE(*inptr1);
    cr = GETJSAMPLE(*inptr2);
    cred = (*(int *)(__boundcheck_ptr_reference(272,12,"h2v1_merged_upsample",(void *)(&Crrtab[0]),(void *)(&Crrtab[cr]))));
    cgreen = (int) RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr], SCALEBITS);
    cblue = (*(int *)(__boundcheck_ptr_reference(274,13,"h2v1_merged_upsample",(void *)(&Cbbtab[0]),(void *)(&Cbbtab[cb]))));
    y  = GETJSAMPLE(*inptr0);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(276,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(276,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(277,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(277,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(278,5,"h2v1_merged_upsample",(void *)(&outptr[0]),(void *)(&outptr[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(278,25,"h2v1_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
  }
}


/*
 * Upsample and color convert for the case of 2:1 horizontal and 2:1 vertical.
 */

METHODDEF(void)
h2v2_merged_upsample (j_decompress_ptr cinfo,
		      JSAMPIMAGE input_buf, JDIMENSION in_row_group_ctr,
		      JSAMPARRAY output_buf)
{
  my_upsample_ptr upsample = (my_upsample_ptr) cinfo->upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));

  register int y, cred, cgreen, cblue;
  int cb;
__boundcheck_metadata_store((void *)(&cb),(void *)((size_t)(&cb)+sizeof(cb)*8-1));
int  cr;
__boundcheck_metadata_store((void *)(&cr),(void *)((size_t)(&cr)+sizeof(cr)*8-1));

  register JSAMPROW outptr0, outptr1;
  JSAMPROW inptr00;
__boundcheck_metadata_store((void *)(&inptr00),(void *)((size_t)(&inptr00)+sizeof(inptr00)*8-1));
JSAMPROW  inptr01;
__boundcheck_metadata_store((void *)(&inptr01),(void *)((size_t)(&inptr01)+sizeof(inptr01)*8-1));
JSAMPROW  inptr1;
__boundcheck_metadata_store((void *)(&inptr1),(void *)((size_t)(&inptr1)+sizeof(inptr1)*8-1));
JSAMPROW  inptr2;
__boundcheck_metadata_store((void *)(&inptr2),(void *)((size_t)(&inptr2)+sizeof(inptr2)*8-1));

  JDIMENSION col;
__boundcheck_metadata_store((void *)(&col),(void *)((size_t)(&col)+sizeof(col)*8-1));

  /* copy these pointers into registers if possible */
  register JSAMPLE * range_limit = cinfo->sample_range_limit;
  int * Crrtab = upsample->Cr_r_tab;
__boundcheck_metadata_store((void *)(&Crrtab),(void *)((size_t)(&Crrtab)+sizeof(Crrtab)*8-1));

  int * Cbbtab = upsample->Cb_b_tab;
__boundcheck_metadata_store((void *)(&Cbbtab),(void *)((size_t)(&Cbbtab)+sizeof(Cbbtab)*8-1));

  INT32 * Crgtab = upsample->Cr_g_tab;
__boundcheck_metadata_store((void *)(&Crgtab),(void *)((size_t)(&Crgtab)+sizeof(Crgtab)*8-1));

  INT32 * Cbgtab = upsample->Cb_g_tab;
__boundcheck_metadata_store((void *)(&Cbgtab),(void *)((size_t)(&Cbgtab)+sizeof(Cbgtab)*8-1));

  SHIFT_TEMPS

  inptr00 = (*(JSAMPROW *)(__boundcheck_ptr_reference(306,13,"h2v2_merged_upsample",(void *)(&input_buf[0][0]),(void *)(&input_buf[0][in_row_group_ctr * 2]))));
  inptr01 = (*(JSAMPROW *)(__boundcheck_ptr_reference(307,13,"h2v2_merged_upsample",(void *)(&input_buf[0][0]),(void *)(&input_buf[0][in_row_group_ctr * 2 + 1]))));
  inptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(308,12,"h2v2_merged_upsample",(void *)(&input_buf[1][0]),(void *)(&input_buf[1][in_row_group_ctr]))));
  inptr2 = (*(JSAMPROW *)(__boundcheck_ptr_reference(309,12,"h2v2_merged_upsample",(void *)(&input_buf[2][0]),(void *)(&input_buf[2][in_row_group_ctr]))));
  outptr0 = (*(JSAMPROW *)(__boundcheck_ptr_reference(310,13,"h2v2_merged_upsample",(void *)(&output_buf[0]),(void *)(&output_buf[0]))));
  outptr1 = (*(JSAMPROW *)(__boundcheck_ptr_reference(311,13,"h2v2_merged_upsample",(void *)(&output_buf[0]),(void *)(&output_buf[1]))));
  /* Loop for each group of output pixels */
  for (col = cinfo->output_width >> 1; col > 0; col--) {
    /* Do the chroma part of the calculation */
    cb = GETJSAMPLE(*inptr1++);
    cr = GETJSAMPLE(*inptr2++);
    cred = (*(int *)(__boundcheck_ptr_reference(317,12,"h2v2_merged_upsample",(void *)(&Crrtab[0]),(void *)(&Crrtab[cr]))));
    cgreen = (int) RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr], SCALEBITS);
    cblue = (*(int *)(__boundcheck_ptr_reference(319,13,"h2v2_merged_upsample",(void *)(&Cbbtab[0]),(void *)(&Cbbtab[cb]))));
    /* Fetch 4 Y values and emit 4 pixels */
    y  = GETJSAMPLE(*inptr00++);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(322,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(322,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(323,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(323,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(324,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(324,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
    outptr0 += RGB_PIXELSIZE;
    y  = GETJSAMPLE(*inptr00++);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(327,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(327,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(328,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(328,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(329,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(329,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
    outptr0 += RGB_PIXELSIZE;
    y  = GETJSAMPLE(*inptr01++);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(332,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(332,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(333,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(333,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(334,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(334,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
    outptr1 += RGB_PIXELSIZE;
    y  = GETJSAMPLE(*inptr01++);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(337,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(337,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(338,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(338,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(339,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(339,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
    outptr1 += RGB_PIXELSIZE;
  }
  /* If image width is odd, do the last output column separately */
  if (cinfo->output_width & 1) {
    cb = GETJSAMPLE(*inptr1);
    cr = GETJSAMPLE(*inptr2);
    cred = (*(int *)(__boundcheck_ptr_reference(346,12,"h2v2_merged_upsample",(void *)(&Crrtab[0]),(void *)(&Crrtab[cr]))));
    cgreen = (int) RIGHT_SHIFT(Cbgtab[cb] + Crgtab[cr], SCALEBITS);
    cblue = (*(int *)(__boundcheck_ptr_reference(348,13,"h2v2_merged_upsample",(void *)(&Cbbtab[0]),(void *)(&Cbbtab[cb]))));
    y  = GETJSAMPLE(*inptr00);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(350,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(350,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(351,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(351,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(352,5,"h2v2_merged_upsample",(void *)(&outptr0[0]),(void *)(&outptr0[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(352,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
    y  = GETJSAMPLE(*inptr01);
    (*(JSAMPLE *)(__boundcheck_ptr_reference(354,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[0])))) =   (*(JSAMPLE *)(__boundcheck_ptr_reference(354,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cred]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(355,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(355,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cgreen]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(356,5,"h2v2_merged_upsample",(void *)(&outptr1[0]),(void *)(&outptr1[2])))) =  (*(JSAMPLE *)(__boundcheck_ptr_reference(356,26,"h2v2_merged_upsample",(void *)(&range_limit[0]),(void *)(&range_limit[y + cblue]))));
  }
}


/*
 * Module initialization routine for merged upsampling/color conversion.
 *
 * NB: this is called under the conditions determined by use_merged_upsample()
 * in jdmaster.c.  That routine MUST correspond to the actual capabilities
 * of this module; no safety checks are made here.
 */

GLOBAL(void)
jinit_merged_upsampler (j_decompress_ptr cinfo)
{
  my_upsample_ptr upsample;
__boundcheck_metadata_store((void *)(&upsample),(void *)((size_t)(&upsample)+sizeof(upsample)*8-1));


  upsample = (my_upsample_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(375,19,"jinit_merged_upsampler",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_upsampler));
  cinfo->upsample = (struct jpeg_upsampler *) upsample;
  upsample->pub.start_pass = start_pass_merged_upsample;
  upsample->pub.need_context_rows = FALSE;

  upsample->out_row_width = cinfo->output_width * cinfo->out_color_components;

  if (cinfo->max_v_samp_factor == 2) {
    upsample->pub.upsample = merged_2v_upsample;
    upsample->upmethod = h2v2_merged_upsample;
    /* Allocate a spare row buffer */
    upsample->spare_row = (JSAMPROW)
      (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(388,21,"jinit_merged_upsampler",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
		(size_t) (upsample->out_row_width * SIZEOF(JSAMPLE)));
  } else {
    upsample->pub.upsample = merged_1v_upsample;
    upsample->upmethod = h2v1_merged_upsample;
    /* No spare row needed */
    upsample->spare_row = NULL;
  }

  build_ycc_rgb_table(cinfo);
}

#endif /* UPSAMPLE_MERGING_SUPPORTED */
