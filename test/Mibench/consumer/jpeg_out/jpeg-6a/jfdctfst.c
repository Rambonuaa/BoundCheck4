/*
 * jfdctfst.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a fast, not so accurate integer implementation of the
 * forward DCT (Discrete Cosine Transform).
 *
 * A 2-D DCT can be done by 1-D DCT on each row followed by 1-D DCT
 * on each column.  Direct algorithms are also available, but they are
 * much more complex and seem not to be any faster when reduced to code.
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
 * see jfdctint.c for more details.  However, we choose to descale
 * (right shift) multiplication products as soon as they are formed,
 * rather than carrying additional fractional bits into subsequent additions.
 * This compromises accuracy slightly, but it lets us save a few shifts.
 * More importantly, 16-bit arithmetic is then adequate (for 8-bit samples)
 * everywhere except in the multiplications proper; this saves a good deal
 * of work on 16-bit-int machines.
 *
 * Again to save a few shifts, the intermediate results between pass 1 and
 * pass 2 are not upscaled, but are represented only to integral precision.
 *
 * A final compromise is to represent the multiplicative constants to only
 * 8 fractional bits, rather than 13.  This saves some shifting work on some
 * machines, and may also reduce the cost of multiplication (since there
 * are fewer one-bits in the constants).
 */

#define CONST_BITS  8


/* Some C compilers fail to reduce "FIX(constant)" at compile time, thus
 * causing a lot of useless floating-point operations at run time.
 * To get around this we use the following pre-calculated constants.
 * If you change CONST_BITS you may want to add appropriate values.
 * (With a reasonable C compiler, you can just rely on the FIX() macro...)
 */

#if CONST_BITS == 8
#define FIX_0_382683433  ((INT32)   98)		/* FIX(0.382683433) */
#define FIX_0_541196100  ((INT32)  139)		/* FIX(0.541196100) */
#define FIX_0_707106781  ((INT32)  181)		/* FIX(0.707106781) */
#define FIX_1_306562965  ((INT32)  334)		/* FIX(1.306562965) */
#else
#define FIX_0_382683433  FIX(0.382683433)
#define FIX_0_541196100  FIX(0.541196100)
#define FIX_0_707106781  FIX(0.707106781)
#define FIX_1_306562965  FIX(1.306562965)
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


/*
 * Perform the forward DCT on one block of samples.
 */

GLOBAL(void)
jpeg_fdct_ifast (DCTELEM * data)
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

  DCTELEM z1;
__boundcheck_metadata_store((void *)(&z1),(void *)((size_t)(&z1)+sizeof(z1)*8-1));
DCTELEM  z2;
__boundcheck_metadata_store((void *)(&z2),(void *)((size_t)(&z2)+sizeof(z2)*8-1));
DCTELEM  z3;
__boundcheck_metadata_store((void *)(&z3),(void *)((size_t)(&z3)+sizeof(z3)*8-1));
DCTELEM  z4;
__boundcheck_metadata_store((void *)(&z4),(void *)((size_t)(&z4)+sizeof(z4)*8-1));
DCTELEM  z5;
__boundcheck_metadata_store((void *)(&z5),(void *)((size_t)(&z5)+sizeof(z5)*8-1));
DCTELEM  z11;
__boundcheck_metadata_store((void *)(&z11),(void *)((size_t)(&z11)+sizeof(z11)*8-1));
DCTELEM  z13;
__boundcheck_metadata_store((void *)(&z13),(void *)((size_t)(&z13)+sizeof(z13)*8-1));

  DCTELEM *dataptr;
__boundcheck_metadata_store((void *)(&dataptr),(void *)((size_t)(&dataptr)+sizeof(dataptr)*8-1));

  int ctr;
__boundcheck_metadata_store((void *)(&ctr),(void *)((size_t)(&ctr)+sizeof(ctr)*8-1));

  SHIFT_TEMPS

  /* Pass 1: process rows. */

  dataptr = data;
