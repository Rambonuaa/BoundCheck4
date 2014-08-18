/*
 * jidctfst.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a fast, not so accurate integer implementation of the
 * inverse DCT (Discrete Cosine Transform).  In the IJG code, this routine
 * must also perform dequantization of the input coefficients.
 *
 * A 2-D IDCT can be done by 1-D IDCT on each column followed by 1-D IDCT
 * on each row (or vice versa, but it's more convenient to emit a row at
 * a time).  Direct algorithms are also available, but they are much more
 * complex and seem not to be any faster when reduced to code.
 *
 * This implementation is based on Arai, Agui, and Nakajima's algorithm for
 * scaled DCT.  Their original paper (Trans. IEICE E-71(11):1095) is in
 * Japanese, but the algorithm is described in the Pennebaker & Mitchell
 * JPEG textbook (see REFERENCES section in file README).  The following code
 * is based directly on figure 4-8 in P&M.
 * While an 8-point DCT cannot be done in less than 11 multiplies, it is
 * possible to arrange the computation so that many of the multiplies are
 * simple scalings of the final outputs.  These multiplies can then be
 * folded into the multiplications or divisions by the JPEG quantization
 * table entries.  The AA&N method leaves only 5 multiplies and 29 adds
 * to be done in the DCT itself.
 * The primary disadvantage of this method is that with fixed-point math,
 * accuracy is lost due to imprecise representation of the scaled
 * quantization values.  The smaller the quantization table entry, the less
 * precise the scaled value, so this implementation does worse with high-
 * quality-setting files than with low-quality ones.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jdct.h"		/* Private declarations for DCT subsystem */

#ifdef DCT_IFAST_SUPPORTED


/*
 * This module is specialized to the case DCTSIZE = 8.
 */

#if DCTSIZE != 8
  Sorry, this code only copes with 8x8 DCTs. /* deliberate syntax err */
#endif


/* Scaling decisions are generally the same as in the LL&M algorithm;
 * see jidctint.c for more details.  However, we choose to descale
 * (right shift) multiplication products as soon as they are formed,
 * rather than carrying additional fractional bits into subsequent additions.
 * This compromises accuracy slightly, but it lets us save a few shifts.
 * More importantly, 16-bit arithmetic is then adequate (for 8-bit samples)
 * everywhere except in the multiplications proper; this saves a good deal
 * of work on 16-bit-int machines.
 *
 * The dequantized coefficients are not integers because the AA&N scaling
 * factors have been incorporated.  We represent them scaled up by PASS1_BITS,
 * so that the first and second IDCT rounds have the same input scaling.
 * For 8-bit JSAMPLEs, we choose IFAST_SCALE_BITS = PASS1_BITS so as to
 * avoid a descaling shift; this compromises accuracy rather drastically
 * for small quantization table entries, but it saves a lot of shifts.
 * For 12-bit JSAMPLEs, there's no hope of using 16x16 multiplies anyway,
 * so we use a much larger scaling factor to preserve accuracy.
 *
 * A final compromise is to represent the multiplicative constants to only
 * 8 fractional bits, rather than 13.  This saves some shifting work on some
 * machines, and may also reduce the cost of multiplication (since there
 * are fewer one-bits in the constants).
 */

#if BITS_IN_JSAMPLE == 8
#define CONST_BITS  8
#define PASS1_BITS  2
#else
#define CONST_BITS  8
#define PASS1_BITS  1		/* lose a little precision to avoid overflow */
#endif

/* Some C compilers fail to reduce "FIX(constant)" at compile time, thus
 * causing a lot of useless floating-point operations at run time.
 * To get around this we use the following pre-calculated constants.
 * If you change CONST_BITS you may want to add appropriate values.
 * (With a reasonable C compiler, you can just rely on the FIX() macro...)
 */

#if CONST_BITS == 8
#define FIX_1_082392200  ((INT32)  277)		/* FIX(1.082392200) */
#define FIX_1_414213562  ((INT32)  362)		/* FIX(1.414213562) */
#define FIX_1_847759065  ((INT32)  473)		/* FIX(1.847759065) */
#define FIX_2_613125930  ((INT32)  669)		/* FIX(2.613125930) */
#else
#define FIX_1_082392200  FIX(1.082392200)
#define FIX_1_414213562  FIX(1.414213562)
#define FIX_1_847759065  FIX(1.847759065)
#define FIX_2_613125930  FIX(2.613125930)
#endif


/* We can gain a little more speed, with a further compromise in accuracy,
 * by omitting the addition in a descaling shift.  This yields an incorrectly
 * rounded result half the time...
 */

