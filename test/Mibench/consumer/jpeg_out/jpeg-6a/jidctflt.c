/*
 * jidctflt.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a floating-point implementation of the
 * inverse DCT (Discrete Cosine Transform).  In the IJG code, this routine
 * must also perform dequantization of the input coefficients.
 *
 * This implementation should be more accurate than either of the integer
 * IDCT implementations.  However, it may not give the same results on all
 * machines because of differences in roundoff behavior.  Speed will depend
 * on the hardware's floating point capacity.
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


/* Dequantize a coefficient by multiplying it by the multiplier-table
 * entry; produce a float result.
 */

#define DEQUANTIZE(coef,quantval)  (((FAST_FLOAT) (coef)) * (quantval))


/*
 * Perform dequantization and inverse DCT on one block of coefficients.
 */

GLOBAL(void)
jpeg_idct_float (j_decompress_ptr cinfo, jpeg_component_info * compptr,
		 JCOEFPTR coef_block,
		 JSAMPARRAY output_buf, JDIMENSION output_col)
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
float 
  FAST_FLOAT tmp10;
__boundcheck_metadata_store((void *)(&tmp10),(void *)((size_t)(&tmp10)+sizeof(tmp10)*8-1));
float  tmp11;
__boundcheck_metadata_store((void *)(&tmp11),(void *)((size_t)(&tmp11)+sizeof(tmp11)*8-1));
float  tmp12;
__boundcheck_metadata_store((void *)(&tmp12),(void *)((size_t)(&tmp12)+sizeof(tmp12)*8-1));
float  tmp13;
__boundcheck_metadata_store((void *)(&tmp13),(void *)((size_t)(&tmp13)+sizeof(tmp13)*8-1));
float 
  FAST_FLOAT z5;
__boundcheck_metadata_store((void *)(&z5),(void *)((size_t)(&z5)+sizeof(z5)*8-1));
float  z10;
__boundcheck_metadata_store((void *)(&z10),(void *)((size_t)(&z10)+sizeof(z10)*8-1));
float  z11;
__boundcheck_metadata_store((void *)(&z11),(void *)((size_t)(&z11)+sizeof(z11)*8-1));
float  z12;
__boundcheck_metadata_store((void *)(&z12),(void *)((size_t)(&z12)+sizeof(z12)*8-1));
float  z13;
__boundcheck_metadata_store((void *)(&z13),(void *)((size_t)(&z13)+sizeof(z13)*8-1));

  JCOEFPTR inptr;
__boundcheck_metadata_store((void *)(&inptr),(void *)((size_t)(&inptr)+sizeof(inptr)*8-1));

  FLOAT_MULT_TYPE * quantptr;
__boundcheck_metadata_store((void *)(&quantptr),(void *)((size_t)(&quantptr)+sizeof(quantptr)*8-1));

  FAST_FLOAT * wsptr;
__boundcheck_metadata_store((void *)(&wsptr),(void *)((size_t)(&wsptr)+sizeof(wsptr)*8-1));

  JSAMPROW outptr;
__boundcheck_metadata_store((void *)(&outptr),(void *)((size_t)(&outptr)+sizeof(outptr)*8-1));

  JSAMPLE *range_limit = IDCT_range_limit(
__boundcheck_metadata_store((void *)(&range_limit),(void *)((size_t)(&range_limit)+sizeof(range_limit)*8-1));
cinfo);
  int ctr;
__boundcheck_metadata_store((void *)(&ctr),(void *)((size_t)(&ctr)+sizeof(ctr)*8-1));

  FAST_FLOAT workspace[DCTSIZE2];__boundcheck_metadata_store(&workspace[0],&workspace[64-1]);
float  /* buffers data between passes */
  SHIFT_TEMPS

  /* Pass 1: process columns from input, store into work array. */

  inptr = coef_block;