__boundcheck_metadata_trans_check((void *)(dataptr),(void *)(data),(void *)(data));

  for (ctr = DCTSIZE-1; ctr >= 0; ctr--) {
    tmp0 = (*(DCTELEM *)(__boundcheck_ptr_reference(127,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[0])))) + (*(DCTELEM *)(__boundcheck_ptr_reference(127,25,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[7]))));
    tmp7 = (*(DCTELEM *)(__boundcheck_ptr_reference(128,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[0])))) - (*(DCTELEM *)(__boundcheck_ptr_reference(128,25,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[7]))));
    tmp1 = (*(DCTELEM *)(__boundcheck_ptr_reference(129,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[1])))) + (*(DCTELEM *)(__boundcheck_ptr_reference(129,25,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[6]))));
    tmp6 = (*(DCTELEM *)(__boundcheck_ptr_reference(130,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[1])))) - (*(DCTELEM *)(__boundcheck_ptr_reference(130,25,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[6]))));
    tmp2 = (*(DCTELEM *)(__boundcheck_ptr_reference(131,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[2])))) + (*(DCTELEM *)(__boundcheck_ptr_reference(131,25,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[5]))));
    tmp5 = (*(DCTELEM *)(__boundcheck_ptr_reference(132,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[2])))) - (*(DCTELEM *)(__boundcheck_ptr_reference(132,25,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[5]))));
    tmp3 = (*(DCTELEM *)(__boundcheck_ptr_reference(133,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[3])))) + (*(DCTELEM *)(__boundcheck_ptr_reference(133,25,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[4]))));
    tmp4 = (*(DCTELEM *)(__boundcheck_ptr_reference(134,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[3])))) - (*(DCTELEM *)(__boundcheck_ptr_reference(134,25,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[4]))));
    
    /* Even part */
    
    tmp10 = tmp0 + tmp3;	/* phase 2 */
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;
    
    (*(DCTELEM *)(__boundcheck_ptr_reference(143,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[0])))) = tmp10 + tmp11; /* phase 3 */
    (*(DCTELEM *)(__boundcheck_ptr_reference(144,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[4])))) = tmp10 - tmp11;
    
    z1 = MULTIPLY(tmp12 + tmp13, FIX_0_707106781); /* c4 */
    (*(DCTELEM *)(__boundcheck_ptr_reference(147,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[2])))) = tmp13 + z1;	/* phase 5 */
    (*(DCTELEM *)(__boundcheck_ptr_reference(148,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[6])))) = tmp13 - z1;
    
    /* Odd part */

    tmp10 = tmp4 + tmp5;	/* phase 2 */
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    /* The rotator is modified from fig 4-8 to avoid extra negations. */
    z5 = MULTIPLY(tmp10 - tmp12, FIX_0_382683433); /* c6 */
    z2 = MULTIPLY(tmp10, FIX_0_541196100) + z5; /* c2-c6 */
    z4 = MULTIPLY(tmp12, FIX_1_306562965) + z5; /* c2+c6 */
    z3 = MULTIPLY(tmp11, FIX_0_707106781); /* c4 */

    z11 = tmp7 + z3;		/* phase 5 */
    z13 = tmp7 - z3;

    (*(DCTELEM *)(__boundcheck_ptr_reference(165,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[5])))) = z13 + z2;	/* phase 6 */
    (*(DCTELEM *)(__boundcheck_ptr_reference(166,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[3])))) = z13 - z2;
    (*(DCTELEM *)(__boundcheck_ptr_reference(167,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[1])))) = z11 + z4;
    (*(DCTELEM *)(__boundcheck_ptr_reference(168,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[7])))) = z11 - z4;

    dataptr += DCTSIZE;		/* advance pointer to next row */
  }

  /* Pass 2: process columns. */

  dataptr = data;
__boundcheck_metadata_trans_check((void *)(dataptr),(void *)(data),(void *)(data));

  for (ctr = DCTSIZE-1; ctr >= 0; ctr--) {
    tmp0 = (*(DCTELEM *)(__boundcheck_ptr_reference(177,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 0])))) + (*(DCTELEM *)(__boundcheck_ptr_reference(177,33,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 7]))));
    tmp7 = (*(DCTELEM *)(__boundcheck_ptr_reference(178,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 0])))) - (*(DCTELEM *)(__boundcheck_ptr_reference(178,33,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 7]))));
    tmp1 = (*(DCTELEM *)(__boundcheck_ptr_reference(179,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 1])))) + (*(DCTELEM *)(__boundcheck_ptr_reference(179,33,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 6]))));
    tmp6 = (*(DCTELEM *)(__boundcheck_ptr_reference(180,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 1])))) - (*(DCTELEM *)(__boundcheck_ptr_reference(180,33,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 6]))));
    tmp2 = (*(DCTELEM *)(__boundcheck_ptr_reference(181,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 2])))) + (*(DCTELEM *)(__boundcheck_ptr_reference(181,33,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 5]))));
    tmp5 = (*(DCTELEM *)(__boundcheck_ptr_reference(182,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 2])))) - (*(DCTELEM *)(__boundcheck_ptr_reference(182,33,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 5]))));
    tmp3 = (*(DCTELEM *)(__boundcheck_ptr_reference(183,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 3])))) + (*(DCTELEM *)(__boundcheck_ptr_reference(183,33,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 4]))));
    tmp4 = (*(DCTELEM *)(__boundcheck_ptr_reference(184,12,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 3])))) - (*(DCTELEM *)(__boundcheck_ptr_reference(184,33,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 4]))));
    
    /* Even part */
    
    tmp10 = tmp0 + tmp3;	/* phase 2 */
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;
    
    (*(DCTELEM *)(__boundcheck_ptr_reference(193,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 0])))) = tmp10 + tmp11; /* phase 3 */
    (*(DCTELEM *)(__boundcheck_ptr_reference(194,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 4])))) = tmp10 - tmp11;
    
    z1 = MULTIPLY(tmp12 + tmp13, FIX_0_707106781); /* c4 */
    (*(DCTELEM *)(__boundcheck_ptr_reference(197,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 2])))) = tmp13 + z1; /* phase 5 */
    (*(DCTELEM *)(__boundcheck_ptr_reference(198,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 6])))) = tmp13 - z1;
    
    /* Odd part */

    tmp10 = tmp4 + tmp5;	/* phase 2 */
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    /* The rotator is modified from fig 4-8 to avoid extra negations. */
    z5 = MULTIPLY(tmp10 - tmp12, FIX_0_382683433); /* c6 */
    z2 = MULTIPLY(tmp10, FIX_0_541196100) + z5; /* c2-c6 */
    z4 = MULTIPLY(tmp12, FIX_1_306562965) + z5; /* c2+c6 */
    z3 = MULTIPLY(tmp11, FIX_0_707106781); /* c4 */

    z11 = tmp7 + z3;		/* phase 5 */
    z13 = tmp7 - z3;

    (*(DCTELEM *)(__boundcheck_ptr_reference(215,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 5])))) = z13 + z2; /* phase 6 */
    (*(DCTELEM *)(__boundcheck_ptr_reference(216,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 3])))) = z13 - z2;
    (*(DCTELEM *)(__boundcheck_ptr_reference(217,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 1])))) = z11 + z4;
    (*(DCTELEM *)(__boundcheck_ptr_reference(218,5,"jpeg_fdct_ifast",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 7])))) = z11 - z4;

    dataptr++;			/* advance pointer to next column */
  }
}

#endif /* DCT_IFAST_SUPPORTED */
