/*
 * jcphuff.c
 *
 * Copyright (C) 1995-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains Huffman entropy encoding routines for progressive JPEG.
 *
 * We do not support output suspension in this module, since the library
 * currently does not allow multiple-scan files to be written with output
 * suspension.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jchuff.h"		/* Declarations shared with jchuff.c */

#ifdef C_PROGRESSIVE_SUPPORTED

/* Expanded entropy encoder object for progressive Huffman encoding. */

typedef struct {
  struct jpeg_entropy_encoder pub; /* public fields */

  /* Mode flag: TRUE for optimization, FALSE for actual data output */
  boolean gather_statistics;

  /* Bit-level coding status.
   * next_output_byte/free_in_buffer are local copies of cinfo->dest fields.
   */
  JOCTET * next_output_byte;	/* => next byte to write in buffer */
  size_t free_in_buffer;	/* # of byte spaces remaining in buffer */
  INT32 put_buffer;		/* current bit-accumulation buffer */
  int put_bits;			/* # of bits now in it */
  j_compress_ptr cinfo;		/* link to cinfo (needed for dump_buffer) */

  /* Coding status for DC components */
  int last_dc_val[MAX_COMPS_IN_SCAN]; /* last DC coef for each component */

  /* Coding status for AC components */
  int ac_tbl_no;		/* the table number of the single component */
  unsigned int EOBRUN;		/* run length of EOBs */
  unsigned int BE;		/* # of buffered correction bits before MCU */
  char * bit_buffer;		/* buffer for correction bits (1 per char) */
  /* packing correction bits tightly would save some space but cost time... */

  unsigned int restarts_to_go;	/* MCUs left in this restart interval */
  int next_restart_num;		/* next restart number to write (0-7) */

  /* Pointers to derived tables (these workspaces have image lifespan).
   * Since any one scan codes only DC or only AC, we only need one set
   * of tables, not one for DC and one for AC.
   */
  c_derived_tbl * derived_tbls[NUM_HUFF_TBLS];

  /* Statistics tables for optimization; again, one set is enough */
  long * count_ptrs[NUM_HUFF_TBLS];
} phuff_entropy_encoder;

typedef phuff_entropy_encoder * phuff_entropy_ptr;

/* MAX_CORR_BITS is the number of bits the AC refinement correction-bit
 * buffer can hold.  Larger sizes may slightly improve compression, but
 * 1000 is already well into the realm of overkill.
 * The minimum safe size is 64 bits.
 */

#define MAX_CORR_BITS  1000	/* Max # of correction bits I can buffer */

/* IRIGHT_SHIFT is like RIGHT_SHIFT, but works on int rather than INT32.
 * We assume that int right shift is unsigned if INT32 right shift is,
 * which should be safe.
 */

#ifdef RIGHT_SHIFT_IS_UNSIGNED
#define ISHIFT_TEMPS	int ishift_temp;
#define IRIGHT_SHIFT(x,shft)  \
	((ishift_temp = (x)) < 0 ? \
	 (ishift_temp >> (shft)) | ((~0) << (16-(shft))) : \
	 (ishift_temp >> (shft)))
#else
#define ISHIFT_TEMPS
#define IRIGHT_SHIFT(x,shft)	((x) >> (shft))
#endif

/* Forward declarations */
METHODDEF(boolean) encode_mcu_DC_first JPP((j_compress_ptr cinfo,
					    JBLOCKROW *MCU_data));
METHODDEF(boolean) encode_mcu_AC_first JPP((j_compress_ptr cinfo,
					    JBLOCKROW *MCU_data));
METHODDEF(boolean) encode_mcu_DC_refine JPP((j_compress_ptr cinfo,
					     JBLOCKROW *MCU_data));
METHODDEF(boolean) encode_mcu_AC_refine JPP((j_compress_ptr cinfo,
					     JBLOCKROW *MCU_data));
METHODDEF(void) finish_pass_phuff JPP((j_compress_ptr cinfo));
METHODDEF(void) finish_pass_gather_phuff JPP((j_compress_ptr cinfo));


/*
 * Initialize for a Huffman-compressed scan using progressive JPEG.
 */

