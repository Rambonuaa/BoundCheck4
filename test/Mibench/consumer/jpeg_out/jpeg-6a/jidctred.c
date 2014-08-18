/*
 * jidctred.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains inverse-DCT routines that produce reduced-size output:
 * either 4x4, 2x2, or 1x1 pixels from an 8x8 DCT block.
 *
 * The implementation is based on the Loeffler, Ligtenberg and Moschytz (LL&M)
 * algorithm used in jidctint.c.  We simply replace each 8-to-8 1-D IDCT step
 * with an 8-to-4 step that produces the four averages of two adjacent outputs
 * (or an 8-to-2 step producing two averages of four outputs, for 2x2 output).
 * These steps were derived by computing the corresponding values at the end
 * of the normal LL&M code, then simplifying as much as possible.
 *
 * 1x1 is trivial: just take the DC coefficient divided by 8.
 *
 * See jidctint.c for additional comments.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jdct.h"		/* Private declarations for DCT subsystem */

#ifdef IDCT_SCALING_SUPPORTED


/*
 * This module is specialized to the case DCTSIZE = 8.
 */

#if DCTSIZE != 8
  Sorry, this code only copes with 8x8 DCTs. /* deliberate syntax err */
#endif


/* Scaling is the same as in jidctint.c. */

#if BITS_IN_JSAMPLE == 8
#define CONST_BITS  13
#define PASS1_BITS  2
#else
#define CONST_BITS  13
#define PASS1_BITS  1		/* lose a little precision to avoid overflow */
#endif

/* Some C compilers fail to reduce "FIX(constant)" at compile time, thus
 * causing a lot of useless floating-point operations at run time.
 * To get around this we use the following pre-calculated constants.
 * If you change CONST_BITS you may want to add appropriate values.
 * (With a reasonable C compiler, you can just rely on the FIX() macro...)
 */

#if CONST_BITS == 13
#define FIX_0_211164243  ((INT32)  1730)	/* FIX(0.211164243) */
#define FIX_0_509795579  ((INT32)  4176)	/* FIX(0.509795579) */
#define FIX_0_601344887  ((INT32)  4926)	/* FIX(0.601344887) */
#define FIX_0_720959822  ((INT32)  5906)	/* FIX(0.720959822) */
#define FIX_0_765366865  ((INT32)  6270)	/* FIX(0.765366865) */
#define FIX_0_850430095  ((INT32)  6967)	/* FIX(0.850430095) */
#define FIX_0_899976223  ((INT32)  7373)	/* FIX(0.899976223) */
#define FIX_1_061594337  ((INT32)  8697)	/* FIX(1.061594337) */
#define FIX_1_272758580  ((INT32)  10426)	/* FIX(1.272758580) */
#define FIX_1_451774981  ((INT32)  11893)	/* FIX(1.451774981) */
#define FIX_1_847759065  ((INT32)  15137)	/* FIX(1.847759065) */
#define FIX_2_172734803  ((INT32)  17799)	/* FIX(2.172734803) */
#define FIX_2_562915447  ((INT32)  20995)	/* FIX(2.562915447) */
#define FIX_3_624509785  ((INT32)  29692)	/* FIX(3.624509785) */
#else
#define FIX_0_211164243  FIX(0.211164243)
#define FIX_0_509795579  FIX(0.509795579)
#define FIX_0_601344887  FIX(0.601344887)
#define FIX_0_720959822  FIX(0.720959822)
#define FIX_0_765366865  FIX(0.765366865)
#define FIX_0_850430095  FIX(0.850430095)
#define FIX_0_899976223  FIX(0.899976223)
#define FIX_1_061594337  FIX(1.061594337)
#define FIX_1_272758580  FIX(1.272758580)
#define FIX_1_451774981  FIX(1.451774981)
#define FIX_1_847759065  FIX(1.847759065)
#define FIX_2_172734803  FIX(2.172734803)
#define FIX_2_562915447  FIX(2.562915447)
#define FIX_3_624509785  FIX(3.624509785)
#endif


/* Multiply an INT32 variable by an INT32 constant to yield an INT32 result.
 * For 8-bit samples with the recommended scaling, all the variable
 * and constant values involved are no more than 16 bits wide, so a
 * 16x16->32 bit multiply can be used instead of a full 32x32 multiply.
 * For 12-bit samples, a full 32-bit multiplication will be needed.
 */

