/*
 * jfdctflt.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a floating-point implementation of the
 * forward DCT (Discrete Cosine Transform).
 *
 * This implementation should be more accurate than either of the integer
 * DCT implementations.  However, it may not give the same results on all
 * machines because of differences in roundoff behavior.  Speed will depend
 * on the hardware's floating point capacity.
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
 * The primary disadvantage of this method is that with a fixed-point
 * implementation, accuracy is lost due to imprecise representation of the
 * scaled quantization values.  However, that problem does not arise if
 * we use floating point arithmetic.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jdct.h"		/* Private declarations for DCT subsystem */

#ifdef DCT_FLOAT_SUPPORTED


/*
 * This module is specialized to the case DCTSIZE = 8.
 */

#if DCTSIZE != 8
  Sorry, this code only copes with 8x8 DCTs. /* deliberate syntax err */
#endif


/*
 * Perform the forward DCT on one block of samples.
 */

GLOBAL(void)
jpeg_fdct_float (FAST_FLOAT * data)
{
  FAST_FLOAT tmp0;
__boundcheck_metadata_store((void *)(&tmp0),(void *)((size_t)(&tmp0)+sizeof(tmp0)*8-1));
float  tmp1;
__boundcheck_metadata_store((void *)(&tmp1),(void *)((size_t)(&tmp1)+sizeof(tmp1)*8-1));
float  tmp2;
__boundcheck_metadata_store((void *)(&tmp2),(void *)((size_t)(&tmp2)+sizeof(tmp2)*8-1));
float  tmp3;
__boundcheck_metadata_store((void *)(&tmp3),(void *)((size_t)(&tmp3)+sizeof(tmp3)*8-1));
float  tmp4;
__boundcheck_metadata_store((void *)(&tmp4),(void *)((size_t)(&tmp4)+sizeof(tmp4)*8-1));
float  tmp5;
__boundcheck_metadata_store((void *)(&tmp5),(void *)((size_t)(&tmp5)+sizeof(tmp5)*8-1));
float  tmp6;
__boundcheck_metadata_store((void *)(&tmp6),(void *)((size_t)(&tmp6)+sizeof(tmp6)*8-1));
float  tmp7;
__boundcheck_metadata_store((void *)(&tmp7),(void *)((size_t)(&tmp7)+sizeof(tmp7)*8-1)); 
  FAST_FLOAT tmp10;
__boundcheck_metadata_store((void *)(&tmp10),(void *)((size_t)(&tmp10)+sizeof(tmp10)*8-1));
float  tmp11;
__boundcheck_metadata_store((void *)(&tmp11),(void *)((size_t)(&tmp11)+sizeof(tmp11)*8-1));
float  tmp12;
__boundcheck_metadata_store((void *)(&tmp12),(void *)((size_t)(&tmp12)+sizeof(tmp12)*8-1));
float  tmp13;
__boundcheck_metadata_store((void *)(&tmp13),(void *)((size_t)(&tmp13)+sizeof(tmp13)*8-1)); 
  FAST_FLOAT z1;
__boundcheck_metadata_store((void *)(&z1),(void *)((size_t)(&z1)+sizeof(z1)*8-1));
float  z2;
__boundcheck_metadata_store((void *)(&z2),(void *)((size_t)(&z2)+sizeof(z2)*8-1));
float  z3;
__boundcheck_metadata_store((void *)(&z3),(void *)((size_t)(&z3)+sizeof(z3)*8-1));
float  z4;
__boundcheck_metadata_store((void *)(&z4),(void *)((size_t)(&z4)+sizeof(z4)*8-1));
float  z5;
__boundcheck_metadata_store((void *)(&z5),(void *)((size_t)(&z5)+sizeof(z5)*8-1));
float  z11;
__boundcheck_metadata_store((void *)(&z11),(void *)((size_t)(&z11)+sizeof(z11)*8-1));
float  z13;
__boundcheck_metadata_store((void *)(&z13),(void *)((size_t)(&z13)+sizeof(z13)*8-1));  
  FAST_FLOAT *dataptr;
__boundcheck_metadata_store((void *)(&dataptr),(void *)((size_t)(&dataptr)+sizeof(dataptr)*8-1));

  int ctr;
__boundcheck_metadata_store((void *)(&ctr),(void *)((size_t)(&ctr)+sizeof(ctr)*8-1));


  /* Pass 1: process rows. */

  dataptr = data;
__boundcheck_metadata_trans_check((void *)(dataptr),(void *)(data),(void *)(data));

  for (ctr = DCTSIZE-1; ctr >= 0; ctr--) {
    tmp0 = (*(float *)(__boundcheck_ptr_reference(71,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[0])))) + (*(float *)(__boundcheck_ptr_reference(71,25,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[7]))));
    tmp7 = (*(float *)(__boundcheck_ptr_reference(72,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[0])))) - (*(float *)(__boundcheck_ptr_reference(72,25,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[7]))));
    tmp1 = (*(float *)(__boundcheck_ptr_reference(73,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[1])))) + (*(float *)(__boundcheck_ptr_reference(73,25,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[6]))));
    tmp6 = (*(float *)(__boundcheck_ptr_reference(74,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[1])))) - (*(float *)(__boundcheck_ptr_reference(74,25,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[6]))));
    tmp2 = (*(float *)(__boundcheck_ptr_reference(75,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[2])))) + (*(float *)(__boundcheck_ptr_reference(75,25,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[5]))));
    tmp5 = (*(float *)(__boundcheck_ptr_reference(76,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[2])))) - (*(float *)(__boundcheck_ptr_reference(76,25,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[5]))));
    tmp3 = (*(float *)(__boundcheck_ptr_reference(77,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[3])))) + (*(float *)(__boundcheck_ptr_reference(77,25,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[4]))));
    tmp4 = (*(float *)(__boundcheck_ptr_reference(78,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[3])))) - (*(float *)(__boundcheck_ptr_reference(78,25,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[4]))));
    
    /* Even part */
    
    tmp10 = tmp0 + tmp3;	/* phase 2 */
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;
    
    (*(float *)(__boundcheck_ptr_reference(87,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[0])))) = tmp10 + tmp11; /* phase 3 */
    (*(float *)(__boundcheck_ptr_reference(88,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[4])))) = tmp10 - tmp11;
    
    z1 = (tmp12 + tmp13) * ((FAST_FLOAT) 0.707106781); /* c4 */
    (*(float *)(__boundcheck_ptr_reference(91,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[2])))) = tmp13 + z1;	/* phase 5 */
    (*(float *)(__boundcheck_ptr_reference(92,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[6])))) = tmp13 - z1;
    
    /* Odd part */

    tmp10 = tmp4 + tmp5;	/* phase 2 */
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    /* The rotator is modified from fig 4-8 to avoid extra negations. */
    z5 = (tmp10 - tmp12) * ((FAST_FLOAT) 0.382683433); /* c6 */
    z2 = ((FAST_FLOAT) 0.541196100) * tmp10 + z5; /* c2-c6 */
    z4 = ((FAST_FLOAT) 1.306562965) * tmp12 + z5; /* c2+c6 */
    z3 = tmp11 * ((FAST_FLOAT) 0.707106781); /* c4 */

    z11 = tmp7 + z3;		/* phase 5 */
    z13 = tmp7 - z3;

    (*(float *)(__boundcheck_ptr_reference(109,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[5])))) = z13 + z2;	/* phase 6 */
    (*(float *)(__boundcheck_ptr_reference(110,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[3])))) = z13 - z2;
    (*(float *)(__boundcheck_ptr_reference(111,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[1])))) = z11 + z4;
    (*(float *)(__boundcheck_ptr_reference(112,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[7])))) = z11 - z4;

    dataptr += DCTSIZE;		/* advance pointer to next row */
  }

  /* Pass 2: process columns. */

  dataptr = data;