METHODDEF(void)
start_pass_phuff (j_compress_ptr cinfo, boolean gather_statistics)
{  
  phuff_entropy_ptr entropy = (phuff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  boolean is_DC_band;
__boundcheck_metadata_store((void *)(&is_DC_band),(void *)((size_t)(&is_DC_band)+sizeof(is_DC_band)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  tbl;
__boundcheck_metadata_store((void *)(&tbl),(void *)((size_t)(&tbl)+sizeof(tbl)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));


  entropy->cinfo = cinfo;
  entropy->gather_statistics = gather_statistics;

  is_DC_band = (cinfo->Ss == 0);

  /* We assume jcmaster.c already validated the scan parameters. */

  /* Select execution routines */
  if (cinfo->Ah == 0) {
    if (is_DC_band)
      entropy->pub.encode_mcu = encode_mcu_DC_first;
    else
      entropy->pub.encode_mcu = encode_mcu_AC_first;
  } else {
    if (is_DC_band)
      entropy->pub.encode_mcu = encode_mcu_DC_refine;
    else {
      entropy->pub.encode_mcu = encode_mcu_AC_refine;
      /* AC refinement needs a correction bit buffer */
      if (entropy->bit_buffer == NULL)
	entropy->bit_buffer = (char *)
	  (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(134,18,"start_pass_phuff",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				      MAX_CORR_BITS * SIZEOF(char));
    }
  }
  if (gather_statistics)
    entropy->pub.finish_pass = finish_pass_gather_phuff;
  else
    entropy->pub.finish_pass = finish_pass_phuff;

  /* Only DC coefficients may be interleaved, so cinfo->comps_in_scan = 1
   * for AC coefficients.
   */
  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,147,15,"start_pass_phuff",ci)];
    /* Initialize DC predictions to 0 */
    entropy->last_dc_val[_RV_insert_check(0,4,149,5,"start_pass_phuff",ci)] = 0;
    /* Make sure requested tables are present */
    /* (In gather mode, tables need not be allocated yet) */
    if (is_DC_band) {
      if (cinfo->Ah != 0)	/* DC refinement needs no table */
	continue;
      tbl = compptr->dc_tbl_no;
      if (tbl < 0 || tbl >= NUM_HUFF_TBLS ||
	  (cinfo->dc_huff_tbl_ptrs[_RV_insert_check(0,4,157,5,"start_pass_phuff",tbl)] == NULL && !gather_statistics))
	ERREXIT1(cinfo,JERR_NO_HUFF_TABLE, tbl);
    } else {
      entropy->ac_tbl_no = tbl = compptr->ac_tbl_no;
      if (tbl < 0 || tbl >= NUM_HUFF_TBLS ||
          (cinfo->ac_huff_tbl_ptrs[_RV_insert_check(0,4,162,12,"start_pass_phuff",tbl)] == NULL && !gather_statistics))
        ERREXIT1(cinfo,JERR_NO_HUFF_TABLE, tbl);
    }
    if (gather_statistics) {
      /* Allocate and zero the statistics tables */
      /* Note that jpeg_gen_optimal_table expects 257 entries in each table! */
      if (entropy->count_ptrs[_RV_insert_check(0,4,168,11,"start_pass_phuff",tbl)] == NULL)
	entropy->count_ptrs[_RV_insert_check(0,4,169,2,"start_pass_phuff",tbl)] = (long *)
	  (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(170,18,"start_pass_phuff",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				      257 * SIZEOF(long));
      MEMZERO(entropy->count_ptrs[tbl], 257 * SIZEOF(long));
    } else {
      /* Compute derived values for Huffman tables */
      /* We may do this more than once for a table, but it's not expensive */
      if (is_DC_band)
        jpeg_make_c_derived_tbl(cinfo, cinfo->dc_huff_tbl_ptrs[_RV_insert_check(0,4,177,40,"start_pass_phuff",tbl)],
				& entropy->derived_tbls[_RV_insert_check(0,4,178,7,"start_pass_phuff",tbl)]);
      else
        jpeg_make_c_derived_tbl(cinfo, cinfo->ac_huff_tbl_ptrs[_RV_insert_check(0,4,180,40,"start_pass_phuff",tbl)],
				& entropy->derived_tbls[_RV_insert_check(0,4,181,7,"start_pass_phuff",tbl)]);
    }
  }

  /* Initialize AC stuff */
  entropy->EOBRUN = 0;
  entropy->BE = 0;

  /* Initialize bit buffer to empty */
  entropy->put_buffer = 0;
  entropy->put_bits = 0;

  /* Initialize restart stuff */
  entropy->restarts_to_go = cinfo->restart_interval;
  entropy->next_restart_num = 0;
}


/* Outputting bytes to the file.
 * NB: these must be called only when actually outputting,
 * that is, entropy->gather_statistics == FALSE.
 */

/* Emit a byte */
#define emit_byte(entropy,val)  \
	{ *(entropy)->next_output_byte++ = (JOCTET) (val);  \
	  if (--(entropy)->free_in_buffer == 0)  \
	    dump_buffer(entropy); }


LOCAL(void)
dump_buffer (phuff_entropy_ptr entropy)
/* Empty the output buffer; we do not support suspension in this module. */
{
  struct jpeg_destination_mgr * dest = entropy->cinfo->dest;
__boundcheck_metadata_store((void *)(&dest),(void *)((size_t)(&dest)+sizeof(dest)*8-1));


  if (! (*(boolean (*)(j_compress_ptr))(__boundcheck_ptr_reference(217,17,"dump_buffer",(void *)(dest->empty_output_buffer),(void *)dest->empty_output_buffer))) (entropy->cinfo))
    ERREXIT(entropy->cinfo, JERR_CANT_SUSPEND);
  /* After a successful buffer dump, must reset buffer pointers */
  entropy->next_output_byte = dest->next_output_byte;
  entropy->free_in_buffer = dest->free_in_buffer;
}


/* Outputting bits to the file */

/* Only the right 24 bits of put_buffer are used; the valid bits are
 * left-justified in this part.  At most 16 bits can be passed to emit_bits
 * in one call, and we never retain more than 7 bits in put_buffer
 * between calls, so 24 bits are sufficient.
 */

INLINE
LOCAL(void)
emit_bits (phuff_entropy_ptr entropy, unsigned int code, int size)
/* Emit some bits, unless we are in gather mode */
{
  /* This routine is heavily used, so it's worth coding tightly. */
  register INT32 put_buffer = (INT32) code;
  register int put_bits = entropy->put_bits;

  /* if size is 0, caller used an invalid Huffman table entry */
  if (size == 0)
    ERREXIT(entropy->cinfo, JERR_HUFF_MISSING_CODE);

  if (entropy->gather_statistics)
    return;			/* do nothing if we're only getting stats */

  put_buffer &= (((INT32) 1)<<size) - 1; /* mask off any extra bits in code */
  
  put_bits += size;		/* new number of bits in buffer */
  
  put_buffer <<= 24 - put_bits; /* align incoming bits */

  put_buffer |= entropy->put_buffer; /* and merge with old buffer contents */

  while (put_bits >= 8) {
    int c = (int) ((put_buffer >> 16) & 0xFF);
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));

    
    emit_byte(entropy, c);
    if (c == 0xFF) {		/* need to stuff a zero byte? */
      emit_byte(entropy, 0);
    }
    put_buffer <<= 8;
    put_bits -= 8;
  }

  entropy->put_buffer = put_buffer; /* update variables */
  entropy->put_bits = put_bits;
}


