/*
 * jdcoefct.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains the coefficient buffer controller for decompression.
 * This controller is the top level of the JPEG decompressor proper.
 * The coefficient buffer lies between entropy decoding and inverse-DCT steps.
 *
 * In buffered-image mode, this controller is the interface between
 * input-oriented processing and output-oriented processing.
 * Also, the input side (only) is used when reading a file for transcoding.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"

/* Block smoothing is only applicable for progressive JPEG, so: */
#ifndef D_PROGRESSIVE_SUPPORTED
#undef BLOCK_SMOOTHING_SUPPORTED
#endif

/* Private buffer controller object */

typedef struct {
  struct jpeg_d_coef_controller pub; /* public fields */

  /* These variables keep track of the current location of the input side. */
  /* cinfo->input_iMCU_row is also used for this. */
  JDIMENSION MCU_ctr;		/* counts MCUs processed in current row */
  int MCU_vert_offset;		/* counts MCU rows within iMCU row */
  int MCU_rows_per_iMCU_row;	/* number of such rows needed */

  /* The output side's location is represented by cinfo->output_iMCU_row. */

  /* In single-pass modes, it's sufficient to buffer just one MCU.
   * We allocate a workspace of D_MAX_BLOCKS_IN_MCU coefficient blocks,
   * and let the entropy decoder write into that workspace each time.
   * (On 80x86, the workspace is FAR even though it's not really very big;
   * this is to keep the module interfaces unchanged when a large coefficient
   * buffer is necessary.)
   * In multi-pass modes, this array points to the current MCU's blocks
   * within the virtual arrays; it is used only by the input side.
   */
  JBLOCKROW MCU_buffer[D_MAX_BLOCKS_IN_MCU];

#ifdef D_MULTISCAN_FILES_SUPPORTED
  /* In multi-pass modes, we need a virtual block array for each component. */
  jvirt_barray_ptr whole_image[MAX_COMPONENTS];
#endif

#ifdef BLOCK_SMOOTHING_SUPPORTED
  /* When doing block smoothing, we latch coefficient Al values here */
  int * coef_bits_latch;
#define SAVED_COEFS  6		/* we save coef_bits[0..5] */
#endif
} my_coef_controller;

typedef my_coef_controller * my_coef_ptr;

/* Forward declarations */
METHODDEF(int) decompress_onepass
	JPP((j_decompress_ptr cinfo, JSAMPIMAGE output_buf));
#ifdef D_MULTISCAN_FILES_SUPPORTED
METHODDEF(int) decompress_data
	JPP((j_decompress_ptr cinfo, JSAMPIMAGE output_buf));
#endif
#ifdef BLOCK_SMOOTHING_SUPPORTED
LOCAL(boolean) smoothing_ok JPP((j_decompress_ptr cinfo));
METHODDEF(int) decompress_smooth_data
	JPP((j_decompress_ptr cinfo, JSAMPIMAGE output_buf));
#endif


LOCAL(void)
start_iMCU_row (j_decompress_ptr cinfo)
/* Reset within-iMCU-row counters for a new row (input side) */
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));


  /* In an interleaved scan, an MCU row is the same as an iMCU row.
   * In a noninterleaved scan, an iMCU row has v_samp_factor MCU rows.
   * But at the bottom of the image, process only what's left.
   */
  if (cinfo->comps_in_scan > 1) {
    coef->MCU_rows_per_iMCU_row = 1;
  } else {
    if (cinfo->input_iMCU_row < (cinfo->total_iMCU_rows-1))
      coef->MCU_rows_per_iMCU_row = cinfo->cur_comp_info[_RV_insert_check(0,4,92,37,"start_iMCU_row",0)]->v_samp_factor;
    else
      coef->MCU_rows_per_iMCU_row = cinfo->cur_comp_info[_RV_insert_check(0,4,94,37,"start_iMCU_row",0)]->last_row_height;
  }

  coef->MCU_ctr = 0;
  coef->MCU_vert_offset = 0;
}


/*
 * Initialize for an input processing pass.
 */

METHODDEF(void)
start_input_pass (j_decompress_ptr cinfo)
{
  cinfo->input_iMCU_row = 0;
  start_iMCU_row(cinfo);
}


/*
 * Initialize for an output processing pass.
 */

METHODDEF(void)
start_output_pass (j_decompress_ptr cinfo)
{
#ifdef BLOCK_SMOOTHING_SUPPORTED
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));


  /* If multipass, check to see whether to use block smoothing on this pass */
  if (coef->pub.coef_arrays != NULL) {
    if (cinfo->do_block_smoothing && smoothing_ok(cinfo))
      coef->pub.decompress_data = decompress_smooth_data;
    else
      coef->pub.decompress_data = decompress_data;
  }
#endif
  cinfo->output_iMCU_row = 0;
}


/*
 * Decompress and return some data in the single-pass case.
 * Always attempts to emit one fully interleaved MCU row ("iMCU" row).
 * Input and output must run in lockstep since we have only a one-MCU buffer.
 * Return value is JPEG_ROW_COMPLETED, JPEG_SCAN_COMPLETED, or JPEG_SUSPENDED.
 *
 * NB: output_buf contains a plane for each component in image.
 * For single pass, this is the same as the components in the scan.
 */

METHODDEF(int)
decompress_onepass (j_decompress_ptr cinfo, JSAMPIMAGE output_buf)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));

  JDIMENSION MCU_col_num;
__boundcheck_metadata_store((void *)(&MCU_col_num),(void *)((size_t)(&MCU_col_num)+sizeof(MCU_col_num)*8-1));
	/* index of current MCU within row */
  JDIMENSION last_MCU_col = cinfo->MCUs_per_row - 1;
__boundcheck_metadata_store((void *)(&last_MCU_col),(void *)((size_t)(&last_MCU_col)+sizeof(last_MCU_col)*8-1));

  JDIMENSION last_iMCU_row = cinfo->total_iMCU_rows - 1;
