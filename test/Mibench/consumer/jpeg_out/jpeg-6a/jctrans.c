/*
 * jctrans.c
 *
 * Copyright (C) 1995-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains library routines for transcoding compression,
 * that is, writing raw DCT coefficient arrays to an output JPEG file.
 * The routines in jcapimin.c will also be needed by a transcoder.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/* Forward declarations */
LOCAL(void) transencode_master_selection
	JPP((j_compress_ptr cinfo, jvirt_barray_ptr * coef_arrays));
LOCAL(void) transencode_coef_controller
	JPP((j_compress_ptr cinfo, jvirt_barray_ptr * coef_arrays));


/*
 * Compression initialization for writing raw-coefficient data.
 * Before calling this, all parameters and a data destination must be set up.
 * Call jpeg_finish_compress() to actually write the data.
 *
 * The number of passed virtual arrays must match cinfo->num_components.
 * Note that the virtual arrays need not be filled or even realized at
 * the time write_coefficients is called; indeed, if the virtual arrays
 * were requested from this compression object's memory manager, they
 * typically will be realized during this routine and filled afterwards.
 */

GLOBAL(void)
jpeg_write_coefficients (j_compress_ptr cinfo, jvirt_barray_ptr * coef_arrays)
{
  if (cinfo->global_state != CSTATE_START)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  /* Mark all tables to be written */
  jpeg_suppress_tables(cinfo, FALSE);
  /* (Re)initialize error mgr and destination modules */
  (*(void (*)(j_common_ptr))(__boundcheck_ptr_reference(45,17,"jpeg_write_coefficients",(void *)(cinfo->err->reset_error_mgr),(void *)cinfo->err->reset_error_mgr))) ((j_common_ptr) cinfo);
  (*(void (*)(j_compress_ptr))(__boundcheck_ptr_reference(46,18,"jpeg_write_coefficients",(void *)(cinfo->dest->init_destination),(void *)cinfo->dest->init_destination))) (cinfo);
  /* Perform master selection of active modules */
  transencode_master_selection(cinfo, coef_arrays);
  /* Wait for jpeg_finish_compress() call */
  cinfo->next_scanline = 0;	/* so jpeg_write_marker works */
  cinfo->global_state = CSTATE_WRCOEFS;
}


/*
 * Initialize the compression object with default parameters,
 * then copy from the source object all parameters needed for lossless
 * transcoding.  Parameters that can be varied without loss (such as
 * scan script and Huffman optimization) are left in their default states.
 */