__boundcheck_metadata_trans_check((void *)(dataptr),(void *)(data),(void *)(data));

  for (ctr = DCTSIZE-1; ctr >= 0; ctr--) {
    tmp0 = (*(float *)(__boundcheck_ptr_reference(121,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 0])))) + (*(float *)(__boundcheck_ptr_reference(121,33,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 7]))));
    tmp7 = (*(float *)(__boundcheck_ptr_reference(122,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 0])))) - (*(float *)(__boundcheck_ptr_reference(122,33,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 7]))));
    tmp1 = (*(float *)(__boundcheck_ptr_reference(123,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 1])))) + (*(float *)(__boundcheck_ptr_reference(123,33,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 6]))));
    tmp6 = (*(float *)(__boundcheck_ptr_reference(124,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 1])))) - (*(float *)(__boundcheck_ptr_reference(124,33,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 6]))));
    tmp2 = (*(float *)(__boundcheck_ptr_reference(125,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 2])))) + (*(float *)(__boundcheck_ptr_reference(125,33,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 5]))));
    tmp5 = (*(float *)(__boundcheck_ptr_reference(126,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 2])))) - (*(float *)(__boundcheck_ptr_reference(126,33,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 5]))));
    tmp3 = (*(float *)(__boundcheck_ptr_reference(127,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 3])))) + (*(float *)(__boundcheck_ptr_reference(127,33,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 4]))));
    tmp4 = (*(float *)(__boundcheck_ptr_reference(128,12,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 3])))) - (*(float *)(__boundcheck_ptr_reference(128,33,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 4]))));
    
    /* Even part */
    
    tmp10 = tmp0 + tmp3;	/* phase 2 */
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;
    
    (*(float *)(__boundcheck_ptr_reference(137,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 0])))) = tmp10 + tmp11; /* phase 3 */
    (*(float *)(__boundcheck_ptr_reference(138,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 4])))) = tmp10 - tmp11;
    
    z1 = (tmp12 + tmp13) * ((FAST_FLOAT) 0.707106781); /* c4 */
    (*(float *)(__boundcheck_ptr_reference(141,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 2])))) = tmp13 + z1; /* phase 5 */
    (*(float *)(__boundcheck_ptr_reference(142,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 6])))) = tmp13 - z1;
    
    /* Odd part */

    tmp10 = tmp4 + tmp5;	/* phase 2 */
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    /* The rotator is modified from fig 4-8 to avoid extra negations. */
    z5 = (tmp10 - tmp12) * ((FAST_FLOAT) 0.382683433); /* c6 */
    z2 = ((FAST_FLOAT) 0.541196100) * tmp10 + z5; /* c2-c6 */
    z4 = ((FAST_FLOAT) 1.306562965) * tmp12 + z5; /* c2+c6 */
    z3 = tmp11 * ((FAST_FLOAT) 0.707106781); /* c4 */

    z11 = tmp7 + z3;		/* phase 5 */
    z13 = tmp7 - z3;

    (*(float *)(__boundcheck_ptr_reference(159,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 5])))) = z13 + z2; /* phase 6 */
    (*(float *)(__boundcheck_ptr_reference(160,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 3])))) = z13 - z2;
    (*(float *)(__boundcheck_ptr_reference(161,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 1])))) = z11 + z4;
    (*(float *)(__boundcheck_ptr_reference(162,5,"jpeg_fdct_float",(void *)(&dataptr[0]),(void *)(&dataptr[8 * 7])))) = z11 - z4;

    dataptr++;			/* advance pointer to next column */
  }
}

#endif /* DCT_FLOAT_SUPPORTED */