#if BITS_IN_JSAMPLE == 8
#define MULTIPLY(var,const)  MULTIPLY16C16(var,const)
#else
#define MULTIPLY(var,const)  ((var) * (const))
#endif


/* Dequantize a coefficient by multiplying it by the multiplier-table
 * entry; produce an int result.  In this module, both inputs and result
 * are 16 bits or less, so either int or short multiply will work.
 */

#define DEQUANTIZE(coef,quantval)  (((ISLOW_MULT_TYPE) (coef)) * (quantval))


/*
 * Perform dequantization and inverse DCT on one block of coefficients,
 * producing a reduced-size 4x4 output block.
 */

GLOBAL(void)
jpeg_idct_4x4 (j_decompress_ptr cinfo, jpeg_component_info * compptr,
	       JCOEFPTR coef_block,
	       JSAMPARRAY output_buf, JDIMENSION output_col)
{
  INT32 tmp0;
__boundcheck_metadata_store((void *)(&tmp0),(void *)((size_t)(&tmp0)+sizeof(tmp0)*8-1));
INT32  tmp2;
__boundcheck_metadata_store((void *)(&tmp2),(void *)((size_t)(&tmp2)+sizeof(tmp2)*8-1));
INT32  tmp10;
__boundcheck_metadata_store((void *)(&tmp10),(void *)((size_t)(&tmp10)+sizeof(tmp10)*8-1));
INT32  tmp12;
__boundcheck_metadata_store((void *)(&tmp12),(void *)((size_t)(&tmp12)+sizeof(tmp12)*8-1));

  INT32 z1;
__boundcheck_metadata_store((void *)(&z1),(void *)((size_t)(&z1)+sizeof(z1)*8-1));
INT32  z2;
__boundcheck_metadata_store((void *)(&z2),(void *)((size_t)(&z2)+sizeof(z2)*8-1));
INT32  z3;
__boundcheck_metadata_store((void *)(&z3),(void *)((size_t)(&z3)+sizeof(z3)*8-1));
INT32  z4;
__boundcheck_metadata_store((void *)(&z4),(void *)((size_t)(&z4)+sizeof(z4)*8-1));

  JCOEFPTR inptr;
__boundcheck_metadata_store((void *)(&inptr),(void *)((size_t)(&inptr)+sizeof(inptr)*8-1));

  ISLOW_MULT_TYPE * quantptr;
__boundcheck_metadata_store((void *)(&quantptr),(void *)((size_t)(&quantptr)+sizeof(quantptr)*8-1));

  int * wsptr;
__boundcheck_metadata_store((void *)(&wsptr),(void *)((size_t)(&wsptr)+sizeof(wsptr)*8-1));

  JSAMPROW outptr;
__boundcheck_metadata_store((void *)(&outptr),(void *)((size_t)(&outptr)+sizeof(outptr)*8-1));

  JSAMPLE *range_limit = IDCT_range_limit(
__boundcheck_metadata_store((void *)(&range_limit),(void *)((size_t)(&range_limit)+sizeof(range_limit)*8-1));
cinfo);
  int ctr;
__boundcheck_metadata_store((void *)(&ctr),(void *)((size_t)(&ctr)+sizeof(ctr)*8-1));

  int workspace[DCTSIZE*4];__boundcheck_metadata_store(&workspace[0],&workspace[32-1]);
	/* buffers data between passes */
  SHIFT_TEMPS

  /* Pass 1: process columns from input, store into work array. */

  inptr = coef_block;
__boundcheck_metadata_trans_check((void *)(inptr),(void *)(coef_block),(void *)(coef_block));

  quantptr = (ISLOW_MULT_TYPE *) compptr->dct_table;
  wsptr = workspace;
__boundcheck_metadata_trans_check((void *)(wsptr),(void *)(workspace),(void *)(workspace));

  for (ctr = DCTSIZE; ctr > 0; inptr++, quantptr++, wsptr++, ctr--) {
    /* Don't bother to process column 4, because second pass won't use it */
    if (ctr == DCTSIZE-4)
      continue;
    if (((*(JCOEF *)(__boundcheck_ptr_reference(142,10,"jpeg_idct_4x4",(void *)(&inptr[0]),(void *)(&inptr[8 * 1])))) | (*(JCOEF *)(__boundcheck_ptr_reference(142,29,"jpeg_idct_4x4",(void *)(&inptr[0]),(void *)(&inptr[8 * 2])))) | (*(JCOEF *)(__boundcheck_ptr_reference(142,48,"jpeg_idct_4x4",(void *)(&inptr[0]),(void *)(&inptr[8 * 3])))) |
	 (*(JCOEF *)(__boundcheck_ptr_reference(143,3,"jpeg_idct_4x4",(void *)(&inptr[0]),(void *)(&inptr[8 * 5])))) | (*(JCOEF *)(__boundcheck_ptr_reference(143,22,"jpeg_idct_4x4",(void *)(&inptr[0]),(void *)(&inptr[8 * 6])))) | (*(JCOEF *)(__boundcheck_ptr_reference(143,41,"jpeg_idct_4x4",(void *)(&inptr[0]),(void *)(&inptr[8 * 7]))))) == 0) {
      /* AC terms all zero; we need not examine term 4 for 4x4 output */
      int dcval = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]) << PASS1_BITS;
__boundcheck_metadata_store((void *)(&dcval),(void *)((size_t)(&dcval)+sizeof(dcval)*8-1));

      
      (*(int *)(__boundcheck_ptr_reference(147,7,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 0])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(148,7,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 1])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(149,7,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 2])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(150,7,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 3])))) = dcval;
      
      continue;
    }
    
    /* Even part */
    
    tmp0 = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);
    tmp0 <<= (CONST_BITS+1);
    
    z2 = DEQUANTIZE(inptr[DCTSIZE*2], quantptr[DCTSIZE*2]);
    z3 = DEQUANTIZE(inptr[DCTSIZE*6], quantptr[DCTSIZE*6]);

    tmp2 = MULTIPLY(z2, FIX_1_847759065) + MULTIPLY(z3, - FIX_0_765366865);
    
    tmp10 = tmp0 + tmp2;
    tmp12 = tmp0 - tmp2;
    
    /* Odd part */
    
    z1 = DEQUANTIZE(inptr[DCTSIZE*7], quantptr[DCTSIZE*7]);
    z2 = DEQUANTIZE(inptr[DCTSIZE*5], quantptr[DCTSIZE*5]);
    z3 = DEQUANTIZE(inptr[DCTSIZE*3], quantptr[DCTSIZE*3]);
    z4 = DEQUANTIZE(inptr[DCTSIZE*1], quantptr[DCTSIZE*1]);
    
    tmp0 = MULTIPLY(z1, - FIX_0_211164243) /* sqrt(2) * (c3-c1) */
	 + MULTIPLY(z2, FIX_1_451774981) /* sqrt(2) * (c3+c7) */
	 + MULTIPLY(z3, - FIX_2_172734803) /* sqrt(2) * (-c1-c5) */
	 + MULTIPLY(z4, FIX_1_061594337); /* sqrt(2) * (c5+c7) */
    
    tmp2 = MULTIPLY(z1, - FIX_0_509795579) /* sqrt(2) * (c7-c5) */
	 + MULTIPLY(z2, - FIX_0_601344887) /* sqrt(2) * (c5-c1) */
	 + MULTIPLY(z3, FIX_0_899976223) /* sqrt(2) * (c3-c7) */
	 + MULTIPLY(z4, FIX_2_562915447); /* sqrt(2) * (c1+c3) */

    /* Final output stage */
    
    (*(int *)(__boundcheck_ptr_reference(187,5,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 0])))) = (int) DESCALE(tmp10 + tmp2, CONST_BITS-PASS1_BITS+1);
    (*(int *)(__boundcheck_ptr_reference(188,5,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 3])))) = (int) DESCALE(tmp10 - tmp2, CONST_BITS-PASS1_BITS+1);
    (*(int *)(__boundcheck_ptr_reference(189,5,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 1])))) = (int) DESCALE(tmp12 + tmp0, CONST_BITS-PASS1_BITS+1);
    (*(int *)(__boundcheck_ptr_reference(190,5,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 2])))) = (int) DESCALE(tmp12 - tmp0, CONST_BITS-PASS1_BITS+1);
  }
  
  /* Pass 2: process 4 rows from work array, store into output array. */

  wsptr = workspace;
