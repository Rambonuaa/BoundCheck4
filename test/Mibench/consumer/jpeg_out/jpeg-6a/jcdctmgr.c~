/*
 * jcdctmgr.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains the forward-DCT management logic.
 * This code selects a particular DCT implementation to be used,
 * and it performs related housekeeping chores including coefficient
 * quantization.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jdct.h"		/* Private declarations for DCT subsystem */


/* Private subobject for this module */

typedef struct {
  struct jpeg_forward_dct pub;	/* public fields */

  /* Pointer to the DCT routine actually in use */
  forward_DCT_method_ptr do_dct;

  /* The actual post-DCT divisors --- not identical to the quant table
   * entries, because of scaling (especially for an unnormalized DCT).
   * Each table is given in normal array order.
   */
  DCTELEM * divisors[NUM_QUANT_TBLS];

#ifdef DCT_FLOAT_SUPPORTED
  /* Same as above for the floating-point case. */
  float_DCT_method_ptr do_float_dct;
  FAST_FLOAT * float_divisors[NUM_QUANT_TBLS];
#endif
} my_fdct_controller;

typedef my_fdct_controller * my_fdct_ptr;


/*
 * Initialize for a processing pass.
 * Verify that all referenced Q-tables are present, and set up
 * the divisor table for each one.
 * In the current implementation, DCT of all components is done during
 * the first pass, even if only some components will be output in the
 * first scan.  Hence all components should be examined here.
 */