#ifndef USE_ACCURATE_ROUNDING
#undef DESCALE
#define DESCALE(x,n)  RIGHT_SHIFT(x, n)
#endif


/* Multiply a DCTELEM variable by an INT32 constant, and immediately
 * descale to yield a DCTELEM result.
 */

#define MULTIPLY(var,const)  ((DCTELEM) DESCALE((var) * (const), CONST_BITS))


/* Dequantize a coefficient by multiplying it by the multiplier-table
 * entry; produce a DCTELEM result.  For 8-bit data a 16x16->16
 * multiplication will do.  For 12-bit data, the multiplier table is
 * declared INT32, so a 32-bit multiply will be used.
 */

#if BITS_IN_JSAMPLE == 8
#define DEQUANTIZE(coef,quantval)  (((IFAST_MULT_TYPE) (coef)) * (quantval))
#else
#define DEQUANTIZE(coef,quantval)  \
	DESCALE((coef)*(quantval), IFAST_SCALE_BITS-PASS1_BITS)
#endif


/* Like DESCALE, but applies to a DCTELEM and produces an int.
 * We assume that int right shift is unsigned if INT32 right shift is.
 */

#ifdef RIGHT_SHIFT_IS_UNSIGNED
#define ISHIFT_TEMPS	DCTELEM ishift_temp;
#if BITS_IN_JSAMPLE == 8
#define DCTELEMBITS  16		/* DCTELEM may be 16 or 32 bits */
#else
#define DCTELEMBITS  32		/* DCTELEM must be 32 bits */
#endif
#define IRIGHT_SHIFT(x,shft)  \
    ((ishift_temp = (x)) < 0 ? \
     (ishift_temp >> (shft)) | ((~((DCTELEM) 0)) << (DCTELEMBITS-(shft))) : \
     (ishift_temp >> (shft)))
#else
#define ISHIFT_TEMPS
#define IRIGHT_SHIFT(x,shft)	((x) >> (shft))
#endif

#ifdef USE_ACCURATE_ROUNDING
#define IDESCALE(x,n)  ((int) IRIGHT_SHIFT((x) + (1 << ((n)-1)), n))
#else
#define IDESCALE(x,n)  ((int) IRIGHT_SHIFT(x, n))
#endif


/*
 * Perform dequantization and inverse DCT on one block of coefficients.
 */