__boundcheck_metadata_store((void *)(&last_iMCU_row),(void *)((size_t)(&last_iMCU_row)+sizeof(last_iMCU_row)*8-1));

  int blkn;
__boundcheck_metadata_store((void *)(&blkn),(void *)((size_t)(&blkn)+sizeof(blkn)*8-1));
int  ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  xindex;
__boundcheck_metadata_store((void *)(&xindex),(void *)((size_t)(&xindex)+sizeof(xindex)*8-1));
int  yindex;
__boundcheck_metadata_store((void *)(&yindex),(void *)((size_t)(&yindex)+sizeof(yindex)*8-1));
int  yoffset;
__boundcheck_metadata_store((void *)(&yoffset),(void *)((size_t)(&yoffset)+sizeof(yoffset)*8-1));
int  useful_width;
__boundcheck_metadata_store((void *)(&useful_width),(void *)((size_t)(&useful_width)+sizeof(useful_width)*8-1));

  JSAMPARRAY output_ptr;
__boundcheck_metadata_store((void *)(&output_ptr),(void *)((size_t)(&output_ptr)+sizeof(output_ptr)*8-1));

  JDIMENSION start_col;
__boundcheck_metadata_store((void *)(&start_col),(void *)((size_t)(&start_col)+sizeof(start_col)*8-1));
JDIMENSION  output_col;
__boundcheck_metadata_store((void *)(&output_col),(void *)((size_t)(&output_col)+sizeof(output_col)*8-1));

  jpeg_component_info *compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  inverse_DCT_method_ptr inverse_DCT;
__boundcheck_metadata_store((void *)(&inverse_DCT),(void *)((size_t)(&inverse_DCT)+sizeof(inverse_DCT)*8-1));


  /* Loop to process as much as one whole iMCU row */
  for (yoffset = coef->MCU_vert_offset; yoffset < coef->MCU_rows_per_iMCU_row;
       yoffset++) {
    for (MCU_col_num = coef->MCU_ctr; MCU_col_num <= last_MCU_col;
	 MCU_col_num++) {
      /* Try to fetch an MCU.  Entropy decoder expects buffer to be zeroed. */
      jzero_far((void FAR *) coef->MCU_buffer[_RV_insert_check(0,10,165,30,"decompress_onepass",0)],
		(size_t) (cinfo->blocks_in_MCU * SIZEOF(JBLOCK)));
      if (! (*(boolean (*)(j_decompress_ptr, JBLOCKROW *))(__boundcheck_ptr_reference(167,31,"decompress_onepass",(void *)(cinfo->entropy->decode_mcu),(void *)cinfo->entropy->decode_mcu))) (cinfo, coef->MCU_buffer)) {
	/* Suspension forced; update state counters and exit */
	coef->MCU_vert_offset = yoffset;
	coef->MCU_ctr = MCU_col_num;
	return JPEG_SUSPENDED;
      }
      /* Determine where data should go in output_buf and do the IDCT thing.
       * We skip dummy blocks at the right and bottom edges (but blkn gets
       * incremented past them!).  Note the inner loop relies on having
       * allocated the MCU_buffer[] blocks sequentially.
       */
      blkn = 0;			/* index of current DCT block within MCU */
      for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
	compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,180,12,"decompress_onepass",ci)];
	/* Don't bother to IDCT an uninteresting component. */
	if (! compptr->component_needed) {
	  blkn += compptr->MCU_blocks;
	  continue;
	}
	inverse_DCT = cinfo->idct->inverse_DCT[_RV_insert_check(0,10,186,16,"decompress_onepass",compptr->component_index)];
	useful_width = (MCU_col_num < last_MCU_col) ? compptr->MCU_width
						    : compptr->last_col_width;
	output_ptr = (*(JSAMPARRAY *)(__boundcheck_ptr_reference(189,15,"decompress_onepass",(void *)(&output_buf[0]),(void *)(&output_buf[ci])))) + yoffset * compptr->DCT_scaled_size;
	start_col = MCU_col_num * compptr->MCU_sample_width;
	for (yindex = 0; yindex < compptr->MCU_height; yindex++) {
	  if (cinfo->input_iMCU_row < last_iMCU_row ||
	      yoffset+yindex < compptr->last_row_height) {
	    output_col = start_col;
	    for (xindex = 0; xindex < useful_width; xindex++) {
	      (*(inverse_DCT_method_ptr)(__boundcheck_ptr_reference(196,10,"decompress_onepass",(void *)(inverse_DCT),(void *)(inverse_DCT)))) (cinfo, compptr,
			      (JCOEFPTR) coef->MCU_buffer[_RV_insert_check(0,10,197,21,"decompress_onepass",blkn+xindex)],
			      output_ptr, output_col);
	      output_col += compptr->DCT_scaled_size;
	    }
	  }
	  blkn += compptr->MCU_width;
	  output_ptr += compptr->DCT_scaled_size;
	}
      }
    }
    /* Completed an MCU row, but perhaps not an iMCU row */
    coef->MCU_ctr = 0;
  }
  /* Completed the iMCU row, advance counters for next one */
  cinfo->output_iMCU_row++;
  if (++(cinfo->input_iMCU_row) < cinfo->total_iMCU_rows) {
    start_iMCU_row(cinfo);
    return JPEG_ROW_COMPLETED;
  }
  /* Completed the scan */
  (*(void (*)(j_decompress_ptr))(__boundcheck_ptr_reference(217,22,"decompress_onepass",(void *)(cinfo->inputctl->finish_input_pass),(void *)cinfo->inputctl->finish_input_pass))) (cinfo);
  return JPEG_SCAN_COMPLETED;
}


/*
 * Dummy consume-input routine for single-pass operation.
 */

METHODDEF(int)
dummy_consume_data (j_decompress_ptr cinfo)
{
  return JPEG_SUSPENDED;	/* Always indicate nothing was done */
}


#ifdef D_MULTISCAN_FILES_SUPPORTED