__boundcheck_metadata_trans_check((void *)(wsptr),(void *)(workspace),(void *)(workspace));

  for (ctr = 0; ctr < 4; ctr++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(197,14,"jpeg_idct_4x4",(void *)(&output_buf[0]),(void *)(&output_buf[ctr])))) + output_col;
    /* It's not clear whether a zero row test is worthwhile here ... */

#ifndef NO_ZERO_ROW_TEST
    if (((*(int *)(__boundcheck_ptr_reference(201,10,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[1])))) | (*(int *)(__boundcheck_ptr_reference(201,21,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[2])))) | (*(int *)(__boundcheck_ptr_reference(201,32,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[3])))) | (*(int *)(__boundcheck_ptr_reference(201,43,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[5])))) | (*(int *)(__boundcheck_ptr_reference(201,54,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[6])))) |
	 (*(int *)(__boundcheck_ptr_reference(202,3,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[7]))))) == 0) {
      /* AC terms all zero */
      JSAMPLE dcval = (*(JSAMPLE *)(__boundcheck_ptr_reference(204,23,"jpeg_idct_4x4",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)wsptr[0]) + (((INT32)1) << ((2 + 3) - 1))) >> (2 + 3)) & (255 * 4 + 3)]))));
__boundcheck_metadata_store((void *)(&dcval),(void *)((size_t)(&dcval)+sizeof(dcval)*8-1));

      
      (*(JSAMPLE *)(__boundcheck_ptr_reference(207,7,"jpeg_idct_4x4",(void *)(&outptr[0]),(void *)(&outptr[0])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(208,7,"jpeg_idct_4x4",(void *)(&outptr[0]),(void *)(&outptr[1])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(209,7,"jpeg_idct_4x4",(void *)(&outptr[0]),(void *)(&outptr[2])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(210,7,"jpeg_idct_4x4",(void *)(&outptr[0]),(void *)(&outptr[3])))) = dcval;
      
      wsptr += DCTSIZE;		/* advance pointer to next row */
      continue;
    }
#endif
    
    /* Even part */
    
    tmp0 = ((INT32) (*(int *)(__boundcheck_ptr_reference(219,21,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[0]))))) << (CONST_BITS+1);
    
    tmp2 = MULTIPLY((INT32) wsptr[2], FIX_1_847759065)
	 + MULTIPLY((INT32) wsptr[6], - FIX_0_765366865);
    
    tmp10 = tmp0 + tmp2;
    tmp12 = tmp0 - tmp2;
    
    /* Odd part */
    
    z1 = (INT32) (*(int *)(__boundcheck_ptr_reference(229,18,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[7]))));
    z2 = (INT32) (*(int *)(__boundcheck_ptr_reference(230,18,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[5]))));
    z3 = (INT32) (*(int *)(__boundcheck_ptr_reference(231,18,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[3]))));
    z4 = (INT32) (*(int *)(__boundcheck_ptr_reference(232,18,"jpeg_idct_4x4",(void *)(&wsptr[0]),(void *)(&wsptr[1]))));
    
    tmp0 = MULTIPLY(z1, - FIX_0_211164243) /* sqrt(2) * (c3-c1) */
	 + MULTIPLY(z2, FIX_1_451774981) /* sqrt(2) * (c3+c7) */
	 + MULTIPLY(z3, - FIX_2_172734803) /* sqrt(2) * (-c1-c5) */
	 + MULTIPLY(z4, FIX_1_061594337); /* sqrt(2) * (c5+c7) */
    
    tmp2 = MULTIPLY(z1, - FIX_0_509795579) /* sqrt(2) * (c7-c5) */
	 + MULTIPLY(z2, - FIX_0_601344887) /* sqrt(2) * (c5-c1) */
	 + MULTIPLY(z3, FIX_0_899976223) /* sqrt(2) * (c3-c7) */
	 + MULTIPLY(z4, FIX_2_562915447); /* sqrt(2) * (c1+c3) */

    /* Final output stage */
    
    (*(JSAMPLE *)(__boundcheck_ptr_reference(246,5,"jpeg_idct_4x4",(void *)(&outptr[0]),(void *)(&outptr[0])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(246,17,"jpeg_idct_4x4",(void *)(&range_limit[0]),(void *)(&range_limit[(int)(((tmp10 + tmp2) + (((INT32)1) << ((13 + 2 + 3 + 1) - 1))) >> (13 + 2 + 3 + 1)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(249,5,"jpeg_idct_4x4",(void *)(&outptr[0]),(void *)(&outptr[3])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(249,17,"jpeg_idct_4x4",(void *)(&range_limit[0]),(void *)(&range_limit[(int)(((tmp10 - tmp2) + (((INT32)1) << ((13 + 2 + 3 + 1) - 1))) >> (13 + 2 + 3 + 1)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(252,5,"jpeg_idct_4x4",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(252,17,"jpeg_idct_4x4",(void *)(&range_limit[0]),(void *)(&range_limit[(int)(((tmp12 + tmp0) + (((INT32)1) << ((13 + 2 + 3 + 1) - 1))) >> (13 + 2 + 3 + 1)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(255,5,"jpeg_idct_4x4",(void *)(&outptr[0]),(void *)(&outptr[2])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(255,17,"jpeg_idct_4x4",(void *)(&range_limit[0]),(void *)(&range_limit[(int)(((tmp12 - tmp0) + (((INT32)1) << ((13 + 2 + 3 + 1) - 1))) >> (13 + 2 + 3 + 1)) & (255 * 4 + 3)]))));
    
    wsptr += DCTSIZE;		/* advance pointer to next row */
  }
}


/*
 * Perform dequantization and inverse DCT on one block of coefficients,
 * producing a reduced-size 2x2 output block.
 */

GLOBAL(void)
jpeg_idct_2x2 (j_decompress_ptr cinfo, jpeg_component_info * compptr,
	       JCOEFPTR coef_block,
	       JSAMPARRAY output_buf, JDIMENSION output_col)
{
  INT32 tmp0;
__boundcheck_metadata_store((void *)(&tmp0),(void *)((size_t)(&tmp0)+sizeof(tmp0)*8-1));
INT32  tmp10;
__boundcheck_metadata_store((void *)(&tmp10),(void *)((size_t)(&tmp10)+sizeof(tmp10)*8-1));
INT32  z1;
__boundcheck_metadata_store((void *)(&z1),(void *)((size_t)(&z1)+sizeof(z1)*8-1));

  JCOEFPTR inptr;
__boundcheck_metadata_store((void *)(&inptr),(void *)((size_t)(&inptr)+sizeof(inptr)*8-1));

  ISLOW_MULT_TYPE * quantptr;
__boundcheck_metadata_store((void *)(&quantptr),(void *)((size_t)(&quantptr)+sizeof(quantptr)*8-1));

  int * wsptr;
__boundcheck_metadata_store((void *)(&wsptr),(void *)((size_t)(&wsptr)+sizeof(wsptr)*8-1));

  JSAMPROW outptr;
__boundcheck_metadata_store((void *)(&outptr),(void *)((size_t)(&outptr)+sizeof(outptr)*8-1));

  JSAMPLE *range_limit = IDCT_range_limit(
__boundcheck_metadata_store((void *)(&range_limit),(void *)((size_t)(&range_limit)+sizeof(range_limit)*8-1));
cinfo);
  int ctr;
__boundcheck_metadata_store((void *)(&ctr),(void *)((size_t)(&ctr)+sizeof(ctr)*8-1));

  int workspace[DCTSIZE*2];__boundcheck_metadata_store(&workspace[0],&workspace[16-1]);
	/* buffers data between passes */
  SHIFT_TEMPS

  /* Pass 1: process columns from input, store into work array. */

  inptr = coef_block;
__boundcheck_metadata_trans_check((void *)(inptr),(void *)(coef_block),(void *)(coef_block));

  quantptr = (ISLOW_MULT_TYPE *) compptr->dct_table;
  wsptr = workspace;
__boundcheck_metadata_trans_check((void *)(wsptr),(void *)(workspace),(void *)(workspace));

  for (ctr = DCTSIZE; ctr > 0; inptr++, quantptr++, wsptr++, ctr--) {
    /* Don't bother to process columns 2,4,6 */
    if (ctr == DCTSIZE-2 || ctr == DCTSIZE-4 || ctr == DCTSIZE-6)
      continue;
    if (((*(JCOEF *)(__boundcheck_ptr_reference(293,10,"jpeg_idct_2x2",(void *)(&inptr[0]),(void *)(&inptr[8 * 1])))) | (*(JCOEF *)(__boundcheck_ptr_reference(293,29,"jpeg_idct_2x2",(void *)(&inptr[0]),(void *)(&inptr[8 * 3])))) |
	 (*(JCOEF *)(__boundcheck_ptr_reference(294,3,"jpeg_idct_2x2",(void *)(&inptr[0]),(void *)(&inptr[8 * 5])))) | (*(JCOEF *)(__boundcheck_ptr_reference(294,22,"jpeg_idct_2x2",(void *)(&inptr[0]),(void *)(&inptr[8 * 7]))))) == 0) {
      /* AC terms all zero; we need not examine terms 2,4,6 for 2x2 output */
      int dcval = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]) << PASS1_BITS;
__boundcheck_metadata_store((void *)(&dcval),(void *)((size_t)(&dcval)+sizeof(dcval)*8-1));

      
      (*(int *)(__boundcheck_ptr_reference(298,7,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 0])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(299,7,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 1])))) = dcval;
      
      continue;
    }
    
    /* Even part */
    
    z1 = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);
    tmp10 = z1 << (CONST_BITS+2);
    
    /* Odd part */

    z1 = DEQUANTIZE(inptr[DCTSIZE*7], quantptr[DCTSIZE*7]);
    tmp0 = MULTIPLY(z1, - FIX_0_720959822); /* sqrt(2) * (c7-c5+c3-c1) */
    z1 = DEQUANTIZE(inptr[DCTSIZE*5], quantptr[DCTSIZE*5]);
    tmp0 += MULTIPLY(z1, FIX_0_850430095); /* sqrt(2) * (-c1+c3+c5+c7) */
    z1 = DEQUANTIZE(inptr[DCTSIZE*3], quantptr[DCTSIZE*3]);
    tmp0 += MULTIPLY(z1, - FIX_1_272758580); /* sqrt(2) * (-c1+c3-c5-c7) */
    z1 = DEQUANTIZE(inptr[DCTSIZE*1], quantptr[DCTSIZE*1]);
    tmp0 += MULTIPLY(z1, FIX_3_624509785); /* sqrt(2) * (c1+c3+c5+c7) */

    /* Final output stage */
    
    (*(int *)(__boundcheck_ptr_reference(322,5,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 0])))) = (int) DESCALE(tmp10 + tmp0, CONST_BITS-PASS1_BITS+2);
    (*(int *)(__boundcheck_ptr_reference(323,5,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 1])))) = (int) DESCALE(tmp10 - tmp0, CONST_BITS-PASS1_BITS+2);
  }
  
  /* Pass 2: process 2 rows from work array, store into output array. */

  wsptr = workspace;
__boundcheck_metadata_trans_check((void *)(wsptr),(void *)(workspace),(void *)(workspace));

  for (ctr = 0; ctr < 2; ctr++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(330,14,"jpeg_idct_2x2",(void *)(&output_buf[0]),(void *)(&output_buf[ctr])))) + output_col;
    /* It's not clear whether a zero row test is worthwhile here ... */

#ifndef NO_ZERO_ROW_TEST
    if (((*(int *)(__boundcheck_ptr_reference(334,10,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[1])))) | (*(int *)(__boundcheck_ptr_reference(334,21,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[3])))) | (*(int *)(__boundcheck_ptr_reference(334,32,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[5])))) | (*(int *)(__boundcheck_ptr_reference(334,43,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[7]))))) == 0) {
      /* AC terms all zero */
      JSAMPLE dcval = (*(JSAMPLE *)(__boundcheck_ptr_reference(336,23,"jpeg_idct_2x2",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)wsptr[0]) + (((INT32)1) << ((2 + 3) - 1))) >> (2 + 3)) & (255 * 4 + 3)]))));