GLOBAL(void)
jpeg_idct_ifast (j_decompress_ptr cinfo, jpeg_component_info * compptr,
		 JCOEFPTR coef_block,
		 JSAMPARRAY output_buf, JDIMENSION output_col)
{
  DCTELEM tmp0;
__boundcheck_metadata_store((void *)(&tmp0),(void *)((size_t)(&tmp0)+sizeof(tmp0)*8-1));
DCTELEM  tmp1;
__boundcheck_metadata_store((void *)(&tmp1),(void *)((size_t)(&tmp1)+sizeof(tmp1)*8-1));
DCTELEM  tmp2;
__boundcheck_metadata_store((void *)(&tmp2),(void *)((size_t)(&tmp2)+sizeof(tmp2)*8-1));
DCTELEM  tmp3;
__boundcheck_metadata_store((void *)(&tmp3),(void *)((size_t)(&tmp3)+sizeof(tmp3)*8-1));
DCTELEM  tmp4;
__boundcheck_metadata_store((void *)(&tmp4),(void *)((size_t)(&tmp4)+sizeof(tmp4)*8-1));
DCTELEM  tmp5;
__boundcheck_metadata_store((void *)(&tmp5),(void *)((size_t)(&tmp5)+sizeof(tmp5)*8-1));
DCTELEM  tmp6;
__boundcheck_metadata_store((void *)(&tmp6),(void *)((size_t)(&tmp6)+sizeof(tmp6)*8-1));
DCTELEM  tmp7;
__boundcheck_metadata_store((void *)(&tmp7),(void *)((size_t)(&tmp7)+sizeof(tmp7)*8-1));

  DCTELEM tmp10;
__boundcheck_metadata_store((void *)(&tmp10),(void *)((size_t)(&tmp10)+sizeof(tmp10)*8-1));
DCTELEM  tmp11;
__boundcheck_metadata_store((void *)(&tmp11),(void *)((size_t)(&tmp11)+sizeof(tmp11)*8-1));
DCTELEM  tmp12;
__boundcheck_metadata_store((void *)(&tmp12),(void *)((size_t)(&tmp12)+sizeof(tmp12)*8-1));
DCTELEM  tmp13;
__boundcheck_metadata_store((void *)(&tmp13),(void *)((size_t)(&tmp13)+sizeof(tmp13)*8-1));

  DCTELEM z5;
__boundcheck_metadata_store((void *)(&z5),(void *)((size_t)(&z5)+sizeof(z5)*8-1));
DCTELEM  z10;
__boundcheck_metadata_store((void *)(&z10),(void *)((size_t)(&z10)+sizeof(z10)*8-1));
DCTELEM  z11;
__boundcheck_metadata_store((void *)(&z11),(void *)((size_t)(&z11)+sizeof(z11)*8-1));
DCTELEM  z12;
__boundcheck_metadata_store((void *)(&z12),(void *)((size_t)(&z12)+sizeof(z12)*8-1));
DCTELEM  z13;
__boundcheck_metadata_store((void *)(&z13),(void *)((size_t)(&z13)+sizeof(z13)*8-1));

  JCOEFPTR inptr;
__boundcheck_metadata_store((void *)(&inptr),(void *)((size_t)(&inptr)+sizeof(inptr)*8-1));

  IFAST_MULT_TYPE * quantptr;
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

  int workspace[DCTSIZE2];__boundcheck_metadata_store(&workspace[0],&workspace[64-1]);
	/* buffers data between passes */
  SHIFT_TEMPS			/* for DESCALE */
  ISHIFT_TEMPS			/* for IDESCALE */

  /* Pass 1: process columns from input, store into work array. */

  inptr = coef_block;
__boundcheck_metadata_trans_check((void *)(inptr),(void *)(coef_block),(void *)(coef_block));

  quantptr = (IFAST_MULT_TYPE *) compptr->dct_table;
  wsptr = workspace;
__boundcheck_metadata_trans_check((void *)(wsptr),(void *)(workspace),(void *)(workspace));

  for (ctr = DCTSIZE; ctr > 0; ctr--) {
    /* Due to quantization, we will usually find that many of the input
     * coefficients are zero, especially the AC terms.  We can exploit this
     * by short-circuiting the IDCT calculation for any column in which all
     * the AC terms are zero.  In that case each output is equal to the
     * DC coefficient (with scale factor as needed).
     * With typical images and quantization tables, half or more of the
     * column DCT calculations can be simplified this way.
     */
    
    if (((*(JCOEF *)(__boundcheck_ptr_reference(200,10,"jpeg_idct_ifast",(void *)(&inptr[0]),(void *)(&inptr[8 * 1])))) | (*(JCOEF *)(__boundcheck_ptr_reference(200,29,"jpeg_idct_ifast",(void *)(&inptr[0]),(void *)(&inptr[8 * 2])))) | (*(JCOEF *)(__boundcheck_ptr_reference(200,48,"jpeg_idct_ifast",(void *)(&inptr[0]),(void *)(&inptr[8 * 3])))) |
	 (*(JCOEF *)(__boundcheck_ptr_reference(201,3,"jpeg_idct_ifast",(void *)(&inptr[0]),(void *)(&inptr[8 * 4])))) | (*(JCOEF *)(__boundcheck_ptr_reference(201,22,"jpeg_idct_ifast",(void *)(&inptr[0]),(void *)(&inptr[8 * 5])))) | (*(JCOEF *)(__boundcheck_ptr_reference(201,41,"jpeg_idct_ifast",(void *)(&inptr[0]),(void *)(&inptr[8 * 6])))) |
	 (*(JCOEF *)(__boundcheck_ptr_reference(202,3,"jpeg_idct_ifast",(void *)(&inptr[0]),(void *)(&inptr[8 * 7]))))) == 0) {
      /* AC terms all zero */
      int dcval = (int) DEQUANTIZE(
__boundcheck_metadata_store((void *)(&dcval),(void *)((size_t)(&dcval)+sizeof(dcval)*8-1));
inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);

      (*(int *)(__boundcheck_ptr_reference(206,7,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 0])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(207,7,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 1])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(208,7,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 2])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(209,7,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 3])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(210,7,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 4])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(211,7,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 5])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(212,7,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 6])))) = dcval;
      (*(int *)(__boundcheck_ptr_reference(213,7,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 7])))) = dcval;
      
      inptr++;			/* advance pointers to next column */
      quantptr++;
      wsptr++;
      continue;
    }
    
    /* Even part */

    tmp0 = DEQUANTIZE(inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);
    tmp1 = DEQUANTIZE(inptr[DCTSIZE*2], quantptr[DCTSIZE*2]);
    tmp2 = DEQUANTIZE(inptr[DCTSIZE*4], quantptr[DCTSIZE*4]);
    tmp3 = DEQUANTIZE(inptr[DCTSIZE*6], quantptr[DCTSIZE*6]);

    tmp10 = tmp0 + tmp2;	/* phase 3 */
    tmp11 = tmp0 - tmp2;

    tmp13 = tmp1 + tmp3;	/* phases 5-3 */
    tmp12 = MULTIPLY(tmp1 - tmp3, FIX_1_414213562) - tmp13; /* 2*c4 */

    tmp0 = tmp10 + tmp13;	/* phase 2 */
    tmp3 = tmp10 - tmp13;
    tmp1 = tmp11 + tmp12;
    tmp2 = tmp11 - tmp12;
    
    /* Odd part */

    tmp4 = DEQUANTIZE(inptr[DCTSIZE*1], quantptr[DCTSIZE*1]);
    tmp5 = DEQUANTIZE(inptr[DCTSIZE*3], quantptr[DCTSIZE*3]);
    tmp6 = DEQUANTIZE(inptr[DCTSIZE*5], quantptr[DCTSIZE*5]);
    tmp7 = DEQUANTIZE(inptr[DCTSIZE*7], quantptr[DCTSIZE*7]);

    z13 = tmp6 + tmp5;		/* phase 6 */
    z10 = tmp6 - tmp5;
    z11 = tmp4 + tmp7;
    z12 = tmp4 - tmp7;

    tmp7 = z11 + z13;		/* phase 5 */
    tmp11 = MULTIPLY(z11 - z13, FIX_1_414213562); /* 2*c4 */

    z5 = MULTIPLY(z10 + z12, FIX_1_847759065); /* 2*c2 */
    tmp10 = MULTIPLY(z12, FIX_1_082392200) - z5; /* 2*(c2-c6) */
    tmp12 = MULTIPLY(z10, - FIX_2_613125930) + z5; /* -2*(c2+c6) */

    tmp6 = tmp12 - tmp7;	/* phase 2 */
    tmp5 = tmp11 - tmp6;
    tmp4 = tmp10 + tmp5;

    (*(int *)(__boundcheck_ptr_reference(262,5,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 0])))) = (int) (tmp0 + tmp7);
    (*(int *)(__boundcheck_ptr_reference(263,5,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 7])))) = (int) (tmp0 - tmp7);
    (*(int *)(__boundcheck_ptr_reference(264,5,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 1])))) = (int) (tmp1 + tmp6);
    (*(int *)(__boundcheck_ptr_reference(265,5,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 6])))) = (int) (tmp1 - tmp6);
    (*(int *)(__boundcheck_ptr_reference(266,5,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 2])))) = (int) (tmp2 + tmp5);
    (*(int *)(__boundcheck_ptr_reference(267,5,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 5])))) = (int) (tmp2 - tmp5);
    (*(int *)(__boundcheck_ptr_reference(268,5,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 4])))) = (int) (tmp3 + tmp4);
    (*(int *)(__boundcheck_ptr_reference(269,5,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 3])))) = (int) (tmp3 - tmp4);

    inptr++;			/* advance pointers to next column */
    quantptr++;
    wsptr++;
  }
  
  /* Pass 2: process rows from work array, store into output array. */
  /* Note that we must descale the results by a factor of 8 == 2**3, */
  /* and also undo the PASS1_BITS scaling. */

  wsptr = workspace;