/*
 * Consume input data and store it in the full-image coefficient buffer.
 * We read as much as one fully interleaved MCU row ("iMCU" row) per call,
 * ie, v_samp_factor block rows for each component in the scan.
 * Return value is JPEG_ROW_COMPLETED, JPEG_SCAN_COMPLETED, or JPEG_SUSPENDED.
 */

METHODDEF(int)
consume_data (j_decompress_ptr cinfo)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));

  JDIMENSION MCU_col_num;
__boundcheck_metadata_store((void *)(&MCU_col_num),(void *)((size_t)(&MCU_col_num)+sizeof(MCU_col_num)*8-1));
	/* index of current MCU within row */
  int blkn;
__boundcheck_metadata_store((void *)(&blkn),(void *)((size_t)(&blkn)+sizeof(blkn)*8-1));
int  ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  xindex;
__boundcheck_metadata_store((void *)(&xindex),(void *)((size_t)(&xindex)+sizeof(xindex)*8-1));
int  yindex;
__boundcheck_metadata_store((void *)(&yindex),(void *)((size_t)(&yindex)+sizeof(yindex)*8-1));
int  yoffset;
__boundcheck_metadata_store((void *)(&yoffset),(void *)((size_t)(&yoffset)+sizeof(yoffset)*8-1));

  JDIMENSION start_col;
__boundcheck_metadata_store((void *)(&start_col),(void *)((size_t)(&start_col)+sizeof(start_col)*8-1));

  JBLOCKARRAY buffer[MAX_COMPS_IN_SCAN];__boundcheck_metadata_store(&buffer[0],&buffer[4-1]);

  JBLOCKROW buffer_ptr;
__boundcheck_metadata_store((void *)(&buffer_ptr),(void *)((size_t)(&buffer_ptr)+sizeof(buffer_ptr)*8-1));

  jpeg_component_info *compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));


  /* Align the virtual buffers for the components used in this scan. */
  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,255,15,"consume_data",ci)];
    buffer[_RV_insert_check(0,4,256,5,"consume_data",ci)] = (*(JBLOCKARRAY (*)(j_common_ptr, jvirt_barray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(256,32,"consume_data",(void *)(cinfo->mem->access_virt_barray),(void *)cinfo->mem->access_virt_barray)))
      ((j_common_ptr) cinfo, coef->whole_image[_RV_insert_check(0,10,257,30,"consume_data",compptr->component_index)],
       cinfo->input_iMCU_row * compptr->v_samp_factor,
       (JDIMENSION) compptr->v_samp_factor, TRUE);
    /* Note: entropy decoder expects buffer to be zeroed,
     * but this is handled automatically by the memory manager
     * because we requested a pre-zeroed array.
     */
  }

  /* Loop to process one whole iMCU row */
  for (yoffset = coef->MCU_vert_offset; yoffset < coef->MCU_rows_per_iMCU_row;
       yoffset++) {
    for (MCU_col_num = coef->MCU_ctr; MCU_col_num < cinfo->MCUs_per_row;
	 MCU_col_num++) {
      /* Construct list of pointers to DCT blocks belonging to this MCU */
      blkn = 0;			/* index of current DCT block within MCU */
      for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
	compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,274,12,"consume_data",ci)];
	start_col = MCU_col_num * compptr->MCU_width;
	for (yindex = 0; yindex < compptr->MCU_height; yindex++) {
	  buffer_ptr = (*(JBLOCKROW *)(__boundcheck_ptr_reference(277,42,"consume_data",(void *)(&buffer[ci][0]),(void *)(&buffer[ci][yi_RV_insert_check(0,4,277,17,"consume_data",nd)ex + yoffset])))) + start_col;
	  for (xindex = 0; xindex < compptr->MCU_width; xindex++) {
	    coef->MCU_buffer[_RV_insert_check(0,10,279,6,"consume_data",blkn++)] = buffer_ptr++;
	  }
	}
      }
      /* Try to fetch the MCU. */
      if (! (*(boolean (*)(j_decompress_ptr, JBLOCKROW *))(__boundcheck_ptr_reference(284,31,"consume_data",(void *)(cinfo->entropy->decode_mcu),(void *)cinfo->entropy->decode_mcu))) (cinfo, coef->MCU_buffer)) {
	/* Suspension forced; update state counters and exit */
	coef->MCU_vert_offset = yoffset;
	coef->MCU_ctr = MCU_col_num;
	return JPEG_SUSPENDED;
      }
    }
    /* Completed an MCU row, but perhaps not an iMCU row */
    coef->MCU_ctr = 0;
  }
  /* Completed the iMCU row, advance counters for next one */
  if (++(cinfo->input_iMCU_row) < cinfo->total_iMCU_rows) {
    start_iMCU_row(cinfo);
    return JPEG_ROW_COMPLETED;
  }
  /* Completed the scan */
  (*(void (*)(j_decompress_ptr))(__boundcheck_ptr_reference(300,22,"consume_data",(void *)(cinfo->inputctl->finish_input_pass),(void *)cinfo->inputctl->finish_input_pass))) (cinfo);
  return JPEG_SCAN_COMPLETED;
}


/*
 * Decompress and return some data in the multi-pass case.
 * Always attempts to emit one fully interleaved MCU row ("iMCU" row).
 * Return value is JPEG_ROW_COMPLETED, JPEG_SCAN_COMPLETED, or JPEG_SUSPENDED.
 *
 * NB: output_buf contains a plane for each component in image.
 */

METHODDEF(int)
decompress_data (j_decompress_ptr cinfo, JSAMPIMAGE output_buf)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));

  JDIMENSION last_iMCU_row = cinfo->total_iMCU_rows - 1;
__boundcheck_metadata_store((void *)(&last_iMCU_row),(void *)((size_t)(&last_iMCU_row)+sizeof(last_iMCU_row)*8-1));

  JDIMENSION block_num;