__boundcheck_metadata_store((void *)(&dcval),(void *)((size_t)(&dcval)+sizeof(dcval)*8-1));

      
      (*(JSAMPLE *)(__boundcheck_ptr_reference(339,7,"jpeg_idct_2x2",(void *)(&outptr[0]),(void *)(&outptr[0])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(340,7,"jpeg_idct_2x2",(void *)(&outptr[0]),(void *)(&outptr[1])))) = dcval;
      
      wsptr += DCTSIZE;		/* advance pointer to next row */
      continue;
    }
#endif
    
    /* Even part */
    
    tmp10 = ((INT32) (*(int *)(__boundcheck_ptr_reference(349,22,"jpeg_idct_2x2",(void *)(&wsptr[0]),(void *)(&wsptr[0]))))) << (CONST_BITS+2);
    
    /* Odd part */

    tmp0 = MULTIPLY((INT32) wsptr[7], - FIX_0_720959822) /* sqrt(2) * (c7-c5+c3-c1) */
	 + MULTIPLY((INT32) wsptr[5], FIX_0_850430095) /* sqrt(2) * (-c1+c3+c5+c7) */
	 + MULTIPLY((INT32) wsptr[3], - FIX_1_272758580) /* sqrt(2) * (-c1+c3-c5-c7) */
	 + MULTIPLY((INT32) wsptr[1], FIX_3_624509785); /* sqrt(2) * (c1+c3+c5+c7) */

    /* Final output stage */
    
    (*(JSAMPLE *)(__boundcheck_ptr_reference(360,5,"jpeg_idct_2x2",(void *)(&outptr[0]),(void *)(&outptr[0])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(360,17,"jpeg_idct_2x2",(void *)(&range_limit[0]),(void *)(&range_limit[(int)(((tmp10 + tmp0) + (((INT32)1) << ((13 + 2 + 3 + 2) - 1))) >> (13 + 2 + 3 + 2)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(363,5,"jpeg_idct_2x2",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(363,17,"jpeg_idct_2x2",(void *)(&range_limit[0]),(void *)(&range_limit[(int)(((tmp10 - tmp0) + (((INT32)1) << ((13 + 2 + 3 + 2) - 1))) >> (13 + 2 + 3 + 2)) & (255 * 4 + 3)]))));
    
    wsptr += DCTSIZE;		/* advance pointer to next row */
  }
}


/*
 * Perform dequantization and inverse DCT on one block of coefficients,
 * producing a reduced-size 1x1 output block.
 */

GLOBAL(void)
jpeg_idct_1x1 (j_decompress_ptr cinfo, jpeg_component_info * compptr,
	       JCOEFPTR coef_block,
	       JSAMPARRAY output_buf, JDIMENSION output_col)
{
  int dcval;
__boundcheck_metadata_store((void *)(&dcval),(void *)((size_t)(&dcval)+sizeof(dcval)*8-1));

  ISLOW_MULT_TYPE * quantptr;
__boundcheck_metadata_store((void *)(&quantptr),(void *)((size_t)(&quantptr)+sizeof(quantptr)*8-1));

  JSAMPLE *range_limit = IDCT_range_limit(
__boundcheck_metadata_store((void *)(&range_limit),(void *)((size_t)(&range_limit)+sizeof(range_limit)*8-1));
cinfo);
  SHIFT_TEMPS

  /* We hardly need an inverse DCT routine for this: just take the
   * average pixel value, which is one-eighth of the DC coefficient.
   */
  quantptr = (ISLOW_MULT_TYPE *) compptr->dct_table;
  dcval = DEQUANTIZE(coef_block[0], quantptr[0]);
  dcval = (int) DESCALE((INT32) dcval, 3);

  (*(JSAMPLE *)(__boundcheck_ptr_reference(394,3,"jpeg_idct_1x1",(void *)(&output_buf[0][0]),(void *)(&output_buf[0][output_col])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(394,31,"jpeg_idct_1x1",(void *)(&range_limit[0]),(void *)(&range_limit[dcval & (255 * 4 + 3)]))));
}

#endif /* IDCT_SCALING_SUPPORTED */