METHODDEF(void)
start_pass_fdctmgr (j_compress_ptr cinfo)
{
  my_fdct_ptr fdct = (my_fdct_ptr) cinfo->fdct;
__boundcheck_metadata_store((void *)(&fdct),(void *)((size_t)(&fdct)+sizeof(fdct)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  qtblno;
__boundcheck_metadata_store((void *)(&qtblno),(void *)((size_t)(&qtblno)+sizeof(qtblno)*8-1));
int  i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  jpeg_component_info *compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  JQUANT_TBL * qtbl;
__boundcheck_metadata_store((void *)(&qtbl),(void *)((size_t)(&qtbl)+sizeof(qtbl)*8-1));

  DCTELEM * dtbl;
__boundcheck_metadata_store((void *)(&dtbl),(void *)((size_t)(&dtbl)+sizeof(dtbl)*8-1));


  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    qtblno = compptr->quant_tbl_no;
    /* Make sure specified quantization table is present */
    if (qtblno < 0 || qtblno >= NUM_QUANT_TBLS ||
	cinfo->quant_tbl_ptrs[_RV_insert_check(0,4,67,2,"start_pass_fdctmgr",qtblno)] == NULL)
      ERREXIT1(cinfo, JERR_NO_QUANT_TABLE, qtblno);
    qtbl = cinfo->quant_tbl_ptrs[_RV_insert_check(0,4,69,12,"start_pass_fdctmgr",qtblno)];
    /* Compute divisors for this quant table */
    /* We may do this more than once for same table, but it's not a big deal */
    switch (cinfo->dct_method) {
#ifdef DCT_ISLOW_SUPPORTED
    case JDCT_ISLOW:
      /* For LL&M IDCT method, divisors are equal to raw quantization
       * coefficients multiplied by 8 (to counteract scaling).
       */
      if (fdct->divisors[_RV_insert_check(0,4,78,11,"start_pass_fdctmgr",qtblno)] == NULL) {
	fdct->divisors[_RV_insert_check(0,4,79,2,"start_pass_fdctmgr",qtblno)] = (DCTELEM *)
	  (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(80,18,"start_pass_fdctmgr",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				      DCTSIZE2 * SIZEOF(DCTELEM));
      }
      dtbl = fdct->divisors[_RV_insert_check(0,4,83,14,"start_pass_fdctmgr",qtblno)];
      for (i = 0; i < DCTSIZE2; i++) {
	(*(DCTELEM *)(__boundcheck_ptr_reference(85,2,"start_pass_fdctmgr",(void *)(&dtbl[0]),(void *)(&dtbl[i])))) = ((DCTELEM) qtbl->quantval[_RV_insert_check(0,64,85,23,"start_pass_fdctmgr",i)]) << 3;
      }
      break;
#endif
#ifdef DCT_IFAST_SUPPORTED
    case JDCT_IFAST:
      {
	/* For AA&N IDCT method, divisors are equal to quantization
	 * coefficients scaled by scalefactor[row]*scalefactor[col], where
	 *   scalefactor[0] = 1
	 *   scalefactor[k] = cos(k*PI/16) * sqrt(2)    for k=1..7
	 * We apply a further scale factor of 8.
	 */
#define CONST_BITS 14
	static const INT16 aanscales[DCTSIZE2] = {
	  /* precomputed values scaled up by 14 bits */
	  16384, 22725, 21407, 19266, 16384, 12873,  8867,  4520,
	  22725, 31521, 29692, 26722, 22725, 17855, 12299,  6270,
	  21407, 29692, 27969, 25172, 21407, 16819, 11585,  5906,
	  19266, 26722, 25172, 22654, 19266, 15137, 10426,  5315,
	  16384, 22725, 21407, 19266, 16384, 12873,  8867,  4520,
	  12873, 17855, 16819, 15137, 12873, 10114,  6967,  3552,
	   8867, 12299, 11585, 10426,  8867,  6967,  4799,  2446,
	   4520,  6270,  5906,  5315,  4520,  3552,  2446,  1247
	};__boundcheck_metadata_store(&aanscales[0],&aanscales[64-1]);

	SHIFT_TEMPS

	if (fdct->divisors[_RV_insert_check(0,4,112,6,"start_pass_fdctmgr",qtblno)] == NULL) {
	  fdct->divisors[_RV_insert_check(0,4,113,4,"start_pass_fdctmgr",qtblno)] = (DCTELEM *)
	    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(114,20,"start_pass_fdctmgr",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
					DCTSIZE2 * SIZEOF(DCTELEM));
	}
	dtbl = fdct->divisors[_RV_insert_check(0,4,117,9,"start_pass_fdctmgr",qtblno)];
	for (i = 0; i < DCTSIZE2; i++) {
	  (*(DCTELEM *)(__boundcheck_ptr_reference(119,4,"start_pass_fdctmgr",(void *)(&dtbl[0]),(void *)(&dtbl[i])))) = (DCTELEM)
	    DESCALE(MULTIPLY16V16((INT32) qtbl->quantval[i],
				  (INT32) aanscales[i]),
		    CONST_BITS-3);
	}
      }
      break;
#endif
#ifdef DCT_FLOAT_SUPPORTED
    case JDCT_FLOAT:
      {
	/* For float AA&N IDCT method, divisors are equal to quantization
	 * coefficients scaled by scalefactor[row]*scalefactor[col], where
	 *   scalefactor[0] = 1
	 *   scalefactor[k] = cos(k*PI/16) * sqrt(2)    for k=1..7
	 * We apply a further scale factor of 8.
	 * What's actually stored is 1/divisor so that the inner loop can
	 * use a multiplication rather than a division.
	 */
	FAST_FLOAT * fdtbl;
__boundcheck_metadata_store((void *)(&fdtbl),(void *)((size_t)(&fdtbl)+sizeof(fdtbl)*8-1));

	int row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));
int  col;
__boundcheck_metadata_store((void *)(&col),(void *)((size_t)(&col)+sizeof(col)*8-1));

	static const double aanscalefactor[DCTSIZE] = {
	  1.0, 1.387039845, 1.306562965, 1.175875602,
	  1.0, 0.785694958, 0.541196100, 0.275899379
	};__boundcheck_metadata_store(&aanscalefactor[0],&aanscalefactor[8-1]);


	if (fdct->float_divisors[_RV_insert_check(0,4,145,6,"start_pass_fdctmgr",qtblno)] == NULL) {
	  fdct->float_divisors[_RV_insert_check(0,4,146,4,"start_pass_fdctmgr",qtblno)] = (FAST_FLOAT *)
	    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(147,20,"start_pass_fdctmgr",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
					DCTSIZE2 * SIZEOF(FAST_FLOAT));
	}
	fdtbl = fdct->float_divisors[_RV_insert_check(0,4,150,10,"start_pass_fdctmgr",qtblno)];
	i = 0;
	for (row = 0; row < DCTSIZE; row++) {
	  for (col = 0; col < DCTSIZE; col++) {
	    (*(float *)(__boundcheck_ptr_reference(154,6,"start_pass_fdctmgr",(void *)(&fdtbl[0]),(void *)(&fdtbl[i])))) = (FAST_FLOAT)
	      (1.0 / (((double) qtbl->quantval[_RV_insert_check(0,64,155,26,"start_pass_fdctmgr",i)] *
		       aanscalefactor[_RV_insert_check(0,8,156,10,"start_pass_fdctmgr",row)] * aanscalefactor[_RV_insert_check(0,8,156,32,"start_pass_fdctmgr",col)] * 8.0)));
	    i++;
	  }
	}
      }
      break;
#endif
    default:
      ERREXIT(cinfo, JERR_NOT_COMPILED);
      break;
    }
  }
}