__boundcheck_metadata_trans_check((void *)(inptr),(void *)(coef_block),(void *)(coef_block));

  quantptr = (FLOAT_MULT_TYPE *) compptr->dct_table;
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
    
    if (((*(JCOEF *)(__boundcheck_ptr_reference(99,10,"jpeg_idct_float",(void *)(&inptr[0]),(void *)(&inptr[8 * 1])))) | (*(JCOEF *)(__boundcheck_ptr_reference(99,29,"jpeg_idct_float",(void *)(&inptr[0]),(void *)(&inptr[8 * 2])))) | (*(JCOEF *)(__boundcheck_ptr_reference(99,48,"jpeg_idct_float",(void *)(&inptr[0]),(void *)(&inptr[8 * 3])))) |
	 (*(JCOEF *)(__boundcheck_ptr_reference(100,3,"jpeg_idct_float",(void *)(&inptr[0]),(void *)(&inptr[8 * 4])))) | (*(JCOEF *)(__boundcheck_ptr_reference(100,22,"jpeg_idct_float",(void *)(&inptr[0]),(void *)(&inptr[8 * 5])))) | (*(JCOEF *)(__boundcheck_ptr_reference(100,41,"jpeg_idct_float",(void *)(&inptr[0]),(void *)(&inptr[8 * 6])))) |
	 (*(JCOEF *)(__boundcheck_ptr_reference(101,3,"jpeg_idct_float",(void *)(&inptr[0]),(void *)(&inptr[8 * 7]))))) == 0) {
      /* AC terms all zero */
      FAST_FLOAT dcval = DEQUANTIZE(
__boundcheck_metadata_store((void *)(&dcval),(void *)((size_t)(&dcval)+sizeof(dcval)*8-1));
inptr[DCTSIZE*0], quantptr[DCTSIZE*0]);
      
      (*(float *)(__boundcheck_ptr_reference(105,7,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 0])))) = dcval;
      (*(float *)(__boundcheck_ptr_reference(106,7,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 1])))) = dcval;
      (*(float *)(__boundcheck_ptr_reference(107,7,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 2])))) = dcval;
      (*(float *)(__boundcheck_ptr_reference(108,7,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 3])))) = dcval;
      (*(float *)(__boundcheck_ptr_reference(109,7,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 4])))) = dcval;
      (*(float *)(__boundcheck_ptr_reference(110,7,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 5])))) = dcval;
      (*(float *)(__boundcheck_ptr_reference(111,7,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 6])))) = dcval;
      (*(float *)(__boundcheck_ptr_reference(112,7,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 7])))) = dcval;
      
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
    tmp12 = (tmp1 - tmp3) * ((FAST_FLOAT) 1.414213562) - tmp13; /* 2*c4 */

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
    tmp11 = (z11 - z13) * ((FAST_FLOAT) 1.414213562); /* 2*c4 */

    z5 = (z10 + z12) * ((FAST_FLOAT) 1.847759065); /* 2*c2 */
    tmp10 = ((FAST_FLOAT) 1.082392200) * z12 - z5; /* 2*(c2-c6) */
    tmp12 = ((FAST_FLOAT) -2.613125930) * z10 + z5; /* -2*(c2+c6) */

    tmp6 = tmp12 - tmp7;	/* phase 2 */
    tmp5 = tmp11 - tmp6;
    tmp4 = tmp10 + tmp5;

    (*(float *)(__boundcheck_ptr_reference(161,5,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 0])))) = tmp0 + tmp7;
    (*(float *)(__boundcheck_ptr_reference(162,5,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 7])))) = tmp0 - tmp7;
    (*(float *)(__boundcheck_ptr_reference(163,5,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 1])))) = tmp1 + tmp6;
    (*(float *)(__boundcheck_ptr_reference(164,5,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 6])))) = tmp1 - tmp6;
    (*(float *)(__boundcheck_ptr_reference(165,5,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 2])))) = tmp2 + tmp5;
    (*(float *)(__boundcheck_ptr_reference(166,5,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 5])))) = tmp2 - tmp5;
    (*(float *)(__boundcheck_ptr_reference(167,5,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 4])))) = tmp3 + tmp4;
    (*(float *)(__boundcheck_ptr_reference(168,5,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[8 * 3])))) = tmp3 - tmp4;

    inptr++;			/* advance pointers to next column */
    quantptr++;
    wsptr++;
  }
  
  /* Pass 2: process rows from work array, store into output array. */
  /* Note that we must descale the results by a factor of 8 == 2**3. */

  wsptr = workspace;