GLOBAL(void)
jpeg_copy_critical_parameters (j_decompress_ptr srcinfo,
			       j_compress_ptr dstinfo)
{
  JQUANT_TBL ** qtblptr;
__boundcheck_metadata_store((void *)(&qtblptr),(void *)((size_t)(&qtblptr)+sizeof(qtblptr)*8-1));

  jpeg_component_info *incomp;
__boundcheck_metadata_store((void *)(&incomp),(void *)((size_t)(&incomp)+sizeof(incomp)*8-1));
jpeg_component_info   *outcomp;
__boundcheck_metadata_store((void *)(&outcomp),(void *)((size_t)(&outcomp)+sizeof(outcomp)*8-1));

  JQUANT_TBL *c_quant;
__boundcheck_metadata_store((void *)(&c_quant),(void *)((size_t)(&c_quant)+sizeof(c_quant)*8-1));
JQUANT_TBL   *slot_quant;
__boundcheck_metadata_store((void *)(&slot_quant),(void *)((size_t)(&slot_quant)+sizeof(slot_quant)*8-1));

  int tblno;
__boundcheck_metadata_store((void *)(&tblno),(void *)((size_t)(&tblno)+sizeof(tblno)*8-1));
int  ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  coefi;
__boundcheck_metadata_store((void *)(&coefi),(void *)((size_t)(&coefi)+sizeof(coefi)*8-1));


  /* Safety check to ensure start_compress not called yet. */
  if (dstinfo->global_state != CSTATE_START)
    ERREXIT1(dstinfo, JERR_BAD_STATE, dstinfo->global_state);
  /* Copy fundamental image dimensions */
  dstinfo->image_width = srcinfo->image_width;
  dstinfo->image_height = srcinfo->image_height;
  dstinfo->input_components = srcinfo->num_components;
  dstinfo->in_color_space = srcinfo->jpeg_color_space;
  /* Initialize all parameters to default values */
  jpeg_set_defaults(dstinfo);
  /* jpeg_set_defaults may choose wrong colorspace, eg YCbCr if input is RGB.
   * Fix it to get the right header markers for the image colorspace.
   */
  jpeg_set_colorspace(dstinfo, srcinfo->jpeg_color_space);
  dstinfo->data_precision = srcinfo->data_precision;
  dstinfo->CCIR601_sampling = srcinfo->CCIR601_sampling;
  /* Copy the source's quantization tables. */
  for (tblno = 0; tblno < NUM_QUANT_TBLS; tblno++) {
    if (srcinfo->quant_tbl_ptrs[_RV_insert_check(0,4,89,9,"jpeg_copy_critical_parameters",tblno)] != NULL) {
      qtblptr = & dstinfo->quant_tbl_ptrs[_RV_insert_check(0,4,90,19,"jpeg_copy_critical_parameters",tblno)];
      if (*(JQUANT_TBL **)(__boundcheck_ptr_reference(91,12,"jpeg_copy_critical_parameters",(void *)(qtblptr),(void *)(qtblptr))) == NULL)
	*(JQUANT_TBL **)(__boundcheck_ptr_reference(92,3,"jpeg_copy_critical_parameters",(void *)(qtblptr),(void *)(qtblptr))) = jpeg_alloc_quant_table((j_common_ptr) dstinfo);
      MEMCOPY((*qtblptr)->quantval,
	      srcinfo->quant_tbl_ptrs[tblno]->quantval,
	      SIZEOF((*qtblptr)->quantval));
      (*(JQUANT_TBL **)(__boundcheck_ptr_reference(96,9,"jpeg_copy_critical_parameters",(void *)(qtblptr),(void *)(qtblptr))))->sent_table = FALSE;
    }
  }
  /* Copy the source's per-component info.
   * Note we assume jpeg_set_defaults has allocated the dest comp_info array.
   */
  dstinfo->num_components = srcinfo->num_components;
  if (dstinfo->num_components < 1 || dstinfo->num_components > MAX_COMPONENTS)
    ERREXIT2(dstinfo, JERR_COMPONENT_COUNT, dstinfo->num_components,
	     MAX_COMPONENTS);
  for (ci = 0, incomp = srcinfo->comp_info, outcomp = dstinfo->comp_info;
       ci < dstinfo->num_components; ci++, incomp++, outcomp++) {
    outcomp->component_id = incomp->component_id;
    outcomp->h_samp_factor = incomp->h_samp_factor;
    outcomp->v_samp_factor = incomp->v_samp_factor;
    outcomp->quant_tbl_no = incomp->quant_tbl_no;
    /* Make sure saved quantization table for component matches the qtable
     * slot.  If not, the input file re-used this qtable slot.
     * IJG encoder currently cannot duplicate this.
     */
    tblno = outcomp->quant_tbl_no;
    if (tblno < 0 || tblno >= NUM_QUANT_TBLS ||
	srcinfo->quant_tbl_ptrs[_RV_insert_check(0,4,118,2,"jpeg_copy_critical_parameters",tblno)] == NULL)
      ERREXIT1(dstinfo, JERR_NO_QUANT_TABLE, tblno);
    slot_quant = srcinfo->quant_tbl_ptrs[_RV_insert_check(0,4,120,18,"jpeg_copy_critical_parameters",tblno)];
    c_quant = incomp->quant_table;
    if (c_quant != NULL) {
      for (coefi = 0; coefi < DCTSIZE2; coefi++) {
	if (c_quant->quantval[_RV_insert_check(0,64,124,6,"jpeg_copy_critical_parameters",coefi)] != slot_quant->quantval[_RV_insert_check(0,64,124,34,"jpeg_copy_critical_parameters",coefi)])
	  ERREXIT1(dstinfo, JERR_MISMATCHED_QUANT_TABLE, tblno);
      }
    }
    /* Note: we do not copy the source's Huffman table assignments;
     * instead we rely on jpeg_set_colorspace to have made a suitable choice.
     */
  }
}


/*
 * Master selection of compression modules for transcoding.
 * This substitutes for jcinit.c's initialization of the full compressor.
 */

LOCAL(void)
transencode_master_selection (j_compress_ptr cinfo,
			      jvirt_barray_ptr * coef_arrays)
{
  /* Although we don't actually use input_components for transcoding,
   * jcmaster.c's initial_setup will complain if input_components is 0.
   */
  cinfo->input_components = 1;
  /* Initialize master control (includes parameter checking/processing) */
  jinit_c_master_control(cinfo, TRUE /* transcode only */);

  /* Entropy encoding: either Huffman or arithmetic coding. */
  if (cinfo->arith_code) {
    ERREXIT(cinfo, JERR_ARITH_NOTIMPL);
  } else {
    if (cinfo->progressive_mode) {
#ifdef C_PROGRESSIVE_SUPPORTED
      jinit_phuff_encoder(cinfo);
#else
      ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif
    } else
      jinit_huff_encoder(cinfo);
  }

  /* We need a special coefficient buffer controller. */
  transencode_coef_controller(cinfo, coef_arrays);

  jinit_marker_writer(cinfo);

  /* We can now tell the memory manager to allocate virtual arrays. */
  (*(void (*)(j_common_ptr))(__boundcheck_ptr_reference(171,17,"transencode_master_selection",(void *)(cinfo->mem->realize_virt_arrays),(void *)cinfo->mem->realize_virt_arrays))) ((j_common_ptr) cinfo);

  /* Write the datastream header (SOI) immediately.
   * Frame and scan headers are postponed till later.
   * This lets application insert special markers after the SOI.
   */
  (*(void (*)(j_compress_ptr))(__boundcheck_ptr_reference(177,20,"transencode_master_selection",(void *)(cinfo->marker->write_file_header),(void *)cinfo->marker->write_file_header))) (cinfo);
}