/*
 * Perform forward DCT on one or more blocks of a component.
 *
 * The input samples are taken from the sample_data[] array starting at
 * position start_row/start_col, and moving to the right for any additional
 * blocks. The quantized coefficients are returned in coef_blocks[].
 */

METHODDEF(void)
forward_DCT (j_compress_ptr cinfo, jpeg_component_info * compptr,
	     JSAMPARRAY sample_data, JBLOCKROW coef_blocks,
	     JDIMENSION start_row, JDIMENSION start_col,
	     JDIMENSION num_blocks)
/* This version is used for integer DCT implementations. */
{
  /* This routine is heavily used, so it's worth coding it tightly. */
  my_fdct_ptr fdct = (my_fdct_ptr) cinfo->fdct;
__boundcheck_metadata_store((void *)(&fdct),(void *)((size_t)(&fdct)+sizeof(fdct)*8-1));

  forward_DCT_method_ptr do_dct = fdct->do_dct;
__boundcheck_metadata_store((void *)(&do_dct),(void *)((size_t)(&do_dct)+sizeof(do_dct)*8-1));

  DCTELEM * divisors = fdct->divisors[_RV_insert_check(0,4,189,24,"forward_DCT",compptr->quant_tbl_no)];
__boundcheck_metadata_store((void *)(&divisors),(void *)((size_t)(&divisors)+sizeof(divisors)*8-1));

  DCTELEM workspace[DCTSIZE2];__boundcheck_metadata_store(&workspace[0],&workspace[64-1]);
	/* work area for FDCT subroutine */
  JDIMENSION bi;
__boundcheck_metadata_store((void *)(&bi),(void *)((size_t)(&bi)+sizeof(bi)*8-1));


  sample_data += start_row;	/* fold in the vertical offset once */

  for (bi = 0; bi < num_blocks; bi++, start_col += DCTSIZE) {
    /* Load data into workspace, applying unsigned->signed conversion */
    { register DCTELEM *workspaceptr;
      register JSAMPROW elemptr;
      register int elemr;

      workspaceptr = workspace;
__boundcheck_metadata_trans_check((void *)(workspaceptr),(void *)(workspace),(void *)(workspace));

      for (elemr = 0; elemr < DCTSIZE; elemr++) {
	elemptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(203,12,"forward_DCT",(void *)(&sample_data[0]),(void *)(&sample_data[elemr])))) + start_col;
#if DCTSIZE == 8		/* unroll the inner loop */
	*(DCTELEM *)(__boundcheck_ptr_reference(205,15,"forward_DCT",(void *)(workspaceptr),(void *)(workspaceptr++))) = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
	*(DCTELEM *)(__boundcheck_ptr_reference(206,15,"forward_DCT",(void *)(workspaceptr),(void *)(workspaceptr++))) = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
	*(DCTELEM *)(__boundcheck_ptr_reference(207,15,"forward_DCT",(void *)(workspaceptr),(void *)(workspaceptr++))) = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
	*(DCTELEM *)(__boundcheck_ptr_reference(208,15,"forward_DCT",(void *)(workspaceptr),(void *)(workspaceptr++))) = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
	*(DCTELEM *)(__boundcheck_ptr_reference(209,15,"forward_DCT",(void *)(workspaceptr),(void *)(workspaceptr++))) = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
	*(DCTELEM *)(__boundcheck_ptr_reference(210,15,"forward_DCT",(void *)(workspaceptr),(void *)(workspaceptr++))) = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
	*(DCTELEM *)(__boundcheck_ptr_reference(211,15,"forward_DCT",(void *)(workspaceptr),(void *)(workspaceptr++))) = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
	*(DCTELEM *)(__boundcheck_ptr_reference(212,15,"forward_DCT",(void *)(workspaceptr),(void *)(workspaceptr++))) = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
#else
	{ register int elemc;
	  for (elemc = DCTSIZE; elemc > 0; elemc--) {
	    *workspaceptr++ = GETJSAMPLE(*elemptr++) - CENTERJSAMPLE;
	  }
	}
#endif
      }
    }

    /* Perform the DCT */
    (*(forward_DCT_method_ptr)(__boundcheck_ptr_reference(224,7,"forward_DCT",(void *)(do_dct),(void *)(do_dct)))) (workspace);

    /* Quantize/descale the coefficients, and store into coef_blocks[] */
    { register DCTELEM temp, qval;
      register int i;
      register JCOEFPTR output_ptr = (*(JBLOCK *)(__boundcheck_ptr_reference(229,38,"forward_DCT",(void *)(&coef_blocks[0]),(void *)(&coef_blocks[bi]))));

      for (i = 0; i < DCTSIZE2; i++) {
	qval = (*(DCTELEM *)(__boundcheck_ptr_reference(232,9,"forward_DCT",(void *)(&divisors[0]),(void *)(&divisors[i]))));
	temp = workspace[_RV_insert_check(0,64,233,9,"forward_DCT",i)];
	/* Divide the coefficient value by qval, ensuring proper rounding.
	 * Since C does not specify the direction of rounding for negative
	 * quotients, we have to force the dividend positive for portability.
	 *
	 * In most files, at least half of the output values will be zero
	 * (at default quantization settings, more like three-quarters...)
	 * so we should ensure that this case is fast.  On many machines,
	 * a comparison is enough cheaper than a divide to make a special test
	 * a win.  Since both inputs will be nonnegative, we need only test
	 * for a < b to discover whether a/b is 0.
	 * If your machine's division is fast enough, define FAST_DIVIDE.
	 */
#ifdef FAST_DIVIDE
#define DIVIDE_BY(a,b)	a /= b
#else
#define DIVIDE_BY(a,b)	if (a >= b) a /= b; else a = 0
#endif
	if (temp < 0) {
	  temp = -temp;
	  temp += qval>>1;	/* for rounding */
	  DIVIDE_BY(temp, qval);
	  temp = -temp;
	} else {
	  temp += qval>>1;	/* for rounding */
	  DIVIDE_BY(temp, qval);
	}
	(*(JCOEF *)(__boundcheck_ptr_reference(260,2,"forward_DCT",(void *)(&output_ptr[0]),(void *)(&output_ptr[i])))) = (JCOEF) temp;
      }
    }
  }
}