__boundcheck_metadata_trans_check((void *)(wsptr),(void *)(workspace),(void *)(workspace));

  for (ctr = 0; ctr < DCTSIZE; ctr++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(282,14,"jpeg_idct_ifast",(void *)(&output_buf[0]),(void *)(&output_buf[ctr])))) + output_col;
    /* Rows of zeroes can be exploited in the same way as we did with columns.
     * However, the column calculation has created many nonzero AC terms, so
     * the simplification applies less often (typically 5% to 10% of the time).
     * On machines with very fast multiplication, it's possible that the
     * test takes more time than it's worth.  In that case this section
     * may be commented out.
     */
    
#ifndef NO_ZERO_ROW_TEST
    if (((*(int *)(__boundcheck_ptr_reference(292,10,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[1])))) | (*(int *)(__boundcheck_ptr_reference(292,21,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[2])))) | (*(int *)(__boundcheck_ptr_reference(292,32,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[3])))) | (*(int *)(__boundcheck_ptr_reference(292,43,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[4])))) | (*(int *)(__boundcheck_ptr_reference(292,54,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[5])))) | (*(int *)(__boundcheck_ptr_reference(292,65,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[6])))) |
	 (*(int *)(__boundcheck_ptr_reference(293,3,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[7]))))) == 0) {
      /* AC terms all zero */
      JSAMPLE dcval = (*(JSAMPLE *)(__boundcheck_ptr_reference(295,23,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((wsptr[0]) >> (2 + 3))) & (255 * 4 + 3)]))));