LOCAL(void)
flush_bits (phuff_entropy_ptr entropy)
{
  emit_bits(entropy, 0x7F, 7); /* fill any partial byte with ones */
  entropy->put_buffer = 0;     /* and reset bit-buffer to empty */
  entropy->put_bits = 0;
}


/*
 * Emit (or just count) a Huffman symbol.
 */

INLINE
LOCAL(void)
emit_symbol (phuff_entropy_ptr entropy, int tbl_no, int symbol)
{
  if (entropy->gather_statistics)
    entropy->count_ptrs[_RV_insert_check(0,4,291,5,"emit_symbol",tbl_no)][_RV_insert_check(0,4,291,5,"emit_symbol",symbol)]++;
  else {
    c_derived_tbl * tbl = entropy->derived_tbls[_RV_insert_check(0,4,293,27,"emit_symbol",tbl_no)];
__boundcheck_metadata_store((void *)(&tbl),(void *)((size_t)(&tbl)+sizeof(tbl)*8-1));

    emit_bits(entropy, tbl->ehufco[_RV_insert_check(0,256,294,24,"emit_symbol",symbol)], tbl->ehufsi[_RV_insert_check(0,256,294,45,"emit_symbol",symbol)]);
  }
}


/*
 * Emit bits from a correction bit buffer.
 */

LOCAL(void)
emit_buffered_bits (phuff_entropy_ptr entropy, char * bufstart,
		    unsigned int nbits)
{
  if (entropy->gather_statistics)
    return;			/* no real work */

  while (nbits > 0) {
    emit_bits(entropy, (unsigned int) (*(char *)(__boundcheck_ptr_reference(311,41,"emit_buffered_bits",(void *)(bufstart),(void *)(bufstart)))), 1);
    bufstart++;
    nbits--;
  }
}


/*
 * Emit any pending EOBRUN symbol.
 */