#ifdef DCT_FLOAT_SUPPORTED

METHODDEF(void)
forward_DCT_float (j_compress_ptr cinfo, jpeg_component_info * compptr,
		   JSAMPARRAY sample_data, JBLOCKROW coef_blocks,
		   JDIMENSION start_row, JDIMENSION start_col,
		   JDIMENSION num_blocks)
/* This version is used for floating-point DCT implementations. */
{
  /* This routine is heavily used, so it's worth coding it tightly. */
  my_fdct_ptr fdct = (my_fdct_ptr) cinfo->fdct;
__boundcheck_metadata_store((void *)(&fdct),(void *)((size_t)(&fdct)+sizeof(fdct)*8-1));

  float_DCT_method_ptr do_dct = fdct->do_float_dct;
__boundcheck_metadata_store((void *)(&do_dct),(void *)((size_t)(&do_dct)+sizeof(do_dct)*8-1));

  FAST_FLOAT * divisors = fdct->float_divisors[_RV_insert_check(0,4,279,27,"forward_DCT_float",compptr->quant_tbl_no)];
__boundcheck_metadata_store((void *)(&divisors),(void *)((size_t)(&divisors)+sizeof(divisors)*8-1));
float  
  FAST_FLOAT workspace[DCTSIZE2];__boundcheck_metadata_store(&workspace[0],&workspace[64-1]);
 /* work area for FDCT subroutine */
  JDIMENSION bi;
__boundcheck_metadata_store((void *)(&bi),(void *)((size_t)(&bi)+sizeof(bi)*8-1));


  sample_data += start_row;	/* fold in the vertical offset once */

  for (bi = 0; bi < num_blocks; bi++, start_col += DCTSIZE) {
    /* Load data into workspace, applying unsigned->signed conversion */
    { register FAST_FLOAT *workspaceptr;
      register JSAMPROW elemptr;
      register int elemr;

      workspaceptr = workspace;
__boundcheck_metadata_trans_check((void *)(workspaceptr),(void *)(workspace),(void *)(workspace));

      for (elemr = 0; elemr < DCTSIZE; elemr++) {
	elemptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(293,12,"forward_DCT_float",(void *)(&sample_data[0]),(void *)(&sample_data[elemr])))) + start_col;
#if DCTSIZE == 8		/* unroll the inner loop */
	*(float *)(__boundcheck_ptr_reference(295,15,"forward_DCT_float",(void *)(workspaceptr),(void *)(workspaceptr++))) = (FAST_FLOAT)(GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
	*(float *)(__boundcheck_ptr_reference(296,15,"forward_DCT_float",(void *)(workspaceptr),(void *)(workspaceptr++))) = (FAST_FLOAT)(GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
	*(float *)(__boundcheck_ptr_reference(297,15,"forward_DCT_float",(void *)(workspaceptr),(void *)(workspaceptr++))) = (FAST_FLOAT)(GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
	*(float *)(__boundcheck_ptr_reference(298,15,"forward_DCT_float",(void *)(workspaceptr),(void *)(workspaceptr++))) = (FAST_FLOAT)(GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
	*(float *)(__boundcheck_ptr_reference(299,15,"forward_DCT_float",(void *)(workspaceptr),(void *)(workspaceptr++))) = (FAST_FLOAT)(GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
	*(float *)(__boundcheck_ptr_reference(300,15,"forward_DCT_float",(void *)(workspaceptr),(void *)(workspaceptr++))) = (FAST_FLOAT)(GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
	*(float *)(__boundcheck_ptr_reference(301,15,"forward_DCT_float",(void *)(workspaceptr),(void *)(workspaceptr++))) = (FAST_FLOAT)(GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
	*(float *)(__boundcheck_ptr_reference(302,15,"forward_DCT_float",(void *)(workspaceptr),(void *)(workspaceptr++))) = (FAST_FLOAT)(GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
#else
	{ register int elemc;
	  for (elemc = DCTSIZE; elemc > 0; elemc--) {
	    *workspaceptr++ = (FAST_FLOAT)
	      (GETJSAMPLE(*elemptr++) - CENTERJSAMPLE);
	  }
	}
#endif
      }
    }

    /* Perform the DCT */
    (*(float_DCT_method_ptr)(__boundcheck_ptr_reference(315,7,"forward_DCT_float",(void *)(do_dct),(void *)(do_dct)))) (workspace);

    /* Quantize/descale the coefficients, and store into coef_blocks[] */
    { register FAST_FLOAT temp;
      register int i;
      register JCOEFPTR output_ptr = (*(JBLOCK *)(__boundcheck_ptr_reference(320,38,"forward_DCT_float",(void *)(&coef_blocks[0]),(void *)(&coef_blocks[bi]))));

      for (i = 0; i < DCTSIZE2; i++) {
	/* Apply the quantization and scaling factor */
	temp = workspace[_RV_insert_check(0,64,324,9,"forward_DCT_float",i)] * (*(float *)(__boundcheck_ptr_reference(324,24,"forward_DCT_float",(void *)(&divisors[0]),(void *)(&divisors[i]))));
	/* Round to nearest integer.
	 * Since C does not specify the direction of rounding for negative
	 * quotients, we have to force the dividend positive for portability.
	 * The maximum coefficient size is +-16K (for 12-bit data), so this
	 * code should work for either 16-bit or 32-bit ints.
	 */
	(*(JCOEF *)(__boundcheck_ptr_reference(331,2,"forward_DCT_float",(void *)(&output_ptr[0]),(void *)(&output_ptr[i])))) = (JCOEF) ((int) (temp + (FAST_FLOAT) 16384.5) - 16384);
      }
    }
  }
}

#endif /* DCT_FLOAT_SUPPORTED */


/*
 * Initialize FDCT manager.
 */

GLOBAL(void)
jinit_forward_dct (j_compress_ptr cinfo)
{
  my_fdct_ptr fdct;
__boundcheck_metadata_store((void *)(&fdct),(void *)((size_t)(&fdct)+sizeof(fdct)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  fdct = (my_fdct_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(351,19,"jinit_forward_dct",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_fdct_controller));
  cinfo->fdct = (struct jpeg_forward_dct *) fdct;
  fdct->pub.start_pass = start_pass_fdctmgr;

  switch (cinfo->dct_method) {
#ifdef DCT_ISLOW_SUPPORTED
  case JDCT_ISLOW:
    fdct->pub.forward_DCT = forward_DCT;
    fdct->do_dct = jpeg_fdct_islow;
    break;
#endif
#ifdef DCT_IFAST_SUPPORTED
  case JDCT_IFAST:
    fdct->pub.forward_DCT = forward_DCT;
    fdct->do_dct = jpeg_fdct_ifast;
    break;
#endif
#ifdef DCT_FLOAT_SUPPORTED
  case JDCT_FLOAT:
    fdct->pub.forward_DCT = forward_DCT_float;
    fdct->do_float_dct = jpeg_fdct_float;
    break;
#endif
  default:
    ERREXIT(cinfo, JERR_NOT_COMPILED);
    break;
  }

  /* Mark divisor tables unallocated */
  for (i = 0; i < NUM_QUANT_TBLS; i++) {
    fdct->divisors[_RV_insert_check(0,4,382,5,"jinit_forward_dct",i)] = NULL;
#ifdef DCT_FLOAT_SUPPORTED
    fdct->float_divisors[_RV_insert_check(0,4,384,5,"jinit_forward_dct",i)] = NULL;
#endif
  }
}