__boundcheck_metadata_store((void *)(&block_num),(void *)((size_t)(&block_num)+sizeof(block_num)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  block_row;
__boundcheck_metadata_store((void *)(&block_row),(void *)((size_t)(&block_row)+sizeof(block_row)*8-1));
int  block_rows;
__boundcheck_metadata_store((void *)(&block_rows),(void *)((size_t)(&block_rows)+sizeof(block_rows)*8-1));

  JBLOCKARRAY buffer;
__boundcheck_metadata_store((void *)(&buffer),(void *)((size_t)(&buffer)+sizeof(buffer)*8-1));

  JBLOCKROW buffer_ptr;
__boundcheck_metadata_store((void *)(&buffer_ptr),(void *)((size_t)(&buffer_ptr)+sizeof(buffer_ptr)*8-1));

  JSAMPARRAY output_ptr;
__boundcheck_metadata_store((void *)(&output_ptr),(void *)((size_t)(&output_ptr)+sizeof(output_ptr)*8-1));

  JDIMENSION output_col;
__boundcheck_metadata_store((void *)(&output_col),(void *)((size_t)(&output_col)+sizeof(output_col)*8-1));

  jpeg_component_info *compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  inverse_DCT_method_ptr inverse_DCT;
__boundcheck_metadata_store((void *)(&inverse_DCT),(void *)((size_t)(&inverse_DCT)+sizeof(inverse_DCT)*8-1));


  /* Force some input to be done if we are getting ahead of the input. */
  while (cinfo->input_scan_number < cinfo->output_scan_number ||
	 (cinfo->input_scan_number == cinfo->output_scan_number &&
	  cinfo->input_iMCU_row <= cinfo->output_iMCU_row)) {
    if ((*(int (*)(j_decompress_ptr))(__boundcheck_ptr_reference(331,28,"decompress_data",(void *)(cinfo->inputctl->consume_input),(void *)cinfo->inputctl->consume_input)))(cinfo) == JPEG_SUSPENDED)
      return JPEG_SUSPENDED;
  }

  /* OK, output from the virtual arrays. */
  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    /* Don't bother to IDCT an uninteresting component. */
    if (! compptr->component_needed)
      continue;
    /* Align the virtual buffer for this component. */
    buffer = (*(JBLOCKARRAY (*)(j_common_ptr, jvirt_barray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(342,28,"decompress_data",(void *)(cinfo->mem->access_virt_barray),(void *)cinfo->mem->access_virt_barray)))
      ((j_common_ptr) cinfo, coef->whole_image[_RV_insert_check(0,10,343,30,"decompress_data",ci)],
       cinfo->output_iMCU_row * compptr->v_samp_factor,
       (JDIMENSION) compptr->v_samp_factor, FALSE);
    /* Count non-dummy DCT block rows in this iMCU row. */
    if (cinfo->output_iMCU_row < last_iMCU_row)
      block_rows = compptr->v_samp_factor;
    else {
      /* NB: can't use last_row_height here; it is input-side-dependent! */
      block_rows = (int) (compptr->height_in_blocks % compptr->v_samp_factor);
      if (block_rows == 0) block_rows = compptr->v_samp_factor;
    }
    inverse_DCT = cinfo->idct->inverse_DCT[_RV_insert_check(0,10,354,19,"decompress_data",ci)];
    output_ptr = (*(JSAMPARRAY *)(__boundcheck_ptr_reference(355,18,"decompress_data",(void *)(&output_buf[0]),(void *)(&output_buf[ci]))));
    /* Loop over all DCT blocks to be processed. */
    for (block_row = 0; block_row < block_rows; block_row++) {
      buffer_ptr = (*(JBLOCKROW *)(__boundcheck_ptr_reference(358,20,"decompress_data",(void *)(&buffer[0]),(void *)(&buffer[block_row]))));
      output_col = 0;
      for (block_num = 0; block_num < compptr->width_in_blocks; block_num++) {
	(*(inverse_DCT_method_ptr)(__boundcheck_ptr_reference(361,4,"decompress_data",(void *)(inverse_DCT),(void *)(inverse_DCT)))) (cinfo, compptr, (JCOEFPTR) buffer_ptr,
			output_ptr, output_col);
	buffer_ptr++;
	output_col += compptr->DCT_scaled_size;
      }
      output_ptr += compptr->DCT_scaled_size;
    }
  }

  if (++(cinfo->output_iMCU_row) < cinfo->total_iMCU_rows)
    return JPEG_ROW_COMPLETED;
  return JPEG_SCAN_COMPLETED;
}

#endif /* D_MULTISCAN_FILES_SUPPORTED */


#ifdef BLOCK_SMOOTHING_SUPPORTED

/*
 * This code applies interblock smoothing as described by section K.8
 * of the JPEG standard: the first 5 AC coefficients are estimated from
 * the DC values of a DCT block and its 8 neighboring blocks.
 * We apply smoothing only for progressive JPEG decoding, and only if
 * the coefficients it can estimate are not yet known to full precision.
 */

/* Natural-order array positions of the first 5 zigzag-order coefficients */
#define Q01_POS  1
#define Q10_POS  8
#define Q20_POS  16
#define Q11_POS  9
#define Q02_POS  2

/*
 * Determine whether block smoothing is applicable and safe.
 * We also latch the current states of the coef_bits[] entries for the
 * AC coefficients; otherwise, if the input side of the decompressor
 * advances into a new scan, we might think the coefficients are known
 * more accurately than they really are.
 */

LOCAL(boolean)
smoothing_ok (j_decompress_ptr cinfo)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));

  boolean smoothing_useful = FALSE;