__boundcheck_metadata_trans_check((void *)(wsptr),(void *)(workspace),(void *)(workspace));

  for (ctr = 0; ctr < DCTSIZE; ctr++) {
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(180,14,"jpeg_idct_float",(void *)(&output_buf[0]),(void *)(&output_buf[ctr])))) + output_col;
    /* Rows of zeroes can be exploited in the same way as we did with columns.
     * However, the column calculation has created many nonzero AC terms, so
     * the simplification applies less often (typically 5% to 10% of the time).
     * And testing floats for zero is relatively expensive, so we don't bother.
     */
    
    /* Even part */

    tmp10 = (*(float *)(__boundcheck_ptr_reference(189,13,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[0])))) + (*(float *)(__boundcheck_ptr_reference(189,24,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[4]))));
    tmp11 = (*(float *)(__boundcheck_ptr_reference(190,13,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[0])))) - (*(float *)(__boundcheck_ptr_reference(190,24,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[4]))));

    tmp13 = (*(float *)(__boundcheck_ptr_reference(192,13,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[2])))) + (*(float *)(__boundcheck_ptr_reference(192,24,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[6]))));
    tmp12 = ((*(float *)(__boundcheck_ptr_reference(193,14,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[2])))) - (*(float *)(__boundcheck_ptr_reference(193,25,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[6]))))) * ((FAST_FLOAT) 1.414213562) - tmp13;

    tmp0 = tmp10 + tmp13;
    tmp3 = tmp10 - tmp13;
    tmp1 = tmp11 + tmp12;
    tmp2 = tmp11 - tmp12;

    /* Odd part */

    z13 = (*(float *)(__boundcheck_ptr_reference(202,11,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[5])))) + (*(float *)(__boundcheck_ptr_reference(202,22,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[3]))));
    z10 = (*(float *)(__boundcheck_ptr_reference(203,11,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[5])))) - (*(float *)(__boundcheck_ptr_reference(203,22,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[3]))));
    z11 = (*(float *)(__boundcheck_ptr_reference(204,11,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[1])))) + (*(float *)(__boundcheck_ptr_reference(204,22,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[7]))));
    z12 = (*(float *)(__boundcheck_ptr_reference(205,11,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[1])))) - (*(float *)(__boundcheck_ptr_reference(205,22,"jpeg_idct_float",(void *)(&wsptr[0]),(void *)(&wsptr[7]))));

    tmp7 = z11 + z13;
    tmp11 = (z11 - z13) * ((FAST_FLOAT) 1.414213562);

    z5 = (z10 + z12) * ((FAST_FLOAT) 1.847759065); /* 2*c2 */
    tmp10 = ((FAST_FLOAT) 1.082392200) * z12 - z5; /* 2*(c2-c6) */
    tmp12 = ((FAST_FLOAT) -2.613125930) * z10 + z5; /* -2*(c2+c6) */

    tmp6 = tmp12 - tmp7;
    tmp5 = tmp11 - tmp6;
    tmp4 = tmp10 + tmp5;

    /* Final output stage: scale down by a factor of 8 and range-limit */

    (*(JSAMPLE *)(__boundcheck_ptr_reference(220,5,"jpeg_idct_float",(void *)(&outptr[0]),(void *)(&outptr[0])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(220,17,"jpeg_idct_float",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)(tmp0 + tmp7)) + (((INT32)1) << ((3) - 1))) >> (3)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(222,5,"jpeg_idct_float",(void *)(&outptr[0]),(void *)(&outptr[7])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(222,17,"jpeg_idct_float",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)(tmp0 - tmp7)) + (((INT32)1) << ((3) - 1))) >> (3)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(224,5,"jpeg_idct_float",(void *)(&outptr[0]),(void *)(&outptr[1])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(224,17,"jpeg_idct_float",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)(tmp1 + tmp6)) + (((INT32)1) << ((3) - 1))) >> (3)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(226,5,"jpeg_idct_float",(void *)(&outptr[0]),(void *)(&outptr[6])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(226,17,"jpeg_idct_float",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)(tmp1 - tmp6)) + (((INT32)1) << ((3) - 1))) >> (3)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(228,5,"jpeg_idct_float",(void *)(&outptr[0]),(void *)(&outptr[2])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(228,17,"jpeg_idct_float",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)(tmp2 + tmp5)) + (((INT32)1) << ((3) - 1))) >> (3)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(230,5,"jpeg_idct_float",(void *)(&outptr[0]),(void *)(&outptr[5])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(230,17,"jpeg_idct_float",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)(tmp2 - tmp5)) + (((INT32)1) << ((3) - 1))) >> (3)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(232,5,"jpeg_idct_float",(void *)(&outptr[0]),(void *)(&outptr[4])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(232,17,"jpeg_idct_float",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)(tmp3 + tmp4)) + (((INT32)1) << ((3) - 1))) >> (3)) & (255 * 4 + 3)]))));
    (*(JSAMPLE *)(__boundcheck_ptr_reference(234,5,"jpeg_idct_float",(void *)(&outptr[0]),(void *)(&outptr[3])))) = (*(JSAMPLE *)(__boundcheck_ptr_reference(234,17,"jpeg_idct_float",(void *)(&range_limit[0]),(void *)(&range_limit[(int)((((INT32)(tmp3 - tmp4)) + (((INT32)1) << ((3) - 1))) >> (3)) & (255 * 4 + 3)]))));
    
    wsptr += DCTSIZE;		/* advance pointer to next row */
  }
}

#endif /* DCT_FLOAT_SUPPORTED */