__boundcheck_metadata_store((void *)(&dcval),(void *)((size_t)(&dcval)+sizeof(dcval)*8-1));

      
      (*(JSAMPLE *)(__boundcheck_ptr_reference(298,7,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[0])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(299,7,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[1])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(300,7,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[2])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(301,7,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[3])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(302,7,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[4])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(303,7,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[5])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(304,7,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[6])))) = dcval;
      (*(JSAMPLE *)(__boundcheck_ptr_reference(305,7,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[7])))) = dcval;

      wsptr += DCTSIZE;		/* advance pointer to next row */
      continue;
    }
#endif
    
    /* Even part */

    tmp10 = ((DCTELEM) (*(int *)(__boundcheck_ptr_reference(314,24,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[0])))) + (DCTELEM) (*(int *)(__boundcheck_ptr_reference(314,45,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[4])))));
    tmp11 = ((DCTELEM) (*(int *)(__boundcheck_ptr_reference(315,24,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[0])))) - (DCTELEM) (*(int *)(__boundcheck_ptr_reference(315,45,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[4])))));

    tmp13 = ((DCTELEM) (*(int *)(__boundcheck_ptr_reference(317,24,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[2])))) + (DCTELEM) (*(int *)(__boundcheck_ptr_reference(317,45,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[6])))));
    tmp12 = MULTIPLY((DCTELEM) wsptr[2] - (DCTELEM) wsptr[6], FIX_1_414213562)
	    - tmp13;

    tmp0 = tmp10 + tmp13;
    tmp3 = tmp10 - tmp13;
    tmp1 = tmp11 + tmp12;
    tmp2 = tmp11 - tmp12;

    /* Odd part */

    z13 = (DCTELEM) (*(int *)(__boundcheck_ptr_reference(328,21,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[5])))) + (DCTELEM) (*(int *)(__boundcheck_ptr_reference(328,42,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[3]))));
    z10 = (DCTELEM) (*(int *)(__boundcheck_ptr_reference(329,21,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[5])))) - (DCTELEM) (*(int *)(__boundcheck_ptr_reference(329,42,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[3]))));
    z11 = (DCTELEM) (*(int *)(__boundcheck_ptr_reference(330,21,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[1])))) + (DCTELEM) (*(int *)(__boundcheck_ptr_reference(330,42,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[7]))));
    z12 = (DCTELEM) (*(int *)(__boundcheck_ptr_reference(331,21,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[1])))) - (DCTELEM) (*(int *)(__boundcheck_ptr_reference(331,42,"jpeg_idct_ifast",(void *)(&wsptr[0]),(void *)(&wsptr[7]))));

    tmp7 = z11 + z13;		/* phase 5 */
    tmp11 = MULTIPLY(z11 - z13, FIX_1_414213562); /* 2*c4 */

    z5 = MULTIPLY(z10 + z12, FIX_1_847759065); /* 2*c2 */
    tmp10 = MULTIPLY(z12, FIX_1_082392200) - z5; /* 2*(c2-c6) */
    tmp12 = MULTIPLY(z10, - FIX_2_613125930) + z5; /* -2*(c2+c6) */

    tmp6 = tmp12 - tmp7;	/* phase 2 */
    tmp5 = tmp11 - tmp6;
    tmp4 = tmp10 + tmp5;

    /* Final output stage: scale down by a factor of 8 and range-limit */

    (*(JSAMPLE *)(__boundcheck_ptr_reference(346,5,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[0])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(346,17,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((tmp0 + tmp7) >> (2 + 3))) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(348,5,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[7])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(348,17,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((tmp0 - tmp7) >> (2 + 3))) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(350,5,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(350,17,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((tmp1 + tmp6) >> (2 + 3))) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(352,5,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[6])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(352,17,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((tmp1 - tmp6) >> (2 + 3))) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(354,5,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[2])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(354,17,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((tmp2 + tmp5) >> (2 + 3))) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(356,5,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[5])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(356,17,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((tmp2 - tmp5) >> (2 + 3))) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(358,5,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[4])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(358,17,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((tmp3 + tmp4) >> (2 + 3))) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(360,5,"jpeg_idct_ifast",(void *)(&outptr[0]),(void *)(&outptr[3])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(360,17,"jpeg_idct_ifast",(void *)(&range_limit[0]),(void *)(&range_limit[((int)((tmp3 - tmp4) >> (2 + 3))) & (255 * 4 + 3)]))));

    wsptr += DCTSIZE;		/* advance pointer to next row */
  }
}

#endif /* DCT_IFAST_SUPPORTED */