__boundcheck_metadata_store((void *)(&smoothing_useful),(void *)((size_t)(&smoothing_useful)+sizeof(smoothing_useful)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  coefi;
__boundcheck_metadata_store((void *)(&coefi),(void *)((size_t)(&coefi)+sizeof(coefi)*8-1));

  jpeg_component_info *compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  JQUANT_TBL * qtable;
__boundcheck_metadata_store((void *)(&qtable),(void *)((size_t)(&qtable)+sizeof(qtable)*8-1));

  int * coef_bits;
__boundcheck_metadata_store((void *)(&coef_bits),(void *)((size_t)(&coef_bits)+sizeof(coef_bits)*8-1));

  int * coef_bits_latch;
__boundcheck_metadata_store((void *)(&coef_bits_latch),(void *)((size_t)(&coef_bits_latch)+sizeof(coef_bits_latch)*8-1));


  if (! cinfo->progressive_mode || cinfo->coef_bits == NULL)
    return FALSE;

  /* Allocate latch area if not already done */
  if (coef->coef_bits_latch == NULL)
    coef->coef_bits_latch = (int *)
      (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(420,21,"smoothing_ok",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  cinfo->num_components *
				  (SAVED_COEFS * SIZEOF(int)));
  coef_bits_latch = coef->coef_bits_latch;

  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    /* All components' quantization values must already be latched. */
    if ((qtable = compptr->quant_table) == NULL)
      return FALSE;
    /* Verify DC & first 5 AC quantizers are nonzero to avoid zero-divide. */
    if (qtable->quantval[_RV_insert_check(0,64,431,9,"smoothing_ok",0)] == 0 ||
	qtable->quantval[Q01_POS)] == 0 ||
	qtable->quantval[Q10_POS)] == 0 ||
	qtable->quantval[Q20_POS)] == 0 ||
	qtable->quantval[Q11_POS)] == 0 ||
	qtable->quantval[Q02_POS)] == 0)
      return FALSE;
    /* DC values must be at least partly known for all components. */
    coef_bits = cinfo->coef_bits[ci];
    if ((*(int *)(__boundcheck_ptr_reference(440,9,"smoothing_ok",(void *)(&coef_bits[0]),(void *)(&coef_bits[0])))) < 0)
      return FALSE;
    /* Block smoothing is helpful if some AC coefficients remain inaccurate. */
    for (coefi = 1; coefi <= 5; coefi++) {
      (*(int *)(__boundcheck_ptr_reference(444,7,"smoothing_ok",(void *)(&coef_bits_latch[0]),(void *)(&coef_bits_latch[coefi])))) = (*(int *)(__boundcheck_ptr_reference(444,32,"smoothing_ok",(void *)(&coef_bits[0]),(void *)(&coef_bits[coefi]))));
      if ((*(int *)(__boundcheck_ptr_reference(445,11,"smoothing_ok",(void *)(&coef_bits[0]),(void *)(&coef_bits[coefi])))) != 0)
	smoothing_useful = TRUE;
    }
    coef_bits_latch += SAVED_COEFS;
  }

  return smoothing_useful;
}


/*
 * Variant of decompress_data for use when doing block smoothing.
 */

METHODDEF(int)
decompress_smooth_data (j_decompress_ptr cinfo, JSAMPIMAGE output_buf)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));

  JDIMENSION last_iMCU_row = cinfo->total_iMCU_rows - 1;
__boundcheck_metadata_store((void *)(&last_iMCU_row),(void *)((size_t)(&last_iMCU_row)+sizeof(last_iMCU_row)*8-1));

  JDIMENSION block_num;