LOCAL(void)
emit_eobrun (phuff_entropy_ptr entropy)
{
  register int temp, nbits;

  if (entropy->EOBRUN > 0) {	/* if there is any pending EOBRUN */
    temp = entropy->EOBRUN;
    nbits = 0;
    while ((temp >>= 1))
      nbits++;

    emit_symbol(entropy, entropy->ac_tbl_no, nbits << 4);
    if (nbits)
      emit_bits(entropy, entropy->EOBRUN, nbits);

    entropy->EOBRUN = 0;

    /* Emit any buffered correction bits */
    emit_buffered_bits(entropy, entropy->bit_buffer, entropy->BE);
    entropy->BE = 0;
  }
}


/*
 * Emit a restart marker & resynchronize predictions.
 */

LOCAL(void)
emit_restart (phuff_entropy_ptr entropy, int restart_num)
{
  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));


  emit_eobrun(entropy);

  if (! entropy->gather_statistics) {
    flush_bits(entropy);
    emit_byte(entropy, 0xFF);
    emit_byte(entropy, JPEG_RST0 + restart_num);
  }

  if (entropy->cinfo->Ss == 0) {
    /* Re-initialize DC predictions to 0 */
    for (ci = 0; ci < entropy->cinfo->comps_in_scan; ci++)
      entropy->last_dc_val[_RV_insert_check(0,4,366,7,"emit_restart",ci)] = 0;
  } else {
    /* Re-initialize all AC-related fields to 0 */
    entropy->EOBRUN = 0;
    entropy->BE = 0;
  }
}


/*
 * MCU encoding for DC initial scan (either spectral selection,
 * or first pass of successive approximation).
 */

METHODDEF(boolean)
encode_mcu_DC_first (j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
  phuff_entropy_ptr entropy = (phuff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  register int temp, temp2;
  register int nbits;
  int blkn;
__boundcheck_metadata_store((void *)(&blkn),(void *)((size_t)(&blkn)+sizeof(blkn)*8-1));
int  ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));

  int Al = cinfo->Al;
__boundcheck_metadata_store((void *)(&Al),(void *)((size_t)(&Al)+sizeof(Al)*8-1));

  JBLOCKROW block;
__boundcheck_metadata_store((void *)(&block),(void *)((size_t)(&block)+sizeof(block)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  ISHIFT_TEMPS

  entropy->next_output_byte = cinfo->dest->next_output_byte;
  entropy->free_in_buffer = cinfo->dest->free_in_buffer;

  /* Emit restart marker if needed */
  if (cinfo->restart_interval)
    if (entropy->restarts_to_go == 0)
      emit_restart(entropy, entropy->next_restart_num);

  /* Encode the MCU data blocks */
  for (blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++) {
    block = (*(JBLOCKROW *)(__boundcheck_ptr_reference(402,13,"encode_mcu_DC_first",(void *)(&MCU_data[0]),(void *)(&MCU_data[blkn]))));
    ci = cinfo->MCU_membership[_RV_insert_check(0,10,403,10,"encode_mcu_DC_first",blkn)];
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,404,15,"encode_mcu_DC_first",ci)];

    /* Compute the DC value after the required point transform by Al.
     * This is simply an arithmetic right shift.
     */
    temp2 = IRIGHT_SHIFT((int) ((*block)[0]), Al);

    /* DC differences are figured on the point-transformed values. */
    temp = temp2 - entropy->last_dc_val[_RV_insert_check(0,4,412,20,"encode_mcu_DC_first",ci)];
    entropy->last_dc_val[_RV_insert_check(0,4,413,5,"encode_mcu_DC_first",ci)] = temp2;

    /* Encode the DC coefficient difference per section G.1.2.1 */
    temp2 = temp;
    if (temp < 0) {
      temp = -temp;		/* temp is abs value of input */
      /* For a negative input, want temp2 = bitwise complement of abs(input) */
      /* This code assumes we are on a two's complement machine */
      temp2--;
    }
    
    /* Find the number of bits needed for the magnitude of the coefficient */
    nbits = 0;
    while (temp) {
      nbits++;
      temp >>= 1;
    }
    
    /* Count/emit the Huffman-coded symbol for the number of bits */
    emit_symbol(entropy, compptr->dc_tbl_no, nbits);
    
    /* Emit that number of bits of the value, if positive, */
    /* or the complement of its magnitude, if negative. */
    if (nbits)			/* emit_bits rejects calls with size 0 */
      emit_bits(entropy, (unsigned int) temp2, nbits);
  }

  cinfo->dest->next_output_byte = entropy->next_output_byte;
  cinfo->dest->free_in_buffer = entropy->free_in_buffer;

  /* Update restart-interval state too */
  if (cinfo->restart_interval) {
    if (entropy->restarts_to_go == 0) {
      entropy->restarts_to_go = cinfo->restart_interval;
      entropy->next_restart_num++;
      entropy->next_restart_num &= 7;
    }
    entropy->restarts_to_go--;
  }

  return TRUE;
}