/*
 * The rest of this file is a special implementation of the coefficient
 * buffer controller.  This is similar to jccoefct.c, but it handles only
 * output from presupplied virtual arrays.  Furthermore, we generate any
 * dummy padding blocks on-the-fly rather than expecting them to be present
 * in the arrays.
 */

/* Private buffer controller object */

typedef struct {
  struct jpeg_c_coef_controller pub; /* public fields */

  JDIMENSION iMCU_row_num;	/* iMCU row # within image */
  JDIMENSION mcu_ctr;		/* counts MCUs processed in current row */
  int MCU_vert_offset;		/* counts MCU rows within iMCU row */
  int MCU_rows_per_iMCU_row;	/* number of such rows needed */

  /* Virtual block array for each component. */
  jvirt_barray_ptr * whole_image;

  /* Workspace for constructing dummy blocks at right/bottom edges. */
  JBLOCKROW dummy_buffer[C_MAX_BLOCKS_IN_MCU];
} my_coef_controller;

typedef my_coef_controller * my_coef_ptr;


LOCAL(void)
start_iMCU_row (j_compress_ptr cinfo)
/* Reset within-iMCU-row counters for a new row */
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
    if (coef->iMCU_row_num < (cinfo->total_iMCU_rows-1))
      coef->MCU_rows_per_iMCU_row = cinfo->cur_comp_info[_RV_insert_check(0,4,223,37,"start_iMCU_row",0)]->v_samp_factor;
    else
      coef->MCU_rows_per_iMCU_row = cinfo->cur_comp_info[_RV_insert_check(0,4,225,37,"start_iMCU_row",0)]->last_row_height;
  }

  coef->mcu_ctr = 0;
  coef->MCU_vert_offset = 0;
}


/*
 * Initialize for a processing pass.
 */

METHODDEF(void)
start_pass_coef (j_compress_ptr cinfo, J_BUF_MODE pass_mode)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));


  if (pass_mode != JBUF_CRANK_DEST)
    ERREXIT(cinfo, JERR_BAD_BUFFER_MODE);

  coef->iMCU_row_num = 0;
  start_iMCU_row(cinfo);
}


/*
 * Process some data.
 * We process the equivalent of one fully interleaved MCU row ("iMCU" row)
 * per call, ie, v_samp_factor block rows for each component in the scan.
 * The data is obtained from the virtual arrays and fed to the entropy coder.
 * Returns TRUE if the iMCU row is completed, FALSE if suspended.
 *
 * NB: input_buf is ignored; it is likely to be a NULL pointer.
 */

METHODDEF(boolean)
compress_output (j_compress_ptr cinfo, JSAMPIMAGE input_buf)
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
int  blockcnt;
__boundcheck_metadata_store((void *)(&blockcnt),(void *)((size_t)(&blockcnt)+sizeof(blockcnt)*8-1));

  JDIMENSION start_col;
__boundcheck_metadata_store((void *)(&start_col),(void *)((size_t)(&start_col)+sizeof(start_col)*8-1));

  JBLOCKARRAY buffer[MAX_COMPS_IN_SCAN];__boundcheck_metadata_store(&buffer[0],&buffer[4-1]);

  JBLOCKROW MCU_buffer[C_MAX_BLOCKS_IN_MCU];__boundcheck_metadata_store(&MCU_buffer[0],&MCU_buffer[10-1]);

  JBLOCKROW buffer_ptr;