__boundcheck_metadata_store((void *)(&block_num),(void *)((size_t)(&block_num)+sizeof(block_num)*8-1));
JDIMENSION  last_block_column;
__boundcheck_metadata_store((void *)(&last_block_column),(void *)((size_t)(&last_block_column)+sizeof(last_block_column)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  block_row;
__boundcheck_metadata_store((void *)(&block_row),(void *)((size_t)(&block_row)+sizeof(block_row)*8-1));
int  block_rows;
__boundcheck_metadata_store((void *)(&block_rows),(void *)((size_t)(&block_rows)+sizeof(block_rows)*8-1));
int  access_rows;
__boundcheck_metadata_store((void *)(&access_rows),(void *)((size_t)(&access_rows)+sizeof(access_rows)*8-1));

  JBLOCKARRAY buffer;
__boundcheck_metadata_store((void *)(&buffer),(void *)((size_t)(&buffer)+sizeof(buffer)*8-1));

  JBLOCKROW buffer_ptr;
__boundcheck_metadata_store((void *)(&buffer_ptr),(void *)((size_t)(&buffer_ptr)+sizeof(buffer_ptr)*8-1));
JBLOCKROW  prev_block_row;
__boundcheck_metadata_store((void *)(&prev_block_row),(void *)((size_t)(&prev_block_row)+sizeof(prev_block_row)*8-1));
JBLOCKROW  next_block_row;
__boundcheck_metadata_store((void *)(&next_block_row),(void *)((size_t)(&next_block_row)+sizeof(next_block_row)*8-1));

  JSAMPARRAY output_ptr;
__boundcheck_metadata_store((void *)(&output_ptr),(void *)((size_t)(&output_ptr)+sizeof(output_ptr)*8-1));

  JDIMENSION output_col;
__boundcheck_metadata_store((void *)(&output_col),(void *)((size_t)(&output_col)+sizeof(output_col)*8-1));

  jpeg_component_info *compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  inverse_DCT_method_ptr inverse_DCT;
__boundcheck_metadata_store((void *)(&inverse_DCT),(void *)((size_t)(&inverse_DCT)+sizeof(inverse_DCT)*8-1));

  boolean first_row;
__boundcheck_metadata_store((void *)(&first_row),(void *)((size_t)(&first_row)+sizeof(first_row)*8-1));
boolean  last_row;
__boundcheck_metadata_store((void *)(&last_row),(void *)((size_t)(&last_row)+sizeof(last_row)*8-1));

  JBLOCK workspace;__boundcheck_metadata_store(&workspace[0],&workspace[JBLOCK-1]);

  int *coef_bits;
__boundcheck_metadata_store((void *)(&coef_bits),(void *)((size_t)(&coef_bits)+sizeof(coef_bits)*8-1));

  JQUANT_TBL *quanttbl;
__boundcheck_metadata_store((void *)(&quanttbl),(void *)((size_t)(&quanttbl)+sizeof(quanttbl)*8-1));

  INT32 Q00;
__boundcheck_metadata_store((void *)(&Q00),(void *)((size_t)(&Q00)+sizeof(Q00)*8-1));
INT32 Q01;
__boundcheck_metadata_store((void *)(&Q01),(void *)((size_t)(&Q01)+sizeof(Q01)*8-1));
INT32 Q02;
__boundcheck_metadata_store((void *)(&Q02),(void *)((size_t)(&Q02)+sizeof(Q02)*8-1));
INT32 Q10;
__boundcheck_metadata_store((void *)(&Q10),(void *)((size_t)(&Q10)+sizeof(Q10)*8-1));
INT32 Q11;
__boundcheck_metadata_store((void *)(&Q11),(void *)((size_t)(&Q11)+sizeof(Q11)*8-1));
INT32 Q20;
__boundcheck_metadata_store((void *)(&Q20),(void *)((size_t)(&Q20)+sizeof(Q20)*8-1));
INT32  num;
__boundcheck_metadata_store((void *)(&num),(void *)((size_t)(&num)+sizeof(num)*8-1));

  int DC1;
__boundcheck_metadata_store((void *)(&DC1),(void *)((size_t)(&DC1)+sizeof(DC1)*8-1));
int DC2;
__boundcheck_metadata_store((void *)(&DC2),(void *)((size_t)(&DC2)+sizeof(DC2)*8-1));
int DC3;
__boundcheck_metadata_store((void *)(&DC3),(void *)((size_t)(&DC3)+sizeof(DC3)*8-1));
int DC4;
__boundcheck_metadata_store((void *)(&DC4),(void *)((size_t)(&DC4)+sizeof(DC4)*8-1));
int DC5;
__boundcheck_metadata_store((void *)(&DC5),(void *)((size_t)(&DC5)+sizeof(DC5)*8-1));
int DC6;
__boundcheck_metadata_store((void *)(&DC6),(void *)((size_t)(&DC6)+sizeof(DC6)*8-1));
int DC7;
__boundcheck_metadata_store((void *)(&DC7),(void *)((size_t)(&DC7)+sizeof(DC7)*8-1));
int DC8;
__boundcheck_metadata_store((void *)(&DC8),(void *)((size_t)(&DC8)+sizeof(DC8)*8-1));
int DC9;
__boundcheck_metadata_store((void *)(&DC9),(void *)((size_t)(&DC9)+sizeof(DC9)*8-1));

  int Al;
__boundcheck_metadata_store((void *)(&Al),(void *)((size_t)(&Al)+sizeof(Al)*8-1));
int  pred;
__boundcheck_metadata_store((void *)(&pred),(void *)((size_t)(&pred)+sizeof(pred)*8-1));


  /* Force some input to be done if we are getting ahead of the input. */
  while (cinfo->input_scan_number <= cinfo->output_scan_number &&
	 ! cinfo->inputctl->eoi_reached) {
    if (cinfo->input_scan_number == cinfo->output_scan_number) {
      /* If input is working on current scan, we ordinarily want it to
       * have completed the current row.  But if input scan is DC,
       * we want it to keep one row ahead so that next block row's DC
       * values are up to date.
       */
      JDIMENSION delta = (cinfo->Ss == 0) ? 1 : 0;
__boundcheck_metadata_store((void *)(&delta),(void *)((size_t)(&delta)+sizeof(delta)*8-1));

      if (cinfo->input_iMCU_row > cinfo->output_iMCU_row+delta)
	break;
    }
    if ((*(int (*)(j_decompress_ptr))(__boundcheck_ptr_reference(493,28,"decompress_smooth_data",(void *)(cinfo->inputctl->consume_input),(void *)cinfo->inputctl->consume_input)))(cinfo) == JPEG_SUSPENDED)
      return JPEG_SUSPENDED;
  }

  /* OK, output from the virtual arrays. */
  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    /* Don't bother to IDCT an uninteresting component. */
    if (! compptr->component_needed)
      continue;
    /* Count non-dummy DCT block rows in this iMCU row. */
    if (cinfo->output_iMCU_row < last_iMCU_row) {
      block_rows = compptr->v_samp_factor;
      access_rows = block_rows * 2; /* this and next iMCU row */
      last_row = FALSE;
    } else {
      /* NB: can't use last_row_height here; it is input-side-dependent! */
      block_rows = (int) (compptr->height_in_blocks % compptr->v_samp_factor);
      if (block_rows == 0) block_rows = compptr->v_samp_factor;
      access_rows = block_rows; /* this iMCU row only */
      last_row = TRUE;
    }
    /* Align the virtual buffer for this component. */
    if (cinfo->output_iMCU_row > 0) {
      access_rows += compptr->v_samp_factor; /* prior iMCU row too */
      buffer = (*(JBLOCKARRAY (*)(j_common_ptr, jvirt_barray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(518,30,"decompress_smooth_data",(void *)(cinfo->mem->access_virt_barray),(void *)cinfo->mem->access_virt_barray)))
	((j_common_ptr) cinfo, coef->whole_image[_RV_insert_check(0,10,519,25,"decompress_smooth_data",ci)],
	 (cinfo->output_iMCU_row - 1) * compptr->v_samp_factor,
	 (JDIMENSION) access_rows, FALSE);
      buffer += compptr->v_samp_factor;	/* point to current iMCU row */
      first_row = FALSE;
    } else {
      buffer = (*(JBLOCKARRAY (*)(j_common_ptr, jvirt_barray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(525,30,"decompress_smooth_data",(void *)(cinfo->mem->access_virt_barray),(void *)cinfo->mem->access_virt_barray)))
	((j_common_ptr) cinfo, coef->whole_image[_RV_insert_check(0,10,526,25,"decompress_smooth_data",ci)],
	 (JDIMENSION) 0, (JDIMENSION) access_rows, FALSE);
      first_row = TRUE;
    }
    /* Fetch component-dependent info */
    coef_bits = coef->coef_bits_latch + (ci * SAVED_COEFS);
    quanttbl = compptr->quant_table;
    Q00 = quanttbl->quantval[_RV_insert_check(0,64,533,11,"decompress_smooth_data",0)];
    Q01 = quanttbl->quantval[Q01_POS)];
    Q10 = quanttbl->quantval[Q10_POS)];
    Q20 = quanttbl->quantval[Q20_POS)];
    Q11 = quanttbl->quantval[Q11_POS)];
    Q02 = quanttbl->quantval[Q02_POS)];
    inverse_DCT = cinfo->idct->inverse_DCT[_RV_insert_check(0,10,539,19,"decompress_smooth_data",ci)];
    output_ptr = (*(JSAMPARRAY *)(__boundcheck_ptr_reference(540,18,"decompress_smooth_data",(void *)(&output_buf[0]),(void *)(&output_buf[ci]))));
    /* Loop over all DCT blocks to be processed. */
    for (block_row = 0; block_row < block_rows; block_row++) {
      buffer_ptr = (*(JBLOCKROW *)(__boundcheck_ptr_reference(543,20,"decompress_smooth_data",(void *)(&buffer[0]),(void *)(&buffer[block_row]))));
      if (first_row && block_row == 0)
	prev_block_row = buffer_ptr;
__boundcheck_metadata_trans_check((void *)(prev_block_row),(void *)(buffer_ptr),(void *)(buffer_ptr));

      else
	prev_block_row = (*(JBLOCKROW *)(__boundcheck_ptr_reference(547,19,"decompress_smooth_data",(void *)(&buffer[0]),(void *)(&buffer[block_row - 1]))));
      if (last_row && block_row == block_rows-1)
	next_block_row = buffer_ptr;
__boundcheck_metadata_trans_check((void *)(next_block_row),(void *)(buffer_ptr),(void *)(buffer_ptr));

      else
	next_block_row = (*(JBLOCKROW *)(__boundcheck_ptr_reference(551,19,"decompress_smooth_data",(void *)(&buffer[0]),(void *)(&buffer[block_row + 1]))));
      /* We fetch the surrounding DC values using a sliding-register approach.
       * Initialize all nine here so as to do the right thing on narrow pics.
       */
      DC1 = DC2 = DC3 = (int) (*(JCOEF *)(__boundcheck_ptr_reference(555,31,"decompress_smooth_data",(void *)(&prev_block_row[0][0]),(void *)(&prev_block_row[0][0]))));
      DC4 = DC5 = DC6 = (int) (*(JCOEF *)(__boundcheck_ptr_reference(556,31,"decompress_smooth_data",(void *)(&buffer_ptr[0][0]),(void *)(&buffer_ptr[0][0]))));
      DC7 = DC8 = DC9 = (int) (*(JCOEF *)(__boundcheck_ptr_reference(557,31,"decompress_smooth_data",(void *)(&next_block_row[0][0]),(void *)(&next_block_row[0][0]))));
      output_col = 0;
      last_block_column = compptr->width_in_blocks - 1;
      for (block_num = 0; block_num <= last_block_column; block_num++) {
	/* Fetch current DCT block into workspace so we can modify it. */
	jcopy_block_row(buffer_ptr, (JBLOCKROW) workspace, (JDIMENSION) 1);
	/* Update DC values */
	if (block_num < last_block_column) {
	  DC3 = (int) (*(JCOEF *)(__boundcheck_ptr_reference(565,16,"decompress_smooth_data",(void *)(&prev_block_row[1][0]),(void *)(&prev_block_row[1][0]))));
	  DC6 = (int) (*(JCOEF *)(__boundcheck_ptr_reference(566,16,"decompress_smooth_data",(void *)(&buffer_ptr[1][0]),(void *)(&buffer_ptr[1][0]))));
	  DC9 = (int) (*(JCOEF *)(__boundcheck_ptr_reference(567,16,"decompress_smooth_data",(void *)(&next_block_row[1][0]),(void *)(&next_block_row[1][0]))));
	}
	/* Compute coefficient estimates per K.8.
	 * An estimate is applied only if coefficient is still zero,
	 * and is not known to be fully accurate.
	 */
	/* AC01 */
	if ((Al=(*(int *)(__boundcheck_ptr_reference(574,10,"decompress_smooth_data",(void *)(&coef_bits[0]),(void *)(&coef_bits[1]))))) != 0 && (*(JCOEF *)(__boundcheck_ptr_reference(574,43,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[1])))) == 0) {
	  num = 36 * Q00 * (DC4 - DC6);
	  if (num >= 0) {
	    pred = (int) (((Q01<<7) + num) / (Q01<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q01<<7) - num) / (Q01<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  (*(JCOEF *)(__boundcheck_ptr_reference(586,15,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[1])))) = (JCOEF) pred;
	}
	/* AC10 */
	if ((Al=(*(int *)(__boundcheck_ptr_reference(589,10,"decompress_smooth_data",(void *)(&coef_bits[0]),(void *)(&coef_bits[2]))))) != 0 && (*(JCOEF *)(__boundcheck_ptr_reference(589,43,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[8])))) == 0) {
	  num = 36 * Q00 * (DC2 - DC8);
	  if (num >= 0) {
	    pred = (int) (((Q10<<7) + num) / (Q10<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q10<<7) - num) / (Q10<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  (*(JCOEF *)(__boundcheck_ptr_reference(601,15,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[8])))) = (JCOEF) pred;
	}
	/* AC20 */
	if ((Al=(*(int *)(__boundcheck_ptr_reference(604,10,"decompress_smooth_data",(void *)(&coef_bits[0]),(void *)(&coef_bits[3]))))) != 0 && (*(JCOEF *)(__boundcheck_ptr_reference(604,44,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[16])))) == 0) {
	  num = 9 * Q00 * (DC2 + DC8 - 2*DC5);
	  if (num >= 0) {
	    pred = (int) (((Q20<<7) + num) / (Q20<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q20<<7) - num) / (Q20<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  (*(JCOEF *)(__boundcheck_ptr_reference(616,16,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[16])))) = (JCOEF) pred;
	}
	/* AC11 */
	if ((Al=(*(int *)(__boundcheck_ptr_reference(619,10,"decompress_smooth_data",(void *)(&coef_bits[0]),(void *)(&coef_bits[4]))))) != 0 && (*(JCOEF *)(__boundcheck_ptr_reference(619,43,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[9])))) == 0) {
	  num = 5 * Q00 * (DC1 - DC3 - DC7 + DC9);
	  if (num >= 0) {
	    pred = (int) (((Q11<<7) + num) / (Q11<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q11<<7) - num) / (Q11<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  (*(JCOEF *)(__boundcheck_ptr_reference(631,15,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[9])))) = (JCOEF) pred;
	}
	/* AC02 */
	if ((Al=(*(int *)(__boundcheck_ptr_reference(634,10,"decompress_smooth_data",(void *)(&coef_bits[0]),(void *)(&coef_bits[5]))))) != 0 && (*(JCOEF *)(__boundcheck_ptr_reference(634,43,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[2])))) == 0) {
	  num = 9 * Q00 * (DC4 + DC6 - 2*DC5);
	  if (num >= 0) {
	    pred = (int) (((Q02<<7) + num) / (Q02<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q02<<7) - num) / (Q02<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  (*(JCOEF *)(__boundcheck_ptr_reference(646,15,"decompress_smooth_data",(void *)(&workspace[0]),(void *)(&workspace[2])))) = (JCOEF) pred;
	}
	/* OK, do the IDCT */
	(*(inverse_DCT_method_ptr)(__boundcheck_ptr_reference(649,4,"decompress_smooth_data",(void *)(inverse_DCT),(void *)(inverse_DCT)))) (cinfo, compptr, (JCOEFPTR) workspace,
			output_ptr, output_col);
	/* Advance for next column */
	DC1 = DC2; DC2 = DC3;
	DC4 = DC5; DC5 = DC6;
	DC7 = DC8; DC8 = DC9;
	buffer_ptr++, prev_block_row++, next_block_row++;
	output_col += compptr->DCT_scaled_size;
      }
      output_ptr += compptr->DCT_scaled_size;
    }
  }

  if (++(cinfo->output_iMCU_row) < cinfo->total_iMCU_rows)
    return JPEG_ROW_COMPLETED;
  return JPEG_SCAN_COMPLETED;
}