/*
 * MCU encoding for AC initial scan (either spectral selection,
 * or first pass of successive approximation).
 */

METHODDEF(boolean)
encode_mcu_AC_first (j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
  phuff_entropy_ptr entropy = (phuff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  register int temp, temp2;
  register int nbits;
  register int r, k;
  int Se = cinfo->Se;
__boundcheck_metadata_store((void *)(&Se),(void *)((size_t)(&Se)+sizeof(Se)*8-1));

  int Al = cinfo->Al;
__boundcheck_metadata_store((void *)(&Al),(void *)((size_t)(&Al)+sizeof(Al)*8-1));

  JBLOCKROW block;
__boundcheck_metadata_store((void *)(&block),(void *)((size_t)(&block)+sizeof(block)*8-1));


  entropy->next_output_byte = cinfo->dest->next_output_byte;
  entropy->free_in_buffer = cinfo->dest->free_in_buffer;

  /* Emit restart marker if needed */
  if (cinfo->restart_interval)
    if (entropy->restarts_to_go == 0)
      emit_restart(entropy, entropy->next_restart_num);

  /* Encode the MCU data block */
  block = (*(JBLOCKROW *)(__boundcheck_ptr_reference(482,11,"encode_mcu_AC_first",(void *)(&MCU_data[0]),(void *)(&MCU_data[0]))));

  /* Encode the AC coefficients per section G.1.2.2, fig. G.3 */
  
  r = 0;			/* r = run length of zeros */
   
  for (k = cinfo->Ss; k <= Se; k++) {
    if ((temp = (*(JBLOCKROW)(__boundcheck_ptr_reference(489,19,"encode_mcu_AC_first",(void *)(block),(void *)(block))))[(*(const int *)(__boundcheck_ptr_reference(489,26,"encode_mcu_AC_first",(void *)(&jpeg_natural_order[0]),(void *)(&jpeg_natural_order[k]))))]) == 0) {
      r++;
      continue;
    }
    /* We must apply the point transform by Al.  For AC coefficients this
     * is an integer division with rounding towards 0.  To do this portably
     * in C, we shift after obtaining the absolute value; so the code is
     * interwoven with finding the abs value (temp) and output bits (temp2).
     */
    if (temp < 0) {
      temp = -temp;		/* temp is abs value of input */
      temp >>= Al;		/* apply the point transform */
      /* For a negative coef, want temp2 = bitwise complement of abs(coef) */
      temp2 = ~temp;
    } else {
      temp >>= Al;		/* apply the point transform */
      temp2 = temp;
    }
    /* Watch out for case that nonzero coef is zero after point transform */
    if (temp == 0) {
      r++;
      continue;
    }

    /* Emit any pending EOBRUN */
    if (entropy->EOBRUN > 0)
      emit_eobrun(entropy);
    /* if run length > 15, must emit special run-length-16 codes (0xF0) */
    while (r > 15) {
      emit_symbol(entropy, entropy->ac_tbl_no, 0xF0);
      r -= 16;
    }

    /* Find the number of bits needed for the magnitude of the coefficient */
    nbits = 1;			/* there must be at least one 1 bit */
    while ((temp >>= 1))
      nbits++;

    /* Count/emit Huffman symbol for run length / number of bits */
    emit_symbol(entropy, entropy->ac_tbl_no, (r << 4) + nbits);

    /* Emit that number of bits of the value, if positive, */
    /* or the complement of its magnitude, if negative. */
    emit_bits(entropy, (unsigned int) temp2, nbits);

    r = 0;			/* reset zero run length */
  }

  if (r > 0) {			/* If there are trailing zeroes, */
    entropy->EOBRUN++;		/* count an EOB */
    if (entropy->EOBRUN == 0x7FFF)
      emit_eobrun(entropy);	/* force it out to avoid overflow */
  }

  cinfo->dest->next_output_byte = entropy->next_output_byte;
  cinfo->dest->free_in_buffer = entropy->free_in_buffer;

  /* Update restart-interval state too */
  if (cinfo->restart_interval) {
    if (entropy->restarts_to_go == 0) {
      entropy->restarts_to_go = cinfo->restart_interval;
      entropy->next_restart_num++;
      entropy->next_restart_num &= 7;
    }
    entropy->restarts_to_go--;
  }

  return TRUE;
}


/*
 * MCU encoding for DC successive approximation refinement scan.
 * Note: we assume such scans can be multi-component, although the spec
 * is not very clear on the point.
 */

METHODDEF(boolean)
encode_mcu_DC_refine (j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
  phuff_entropy_ptr entropy = (phuff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  register int temp;
  int blkn;
__boundcheck_metadata_store((void *)(&blkn),(void *)((size_t)(&blkn)+sizeof(blkn)*8-1));

  int Al = cinfo->Al;
__boundcheck_metadata_store((void *)(&Al),(void *)((size_t)(&Al)+sizeof(Al)*8-1));

  JBLOCKROW block;
__boundcheck_metadata_store((void *)(&block),(void *)((size_t)(&block)+sizeof(block)*8-1));


  entropy->next_output_byte = cinfo->dest->next_output_byte;
  entropy->free_in_buffer = cinfo->dest->free_in_buffer;

  /* Emit restart marker if needed */
  if (cinfo->restart_interval)
    if (entropy->restarts_to_go == 0)
      emit_restart(entropy, entropy->next_restart_num);

  /* Encode the MCU data blocks */
  for (blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++) {
    block = (*(JBLOCKROW *)(__boundcheck_ptr_reference(585,13,"encode_mcu_DC_refine",(void *)(&MCU_data[0]),(void *)(&MCU_data[blkn]))));

    /* We simply emit the Al'th bit of the DC coefficient value. */
    temp = (*(JBLOCKROW)(__boundcheck_ptr_reference(588,14,"encode_mcu_DC_refine",(void *)(block),(void *)(block))))[0];
    emit_bits(entropy, (unsigned int) (temp >> Al), 1);
  }

  cinfo->dest->next_output_byte = entropy->next_output_byte;
  cinfo->dest->free_in_buffer = entropy->free_in_buffer;

  /* Update restart-interval state too */
  if (cinfo->restart_interval) {
    if (entropy->restarts_to_go == 0) {
      entropy->restarts_to_go = cinfo->restart_interval;
      entropy->next_restart_num++;
      entropy->next_restart_num &= 7;
    }
    entropy->restarts_to_go--;
  }

  return TRUE;
}


/*
 * MCU encoding for AC successive approximation refinement scan.
 */

METHODDEF(boolean)
encode_mcu_AC_refine (j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
  phuff_entropy_ptr entropy = (phuff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  register int temp;
  register int r, k;
  int EOB;
__boundcheck_metadata_store((void *)(&EOB),(void *)((size_t)(&EOB)+sizeof(EOB)*8-1));

  char *BR_buffer;
__boundcheck_metadata_store((void *)(&BR_buffer),(void *)((size_t)(&BR_buffer)+sizeof(BR_buffer)*8-1));

  unsigned int BR;
__boundcheck_metadata_store((void *)(&BR),(void *)((size_t)(&BR)+sizeof(BR)*8-1));

  int Se = cinfo->Se;
__boundcheck_metadata_store((void *)(&Se),(void *)((size_t)(&Se)+sizeof(Se)*8-1));

  int Al = cinfo->Al;
__boundcheck_metadata_store((void *)(&Al),(void *)((size_t)(&Al)+sizeof(Al)*8-1));

  JBLOCKROW block;
__boundcheck_metadata_store((void *)(&block),(void *)((size_t)(&block)+sizeof(block)*8-1));

  int absvalues[DCTSIZE2];__boundcheck_metadata_store(&absvalues[0],&absvalues[64-1]);


  entropy->next_output_byte = cinfo->dest->next_output_byte;
  entropy->free_in_buffer = cinfo->dest->free_in_buffer;

  /* Emit restart marker if needed */
  if (cinfo->restart_interval)
    if (entropy->restarts_to_go == 0)
      emit_restart(entropy, entropy->next_restart_num);

  /* Encode the MCU data block */
  block = (*(JBLOCKROW *)(__boundcheck_ptr_reference(636,11,"encode_mcu_AC_refine",(void *)(&MCU_data[0]),(void *)(&MCU_data[0]))));

  /* It is convenient to make a pre-pass to determine the transformed
   * coefficients' absolute values and the EOB position.
   */
  EOB = 0;
  for (k = cinfo->Ss; k <= Se; k++) {
    temp = (*(JBLOCKROW)(__boundcheck_ptr_reference(643,14,"encode_mcu_AC_refine",(void *)(block),(void *)(block))))[(*(const int *)(__boundcheck_ptr_reference(643,21,"encode_mcu_AC_refine",(void *)(&jpeg_natural_order[0]),(void *)(&jpeg_natural_order[k]))))];
    /* We must apply the point transform by Al.  For AC coefficients this
     * is an integer division with rounding towards 0.  To do this portably
     * in C, we shift after obtaining the absolute value.
     */
    if (temp < 0)
      temp = -temp;		/* temp is abs value of input */
    temp >>= Al;		/* apply the point transform */
    absvalues[_RV_insert_check(0,64,651,5,"encode_mcu_AC_refine",k)] = temp;	/* save abs value for main pass */
    if (temp == 1)
      EOB = k;			/* EOB = index of last newly-nonzero coef */
  }

  /* Encode the AC coefficients per section G.1.2.3, fig. G.7 */
  
  r = 0;			/* r = run length of zeros */
  BR = 0;			/* BR = count of buffered bits added now */
  BR_buffer = entropy->bit_buffer + entropy->BE; /* Append bits to buffer */

  for (k = cinfo->Ss; k <= Se; k++) {
    if ((temp = absvalues[_RV_insert_check(0,64,663,17,"encode_mcu_AC_refine",k)]) == 0) {
      r++;
      continue;
    }

    /* Emit any required ZRLs, but not if they can be folded into EOB */
    while (r > 15 && k <= EOB) {
      /* emit any pending EOBRUN and the BE correction bits */
      emit_eobrun(entropy);
      /* Emit ZRL */
      emit_symbol(entropy, entropy->ac_tbl_no, 0xF0);
      r -= 16;
      /* Emit buffered correction bits that must be associated with ZRL */
      emit_buffered_bits(entropy, BR_buffer, BR);
      BR_buffer = entropy->bit_buffer; /* BE bits are gone now */
      BR = 0;
    }

    /* If the coef was previously nonzero, it only needs a correction bit.
     * NOTE: a straight translation of the spec's figure G.7 would suggest
     * that we also need to test r > 15.  But if r > 15, we can only get here
     * if k > EOB, which implies that this coefficient is not 1.
     */
    if (temp > 1) {
      /* The correction bit is the next bit of the absolute value. */
      (*(char *)(__boundcheck_ptr_reference(688,7,"encode_mcu_AC_refine",(void *)(&BR_buffer[0]),(void *)(&BR_buffer[BR++])))) = (char) (temp & 1);
      continue;
    }

    /* Emit any pending EOBRUN and the BE correction bits */
    emit_eobrun(entropy);

    /* Count/emit Huffman symbol for run length / number of bits */
    emit_symbol(entropy, entropy->ac_tbl_no, (r << 4) + 1);

    /* Emit output bit for newly-nonzero coef */
    temp = ((*(JBLOCKROW)(__boundcheck_ptr_reference(699,15,"encode_mcu_AC_refine",(void *)(block),(void *)(block))))[(*(const int *)(__boundcheck_ptr_reference(699,22,"encode_mcu_AC_refine",(void *)(&jpeg_natural_order[0]),(void *)(&jpeg_natural_order[k]))))] < 0) ? 0 : 1;
    emit_bits(entropy, (unsigned int) temp, 1);

    /* Emit buffered correction bits that must be associated with this code */
    emit_buffered_bits(entropy, BR_buffer, BR);
    BR_buffer = entropy->bit_buffer; /* BE bits are gone now */
    BR = 0;
    r = 0;			/* reset zero run length */
  }

  if (r > 0 || BR > 0) {	/* If there are trailing zeroes, */
    entropy->EOBRUN++;		/* count an EOB */
    entropy->BE += BR;		/* concat my correction bits to older ones */
    /* We force out the EOB if we risk either:
     * 1. overflow of the EOB counter;
     * 2. overflow of the correction bit buffer during the next MCU.
     */
    if (entropy->EOBRUN == 0x7FFF || entropy->BE > (MAX_CORR_BITS-DCTSIZE2+1))
      emit_eobrun(entropy);
  }

  cinfo->dest->next_output_byte = entropy->next_output_byte;
  cinfo->dest->free_in_buffer = entropy->free_in_buffer;

  /* Update restart-interval state too */
  if (cinfo->restart_interval) {
    if (entropy->restarts_to_go == 0) {
      entropy->restarts_to_go = cinfo->restart_interval;
      entropy->next_restart_num++;
      entropy->next_restart_num &= 7;
    }
    entropy->restarts_to_go--;
  }

  return TRUE;
}


/*
 * Finish up at the end of a Huffman-compressed progressive scan.
 */

METHODDEF(void)
finish_pass_phuff (j_compress_ptr cinfo)
{   
  phuff_entropy_ptr entropy = (phuff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));


  entropy->next_output_byte = cinfo->dest->next_output_byte;
  entropy->free_in_buffer = cinfo->dest->free_in_buffer;

  /* Flush out any buffered data */
  emit_eobrun(entropy);
  flush_bits(entropy);

  cinfo->dest->next_output_byte = entropy->next_output_byte;
  cinfo->dest->free_in_buffer = entropy->free_in_buffer;
}


/*
 * Finish up a statistics-gathering pass and create the new Huffman tables.
 */

METHODDEF(void)
finish_pass_gather_phuff (j_compress_ptr cinfo)
{
  phuff_entropy_ptr entropy = (phuff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  boolean is_DC_band;
__boundcheck_metadata_store((void *)(&is_DC_band),(void *)((size_t)(&is_DC_band)+sizeof(is_DC_band)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  tbl;
__boundcheck_metadata_store((void *)(&tbl),(void *)((size_t)(&tbl)+sizeof(tbl)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  JHUFF_TBL **htblptr;
__boundcheck_metadata_store((void *)(&htblptr),(void *)((size_t)(&htblptr)+sizeof(htblptr)*8-1));

  boolean did[NUM_HUFF_TBLS];__boundcheck_metadata_store(&did[0],&did[4-1]);


  /* Flush out buffered data (all we care about is counting the EOB symbol) */
  emit_eobrun(entropy);

  is_DC_band = (cinfo->Ss == 0);

  /* It's important not to apply jpeg_gen_optimal_table more than once
   * per table, because it clobbers the input frequency counts!
   */
  MEMZERO(did, SIZEOF(did));

  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,783,15,"finish_pass_gather_phuff",ci)];
    if (is_DC_band) {
      if (cinfo->Ah != 0)	/* DC refinement needs no table */
	continue;
      tbl = compptr->dc_tbl_no;
    } else {
      tbl = compptr->ac_tbl_no;
    }
    if (! did[_RV_insert_check(0,4,791,11,"finish_pass_gather_phuff",tbl)]) {
      if (is_DC_band)
        htblptr = & cinfo->dc_huff_tbl_ptrs[_RV_insert_check(0,4,793,21,"finish_pass_gather_phuff",tbl)];
      else
        htblptr = & cinfo->ac_huff_tbl_ptrs[_RV_insert_check(0,4,795,21,"finish_pass_gather_phuff",tbl)];
      if (*(JHUFF_TBL **)(__boundcheck_ptr_reference(796,12,"finish_pass_gather_phuff",(void *)(htblptr),(void *)(htblptr))) == NULL)
        *(JHUFF_TBL **)(__boundcheck_ptr_reference(797,10,"finish_pass_gather_phuff",(void *)(htblptr),(void *)(htblptr))) = jpeg_alloc_huff_table((j_common_ptr) cinfo);
      jpeg_gen_optimal_table(cinfo, *(JHUFF_TBL **)(__boundcheck_ptr_reference(798,38,"finish_pass_gather_phuff",(void *)(htblptr),(void *)(htblptr))), entropy->count_ptrs[_RV_insert_check(0,4,798,47,"finish_pass_gather_phuff",tbl)]);
      did[_RV_insert_check(0,4,799,7,"finish_pass_gather_phuff",tbl)] = TRUE;
    }
  }
}


/*
 * Module initialization routine for progressive Huffman entropy encoding.
 */

GLOBAL(void)
jinit_phuff_encoder (j_compress_ptr cinfo)
{
  phuff_entropy_ptr entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  entropy = (phuff_entropy_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(816,19,"jinit_phuff_encoder",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(phuff_entropy_encoder));
  cinfo->entropy = (struct jpeg_entropy_encoder *) entropy;
  entropy->pub.start_pass = start_pass_phuff;

  /* Mark tables unallocated */
  for (i = 0; i < NUM_HUFF_TBLS; i++) {
    entropy->derived_tbls[_RV_insert_check(0,4,823,5,"jinit_phuff_encoder",i)] = NULL;
    entropy->count_ptrs[_RV_insert_check(0,4,824,5,"jinit_phuff_encoder",i)] = NULL;
  }
  entropy->bit_buffer = NULL;	/* needed only in AC refinement scan */
}

#endif /* C_PROGRESSIVE_SUPPORTED */