__boundcheck_metadata_store((void *)(&buffer_ptr),(void *)((size_t)(&buffer_ptr)+sizeof(buffer_ptr)*8-1));

  jpeg_component_info *compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));


  /* Align the virtual buffers for the components used in this scan. */
  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,276,15,"compress_output",ci)];
    buffer[_RV_insert_check(0,4,277,5,"compress_output",ci)] = (*(JBLOCKARRAY (*)(j_common_ptr, jvirt_barray_ptr, JDIMENSION, JDIMENSION, boolean))(__boundcheck_ptr_reference(277,32,"compress_output",(void *)(cinfo->mem->access_virt_barray),(void *)cinfo->mem->access_virt_barray)))
      ((j_common_ptr) cinfo, coef->whole_image[compptr->component_index],
       coef->iMCU_row_num * compptr->v_samp_factor,
       (JDIMENSION) compptr->v_samp_factor, FALSE);
  }

  /* Loop to process one whole iMCU row */
  for (yoffset = coef->MCU_vert_offset; yoffset < coef->MCU_rows_per_iMCU_row;
       yoffset++) {
    for (MCU_col_num = coef->mcu_ctr; MCU_col_num < cinfo->MCUs_per_row;
	 MCU_col_num++) {
      /* Construct list of pointers to DCT blocks belonging to this MCU */
      blkn = 0;			/* index of current DCT block within MCU */
      for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
	compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,291,12,"compress_output",ci)];
	start_col = MCU_col_num * compptr->MCU_width;
	blockcnt = (MCU_col_num < last_MCU_col) ? compptr->MCU_width
						: compptr->last_col_width;
	for (yindex = 0; yindex < compptr->MCU_height; yindex++) {
	  if (coef->iMCU_row_num < last_iMCU_row ||
	      yindex+yoffset < compptr->last_row_height) {
	    /* Fill in pointers to real blocks in this row */
	    buffer_ptr = (*(JBLOCKROW *)(__boundcheck_ptr_reference(299,44,"compress_output",(void *)(&buffer[ci][0]),(void *)(&buffer[ci][yindex + yoffset])))) + start_col;
	    for (xindex = 0; xindex < blockcnt; xindex++)
	      MCU_buffer[_RV_insert_check(0,10,301,8,"compress_output",blkn++)] = buffer_ptr++;
	  } else {
	    /* At bottom of image, need a whole row of dummy blocks */
	    xindex = 0;
	  }
	  /* Fill in any dummy blocks needed in this row.
	   * Dummy blocks are filled in the same way as in jccoefct.c:
	   * all zeroes in the AC entries, DC entries equal to previous
	   * block's DC value.  The init routine has already zeroed the
	   * AC entries, so we need only set the DC entries correctly.
	   */
	  for (; xindex < compptr->MCU_width; xindex++) {
	    MCU_buffer[_RV_insert_check(0,10,313,6,"compress_output",blkn)] = coef->dummy_buffer[_RV_insert_check(0,10,313,25,"compress_output",blkn)];
	    MCU_buffer[blkn][0][0] = MCU_buffer[blkn-1][0][0];
	    blkn++;
	  }
	}
      }
      /* Try to write the MCU. */
      if (! (*(boolean (*)(j_compress_ptr, JBLOCKROW *))(__boundcheck_ptr_reference(320,31,"compress_output",(void *)(cinfo->entropy->encode_mcu),(void *)cinfo->entropy->encode_mcu))) (cinfo, MCU_buffer)) {
	/* Suspension forced; update state counters and exit */
	coef->MCU_vert_offset = yoffset;
	coef->mcu_ctr = MCU_col_num;
	return FALSE;
      }
    }
    /* Completed an MCU row, but perhaps not an iMCU row */
    coef->mcu_ctr = 0;
  }
  /* Completed the iMCU row, advance counters for next one */
  coef->iMCU_row_num++;
  start_iMCU_row(cinfo);
  return TRUE;
}


/*
 * Initialize coefficient buffer controller.
 *
 * Each passed coefficient array must be the right size for that
 * coefficient: width_in_blocks wide and height_in_blocks high,
 * with unitheight at least v_samp_factor.
 */

LOCAL(void)
transencode_coef_controller (j_compress_ptr cinfo,
			     jvirt_barray_ptr * coef_arrays)
{
  my_coef_ptr coef;
__boundcheck_metadata_store((void *)(&coef),(void *)((size_t)(&coef)+sizeof(coef)*8-1));

  JBLOCKROW buffer;
__boundcheck_metadata_store((void *)(&buffer),(void *)((size_t)(&buffer)+sizeof(buffer)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  coef = (my_coef_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(354,19,"transencode_coef_controller",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_coef_controller));
  cinfo->coef = (struct jpeg_c_coef_controller *) coef;
  coef->pub.start_pass = start_pass_coef;
  coef->pub.compress_data = compress_output;

  /* Save pointer to virtual arrays */
  coef->whole_image = coef_arrays;

  /* Allocate and pre-zero space for dummy DCT blocks. */
  buffer = (JBLOCKROW)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(365,19,"transencode_coef_controller",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				C_MAX_BLOCKS_IN_MCU * SIZEOF(JBLOCK));
  jzero_far((void FAR *) buffer, C_MAX_BLOCKS_IN_MCU * SIZEOF(JBLOCK));
  for (i = 0; i < C_MAX_BLOCKS_IN_MCU; i++) {
    coef->dummy_buffer[_RV_insert_check(0,10,369,5,"transencode_coef_controller",i)] = buffer + i;
  }
}