#endif /* BLOCK_SMOOTHING_SUPPORTED */


/*
 * Initialize coefficient buffer controller.
 */

GLOBAL(void)
jinit_d_coef_controller (j_decompress_ptr cinfo, boolean need_full_buffer)
{
  my_coef_ptr coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));


  coef = (my_coef_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(680,19,"jinit_d_coef_controller",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_coef_controller));
  cinfo->coef = (struct jpeg_d_coef_controller *) coef;
  coef->pub.start_input_pass = start_input_pass;
  coef->pub.start_output_pass = start_output_pass;
#ifdef BLOCK_SMOOTHING_SUPPORTED
  coef->coef_bits_latch = NULL;
#endif

  /* Create the coefficient buffer. */
  if (need_full_buffer) {
#ifdef D_MULTISCAN_FILES_SUPPORTED
    /* Allocate a full-image virtual array for each component, */
    /* padded to a multiple of samp_factor DCT blocks in each direction. */
    /* Note we ask for a pre-zeroed array. */
    int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  access_rows;
__boundcheck_metadata_store((void *)(&access_rows),(void *)((size_t)(&access_rows)+sizeof(access_rows)*8-1));

    jpeg_component_info *compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));


    for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
	 ci++, compptr++) {
      access_rows = compptr->v_samp_factor;
#ifdef BLOCK_SMOOTHING_SUPPORTED
      /* If block smoothing could be used, need a bigger window */
      if (cinfo->progressive_mode)
	access_rows *= 3;
#endif
      coef->whole_image[_RV_insert_check(0,10,706,7,"jinit_d_coef_controller",ci)] = (*(jvirt_barray_ptr (*)(j_common_ptr, int, boolean, JDIMENSION, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(706,45,"jinit_d_coef_controller",(void *)(cinfo->mem->request_virt_barray),(void *)cinfo->mem->request_virt_barray)))
	((j_common_ptr) cinfo, JPOOL_IMAGE, TRUE,
	 (JDIMENSION) jround_up((long) compptr->width_in_blocks,
				(long) compptr->h_samp_factor),
	 (JDIMENSION) jround_up((long) compptr->height_in_blocks,
				(long) compptr->v_samp_factor),
	 (JDIMENSION) access_rows);
    }
    coef->pub.consume_data = consume_data;
    coef->pub.decompress_data = decompress_data;
    coef->pub.coef_arrays = coef->whole_image; /* link to virtual arrays */
#else
    ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif
  } else {
    /* We only need a single-MCU buffer. */
    JBLOCKROW buffer;
__boundcheck_metadata_store((void *)(&buffer),(void *)((size_t)(&buffer)+sizeof(buffer)*8-1));

    int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


    buffer = (JBLOCKROW)
      (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(726,21,"jinit_d_coef_controller",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  D_MAX_BLOCKS_IN_MCU * SIZEOF(JBLOCK));
    for (i = 0; i < D_MAX_BLOCKS_IN_MCU; i++) {
      coef->MCU_buffer[_RV_insert_check(0,10,729,7,"jinit_d_coef_controller",i)] = buffer + i;
    }
    coef->pub.consume_data = dummy_consume_data;
    coef->pub.decompress_data = decompress_onepass;
    coef->pub.coef_arrays = NULL; /* flag for no virtual arrays */
  }
}
