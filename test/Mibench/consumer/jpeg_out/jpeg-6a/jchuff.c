/*
 * jchuff.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains Huffman entropy encoding routines.
 *
 * Much of the complexity here has to do with supporting output suspension.
 * If the data destination module demands suspension, we want to be able to
 * back up to the start of the current MCU.  To do this, we copy state
 * variables into local working storage, and update them back to the
 * permanent JPEG objects only upon successful completion of an MCU.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jchuff.h"		/* Declarations shared with jcphuff.c */


/* Expanded entropy encoder object for Huffman encoding.
 *
 * The savable_state subrecord contains fields that change within an MCU,
 * but must not be updated permanently until we complete the MCU.
 */

typedef struct {
  INT32 put_buffer;		/* current bit-accumulation buffer */
  int put_bits;			/* # of bits now in it */
  int last_dc_val[MAX_COMPS_IN_SCAN]; /* last DC coef for each component */
} savable_state;

/* This macro is to work around compilers with missing or broken
 * structure assignment.  You'll need to fix this code if you have
 * such a compiler and you change MAX_COMPS_IN_SCAN.
 */

#ifndef NO_STRUCT_ASSIGN
#define ASSIGN_STATE(dest,src)  ((dest) = (src))
#else
#if MAX_COMPS_IN_SCAN == 4
#define ASSIGN_STATE(dest,src)  \
	((dest).put_buffer = (src).put_buffer, \
	 (dest).put_bits = (src).put_bits, \
	 (dest).last_dc_val[0] = (src).last_dc_val[0], \
	 (dest).last_dc_val[1] = (src).last_dc_val[1], \
	 (dest).last_dc_val[2] = (src).last_dc_val[2], \
	 (dest).last_dc_val[3] = (src).last_dc_val[3])
#endif
#endif


typedef struct {
  struct jpeg_entropy_encoder pub; /* public fields */

  savable_state saved;		/* Bit buffer & DC state at start of MCU */

  /* These fields are NOT loaded into local working state. */
  unsigned int restarts_to_go;	/* MCUs left in this restart interval */
  int next_restart_num;		/* next restart number to write (0-7) */

  /* Pointers to derived tables (these workspaces have image lifespan) */
  c_derived_tbl * dc_derived_tbls[NUM_HUFF_TBLS];
  c_derived_tbl * ac_derived_tbls[NUM_HUFF_TBLS];

#ifdef ENTROPY_OPT_SUPPORTED	/* Statistics tables for optimization */
  long * dc_count_ptrs[NUM_HUFF_TBLS];
  long * ac_count_ptrs[NUM_HUFF_TBLS];
#endif
} huff_entropy_encoder;

typedef huff_entropy_encoder * huff_entropy_ptr;

/* Working state while writing an MCU.
 * This struct contains all the fields that are needed by subroutines.
 */

typedef struct {
  JOCTET * next_output_byte;	/* => next byte to write in buffer */
  size_t free_in_buffer;	/* # of byte spaces remaining in buffer */
  savable_state cur;		/* Current bit buffer & DC state */
  j_compress_ptr cinfo;		/* dump_buffer needs access to this */
} working_state;


/* Forward declarations */
METHODDEF(boolean) encode_mcu_huff JPP((j_compress_ptr cinfo,
					JBLOCKROW *MCU_data));
METHODDEF(void) finish_pass_huff JPP((j_compress_ptr cinfo));
#ifdef ENTROPY_OPT_SUPPORTED
METHODDEF(boolean) encode_mcu_gather JPP((j_compress_ptr cinfo,
					  JBLOCKROW *MCU_data));
METHODDEF(void) finish_pass_gather JPP((j_compress_ptr cinfo));
#endif


/*
 * Initialize for a Huffman-compressed scan.
 * If gather_statistics is TRUE, we do not output anything during the scan,
 * just count the Huffman symbols used and generate Huffman code tables.
 */

METHODDEF(void)
start_pass_huff (j_compress_ptr cinfo, boolean gather_statistics)
{
  huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  dctbl;
__boundcheck_metadata_store((void *)(&dctbl),(void *)((size_t)(&dctbl)+sizeof(dctbl)*8-1));
int  actbl;
__boundcheck_metadata_store((void *)(&actbl),(void *)((size_t)(&actbl)+sizeof(actbl)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));


  if (gather_statistics) {
#ifdef ENTROPY_OPT_SUPPORTED
    entropy->pub.encode_mcu = encode_mcu_gather;
    entropy->pub.finish_pass = finish_pass_gather;
#else
    ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif
  } else {
    entropy->pub.encode_mcu = encode_mcu_huff;
    entropy->pub.finish_pass = finish_pass_huff;
  }

  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,125,15,"start_pass_huff",ci)];
    dctbl = compptr->dc_tbl_no;
    actbl = compptr->ac_tbl_no;
    /* Make sure requested tables are present */
    /* (In gather mode, tables need not be allocated yet) */
    if (dctbl < 0 || dctbl >= NUM_HUFF_TBLS ||
	(cinfo->dc_huff_tbl_ptrs[_RV_insert_check(0,4,131,3,"start_pass_huff",dctbl)] == NULL && !gather_statistics))
      ERREXIT1(cinfo, JERR_NO_HUFF_TABLE, dctbl);
    if (actbl < 0 || actbl >= NUM_HUFF_TBLS ||
	(cinfo->ac_huff_tbl_ptrs[_RV_insert_check(0,4,134,3,"start_pass_huff",actbl)] == NULL && !gather_statistics))
      ERREXIT1(cinfo, JERR_NO_HUFF_TABLE, actbl);
    if (gather_statistics) {
#ifdef ENTROPY_OPT_SUPPORTED
      /* Allocate and zero the statistics tables */
      /* Note that jpeg_gen_optimal_table expects 257 entries in each table! */
      if (entropy->dc_count_ptrs[_RV_insert_check(0,4,140,11,"start_pass_huff",dctbl)] == NULL)
	entropy->dc_count_ptrs[_RV_insert_check(0,4,141,2,"start_pass_huff",dctbl)] = (long *)
	  (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(142,18,"start_pass_huff",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				      257 * SIZEOF(long));
      MEMZERO(entropy->dc_count_ptrs[dctbl], 257 * SIZEOF(long));
      if (entropy->ac_count_ptrs[_RV_insert_check(0,4,145,11,"start_pass_huff",actbl)] == NULL)
	entropy->ac_count_ptrs[_RV_insert_check(0,4,146,2,"start_pass_huff",actbl)] = (long *)
	  (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(147,18,"start_pass_huff",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				      257 * SIZEOF(long));
      MEMZERO(entropy->ac_count_ptrs[actbl], 257 * SIZEOF(long));
#endif
    } else {
      /* Compute derived values for Huffman tables */
      /* We may do this more than once for a table, but it's not expensive */
      jpeg_make_c_derived_tbl(cinfo, cinfo->dc_huff_tbl_ptrs[_RV_insert_check(0,4,154,38,"start_pass_huff",dctbl)],
			      & entropy->dc_derived_tbls[_RV_insert_check(0,4,155,12,"start_pass_huff",dctbl)]);
      jpeg_make_c_derived_tbl(cinfo, cinfo->ac_huff_tbl_ptrs[_RV_insert_check(0,4,156,38,"start_pass_huff",actbl)],
			      & entropy->ac_derived_tbls[_RV_insert_check(0,4,157,12,"start_pass_huff",actbl)]);
    }
    /* Initialize DC predictions to 0 */
    entropy->saved.last_dc_val[_RV_insert_check(0,4,160,5,"start_pass_huff",ci)] = 0;
  }

  /* Initialize bit buffer to empty */
  entropy->saved.put_buffer = 0;
  entropy->saved.put_bits = 0;

  /* Initialize restart stuff */
  entropy->restarts_to_go = cinfo->restart_interval;
  entropy->next_restart_num = 0;
}


/*
 * Compute the derived values for a Huffman table.
 * Note this is also used by jcphuff.c.
 */

GLOBAL(void)
jpeg_make_c_derived_tbl (j_compress_ptr cinfo, JHUFF_TBL * htbl,
			 c_derived_tbl ** pdtbl)
{
  c_derived_tbl *dtbl;
__boundcheck_metadata_store((void *)(&dtbl),(void *)((size_t)(&dtbl)+sizeof(dtbl)*8-1));

  int p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
int  i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  l;
__boundcheck_metadata_store((void *)(&l),(void *)((size_t)(&l)+sizeof(l)*8-1));
int  lastp;
__boundcheck_metadata_store((void *)(&lastp),(void *)((size_t)(&lastp)+sizeof(lastp)*8-1));
int  si;
__boundcheck_metadata_store((void *)(&si),(void *)((size_t)(&si)+sizeof(si)*8-1));

  char huffsize[257];__boundcheck_metadata_store(&huffsize[0],&huffsize[257-1]);

  unsigned int huffcode[257];__boundcheck_metadata_store(&huffcode[0],&huffcode[257-1]);

  unsigned int code;
__boundcheck_metadata_store((void *)(&code),(void *)((size_t)(&code)+sizeof(code)*8-1));


  /* Allocate a workspace if we haven't already done so. */
  if (*(c_derived_tbl **)(__boundcheck_ptr_reference(189,8,"jpeg_make_c_derived_tbl",(void *)(pdtbl),(void *)(pdtbl))) == NULL)
    *(c_derived_tbl **)(__boundcheck_ptr_reference(190,6,"jpeg_make_c_derived_tbl",(void *)(pdtbl),(void *)(pdtbl))) = (c_derived_tbl *)
      (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(191,21,"jpeg_make_c_derived_tbl",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  SIZEOF(c_derived_tbl));
  dtbl = *(c_derived_tbl **)(__boundcheck_ptr_reference(193,11,"jpeg_make_c_derived_tbl",(void *)(pdtbl),(void *)(pdtbl)));
  
  /* Figure C.1: make table of Huffman code length for each symbol */
  /* Note that this is in code-length order. */

  p = 0;
  for (l = 1; l <= 16; l++) {
    for (i = 1; i <= (int) htbl->bits[_RV_insert_check(0,17,200,28,"jpeg_make_c_derived_tbl",l)]; i++)
      huffsize[_RV_insert_check(0,257,201,7,"jpeg_make_c_derived_tbl",p++)] = (char) l;
  }
  huffsize[_RV_insert_check(0,257,203,3,"jpeg_make_c_derived_tbl",p)] = 0;
  lastp = p;
  
  /* Figure C.2: generate the codes themselves */
  /* Note that this is in code-length order. */
  
  code = 0;
  si = huffsize[_RV_insert_check(0,257,210,8,"jpeg_make_c_derived_tbl",0)];
  p = 0;
  while (huffsize[_RV_insert_check(0,257,212,10,"jpeg_make_c_derived_tbl",p)]) {
    while (((int) huffsize[_RV_insert_check(0,257,213,19,"jpeg_make_c_derived_tbl",p)]) == si) {
      huffcode[_RV_insert_check(0,257,214,7,"jpeg_make_c_derived_tbl",p++)] = code;
      code++;
    }
    code <<= 1;
    si++;
  }
  
  /* Figure C.3: generate encoding tables */
  /* These are code and size indexed by symbol value */

  /* Set any codeless symbols to have code length 0;
   * this allows emit_bits to detect any attempt to emit such symbols.
   */
  MEMZERO(dtbl->ehufsi, SIZEOF(dtbl->ehufsi));

  for (p = 0; p < lastp; p++) {
    dtbl->ehufco[_RV_insert_check(0,256,230,5,"jpeg_make_c_derived_tbl",htbl->huffval[_RV_insert_check(0,256,230,18,"jpeg_make_c_derived_tbl",p)])] = huffcode[_RV_insert_check(0,257,230,38,"jpeg_make_c_derived_tbl",p)];
    dtbl->ehufsi[_RV_insert_check(0,256,231,5,"jpeg_make_c_derived_tbl",htbl->huffval[_RV_insert_check(0,256,231,18,"jpeg_make_c_derived_tbl",p)])] = huffsize[_RV_insert_check(0,257,231,38,"jpeg_make_c_derived_tbl",p)];
  }
}


/* Outputting bytes to the file */

/* Emit a byte, taking 'action' if must suspend. */
#define emit_byte(state,val,action)  \
	{ *(state)->next_output_byte++ = (JOCTET) (val);  \
	  if (--(state)->free_in_buffer == 0)  \
	    if (! dump_buffer(state))  \
	      { action; } }


LOCAL(boolean)
dump_buffer (working_state * state)
/* Empty the output buffer; return TRUE if successful, FALSE if must suspend */
{
  struct jpeg_destination_mgr * dest = state->cinfo->dest;
__boundcheck_metadata_store((void *)(&dest),(void *)((size_t)(&dest)+sizeof(dest)*8-1));


  if (! (*(boolean (*)(j_compress_ptr))(__boundcheck_ptr_reference(252,17,"dump_buffer",(void *)(dest->empty_output_buffer),(void *)dest->empty_output_buffer))) (state->cinfo))
    return FALSE;
  /* After a successful buffer dump, must reset buffer pointers */
  state->next_output_byte = dest->next_output_byte;
  state->free_in_buffer = dest->free_in_buffer;
  return TRUE;
}


/* Outputting bits to the file */

/* Only the right 24 bits of put_buffer are used; the valid bits are
 * left-justified in this part.  At most 16 bits can be passed to emit_bits
 * in one call, and we never retain more than 7 bits in put_buffer
 * between calls, so 24 bits are sufficient.
 */

INLINE
LOCAL(boolean)
emit_bits (working_state * state, unsigned int code, int size)
/* Emit some bits; return TRUE if successful, FALSE if must suspend */
{
  /* This routine is heavily used, so it's worth coding tightly. */
  register INT32 put_buffer = (INT32) code;
  register int put_bits = state->cur.put_bits;

  /* if size is 0, caller used an invalid Huffman table entry */
  if (size == 0)
    ERREXIT(state->cinfo, JERR_HUFF_MISSING_CODE);

  put_buffer &= (((INT32) 1)<<size) - 1; /* mask off any extra bits in code */
  
  put_bits += size;		/* new number of bits in buffer */
  
  put_buffer <<= 24 - put_bits; /* align incoming bits */

  put_buffer |= state->cur.put_buffer; /* and merge with old buffer contents */
  
  while (put_bits >= 8) {
    int c = (int) ((put_buffer >> 16) & 0xFF);
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));

    
    emit_byte(state, c, return FALSE);
    if (c == 0xFF) {		/* need to stuff a zero byte? */
      emit_byte(state, 0, return FALSE);
    }
    put_buffer <<= 8;
    put_bits -= 8;
  }

  state->cur.put_buffer = put_buffer; /* update state variables */
  state->cur.put_bits = put_bits;

  return TRUE;
}


LOCAL(boolean)
flush_bits (working_state * state)
{
  if (! emit_bits(state, 0x7F, 7)) /* fill any partial byte with ones */
    return FALSE;
  state->cur.put_buffer = 0;	/* and reset bit-buffer to empty */
  state->cur.put_bits = 0;
  return TRUE;
}


/* Encode a single block's worth of coefficients */

LOCAL(boolean)
encode_one_block (working_state * state, JCOEFPTR block, int last_dc_val,
		  c_derived_tbl *dctbl, c_derived_tbl *actbl)
{
  register int temp, temp2;
  register int nbits;
  register int k, r, i;
  
  /* Encode the DC coefficient difference per section F.1.2.1 */
  
  temp = temp2 = (*(JCOEF *)(__boundcheck_ptr_reference(331,18,"encode_one_block",(void *)(&block[0]),(void *)(&block[0])))) - last_dc_val;

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
  
  /* Emit the Huffman-coded symbol for the number of bits */
  if (! emit_bits(state, dctbl->ehufco[_RV_insert_check(0,256,348,26,"encode_one_block",nbits)], dctbl->ehufsi[_RV_insert_check(0,256,348,48,"encode_one_block",nbits)]))
    return FALSE;

  /* Emit that number of bits of the value, if positive, */
  /* or the complement of its magnitude, if negative. */
  if (nbits)			/* emit_bits rejects calls with size 0 */
    if (! emit_bits(state, (unsigned int) temp2, nbits))
      return FALSE;

  /* Encode the AC coefficients per section F.1.2.2 */
  
  r = 0;			/* r = run length of zeros */
  
  for (k = 1; k < DCTSIZE2; k++) {
    if ((temp = (*(JCOEF *)(__boundcheck_ptr_reference(362,17,"encode_one_block",(void *)(&block[0]),(void *)(&block[jpeg(*(const int *)(__boundcheck_ptr_reference(362,23,"encode_one_block",(void *)(&jpeg_natural_order[0]),(void *)(&jpeg_natural_order[k])))))) == 0) {
      r++;
    } else {
      /* if run length > 15, must emit special run-length-16 codes (0xF0) */
      while (r > 15) {
	if (! emit_bits(state, actbl->ehufco[_RV_insert_check(0,256,367,25,"encode_one_block",0xF0)], actbl->ehufsi[_RV_insert_check(0,256,367,46,"encode_one_block",0xF0)]))
	  return FALSE;
	r -= 16;
      }

      temp2 = temp;
      if (temp < 0) {
	temp = -temp;		/* temp is abs value of input */
	/* This code assumes we are on a two's complement machine */
	temp2--;
      }
      
      /* Find the number of bits needed for the magnitude of the coefficient */
      nbits = 1;		/* there must be at least one 1 bit */
      while ((temp >>= 1))
	nbits++;
      
      /* Emit Huffman symbol for run length / number of bits */
      i = (r << 4) + nbits;
      if (! emit_bits(state, actbl->ehufco[_RV_insert_check(0,256,386,30,"encode_one_block",i)], actbl->ehufsi[_RV_insert_check(0,256,386,48,"encode_one_block",i)]))
	return FALSE;

      /* Emit that number of bits of the value, if positive, */
      /* or the complement of its magnitude, if negative. */
      if (! emit_bits(state, (unsigned int) temp2, nbits))
	return FALSE;
      
      r = 0;
    }
  }

  /* If the last coef(s) were zero, emit an end-of-block code */
  if (r > 0)
    if (! emit_bits(state, actbl->ehufco[_RV_insert_check(0,256,400,28,"encode_one_block",0)], actbl->ehufsi[_RV_insert_check(0,256,400,46,"encode_one_block",0)]))
      return FALSE;

  return TRUE;
}


/*
 * Emit a restart marker & resynchronize predictions.
 */

LOCAL(boolean)
emit_restart (working_state * state, int restart_num)
{
  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));


  if (! flush_bits(state))
    return FALSE;

  emit_byte(state, 0xFF, return FALSE);
  emit_byte(state, JPEG_RST0 + restart_num, return FALSE);

  /* Re-initialize DC predictions to 0 */
  for (ci = 0; ci < state->cinfo->comps_in_scan; ci++)
    state->cur.last_dc_val[_RV_insert_check(0,4,424,5,"emit_restart",ci)] = 0;

  /* The restart counter is not updated until we successfully write the MCU. */

  return TRUE;
}


/*
 * Encode and output one MCU's worth of Huffman-compressed coefficients.
 */

METHODDEF(boolean)
encode_mcu_huff (j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
  huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  working_state state;
__boundcheck_metadata_store((void *)(&state),(void *)((size_t)(&state)+sizeof(state)*8-1));

  int blkn;
__boundcheck_metadata_store((void *)(&blkn),(void *)((size_t)(&blkn)+sizeof(blkn)*8-1));
int  ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));


  /* Load up working state */
  state.next_output_byte = cinfo->dest->next_output_byte;
  state.free_in_buffer = cinfo->dest->free_in_buffer;
  ASSIGN_STATE(state.cur, entropy->saved);
  state.cinfo = cinfo;

  /* Emit restart marker if needed */
  if (cinfo->restart_interval) {
    if (entropy->restarts_to_go == 0)
      if (! emit_restart(&state, entropy->next_restart_num))
	return FALSE;
  }

  /* Encode the MCU data blocks */
  for (blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++) {
    ci = cinfo->MCU_membership[_RV_insert_check(0,10,459,10,"encode_mcu_huff",blkn)];
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,460,15,"encode_mcu_huff",ci)];
    if (! encode_one_block(&state,
			   (*(JBLOCK *)(__boundcheck_ptr_reference(462,7,"encode_mcu_huff",(void *)(&MCU_data[blkn][0]),(void *)(&MCU_data[blkn][0])))), state.cur.last_dc_val[_RV_insert_check(0,4,462,26,"encode_mcu_huff",ci)],
			   entropy->dc_derived_tbls[_RV_insert_check(0,4,463,7,"encode_mcu_huff",compptr->dc_tbl_no)],
			   entropy->ac_derived_tbls[_RV_insert_check(0,4,464,7,"encode_mcu_huff",compptr->ac_tbl_no)]))
      return FALSE;
    /* Update last_dc_val */
    state.cur.last_dc_val[_RV_insert_check(0,4,467,5,"encode_mcu_huff",ci)] = (*(JCOEF *)(__boundcheck_ptr_reference(467,33,"encode_mcu_huff",(void *)(&MCU_data[blkn][0][0]),(void *)(&MCU_data[blkn][0][0]))));
  }

  /* Completed MCU, so update state */
  cinfo->dest->next_output_byte = state.next_output_byte;
  cinfo->dest->free_in_buffer = state.free_in_buffer;
  ASSIGN_STATE(entropy->saved, state.cur);

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
 * Finish up at the end of a Huffman-compressed scan.
 */

METHODDEF(void)
finish_pass_huff (j_compress_ptr cinfo)
{
  huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  working_state state;
__boundcheck_metadata_store((void *)(&state),(void *)((size_t)(&state)+sizeof(state)*8-1));


  /* Load up working state ... flush_bits needs it */
  state.next_output_byte = cinfo->dest->next_output_byte;
  state.free_in_buffer = cinfo->dest->free_in_buffer;
  ASSIGN_STATE(state.cur, entropy->saved);
  state.cinfo = cinfo;

  /* Flush out the last data */
  if (! flush_bits(&state))
    ERREXIT(cinfo, JERR_CANT_SUSPEND);

  /* Update state */
  cinfo->dest->next_output_byte = state.next_output_byte;
  cinfo->dest->free_in_buffer = state.free_in_buffer;
  ASSIGN_STATE(entropy->saved, state.cur);
}


/*
 * Huffman coding optimization.
 *
 * This actually is optimization, in the sense that we find the best possible
 * Huffman table(s) for the given data.  We first scan the supplied data and
 * count the number of uses of each symbol that is to be Huffman-coded.
 * (This process must agree with the code above.)  Then we build an
 * optimal Huffman coding tree for the observed counts.
 *
 * The JPEG standard requires Huffman codes to be no more than 16 bits long.
 * If some symbols have a very small but nonzero probability, the Huffman tree
 * must be adjusted to meet the code length restriction.  We currently use
 * the adjustment method suggested in the JPEG spec.  This method is *not*
 * optimal; it may not choose the best possible limited-length code.  But
 * since the symbols involved are infrequently used, it's not clear that
 * going to extra trouble is worthwhile.
 */

#ifdef ENTROPY_OPT_SUPPORTED


/* Process a single block's worth of coefficients */

LOCAL(void)
htest_one_block (JCOEFPTR block, int last_dc_val,
		 long dc_counts[], long ac_counts[])
{
  register int temp;
  register int nbits;
  register int k, r;
  
  /* Encode the DC coefficient difference per section F.1.2.1 */
  
  temp = (*(JCOEF *)(__boundcheck_ptr_reference(549,10,"htest_one_block",(void *)(&block[0]),(void *)(&block[0])))) - last_dc_val;
  if (temp < 0)
    temp = -temp;
  
  /* Find the number of bits needed for the magnitude of the coefficient */
  nbits = 0;
  while (temp) {
    nbits++;
    temp >>= 1;
  }

  /* Count the Huffman symbol for the number of bits */
  (*(long *)(__boundcheck_ptr_reference(561,3,"htest_one_block",(void *)(&dc_counts[0]),(void *)(&dc_counts[nbits]))))++;
  
  /* Encode the AC coefficients per section F.1.2.2 */
  
  r = 0;			/* r = run length of zeros */
  
  for (k = 1; k < DCTSIZE2; k++) {
    if ((temp = (*(JCOEF *)(__boundcheck_ptr_reference(568,17,"htest_one_block",(void *)(&block[0]),(void *)(&block[jpeg(*(const int *)(__boundcheck_ptr_reference(568,23,"htest_one_block",(void *)(&jpeg_natural_order[0]),(void *)(&jpeg_natural_order[k])))))) == 0) {
      r++;
    } else {
      /* if run length > 15, must emit special run-length-16 codes (0xF0) */
      while (r > 15) {
	(*(long *)(__boundcheck_ptr_reference(573,2,"htest_one_block",(void *)(&ac_counts[0]),(void *)(&ac_counts[240]))))++;
	r -= 16;
      }
      
      /* Find the number of bits needed for the magnitude of the coefficient */
      if (temp < 0)
	temp = -temp;
      
      /* Find the number of bits needed for the magnitude of the coefficient */
      nbits = 1;		/* there must be at least one 1 bit */
      while ((temp >>= 1))
	nbits++;
      
      /* Count Huffman symbol for run length / number of bits */
      (*(long *)(__boundcheck_ptr_reference(587,7,"htest_one_block",(void *)(&ac_counts[0]),(void *)(&ac_counts[(r << 4) + nbits]))))++;
      
      r = 0;
    }
  }

  /* If the last coef(s) were zero, emit an end-of-block code */
  if (r > 0)
    (*(long *)(__boundcheck_ptr_reference(595,5,"htest_one_block",(void *)(&ac_counts[0]),(void *)(&ac_counts[0]))))++;
}


/*
 * Trial-encode one MCU's worth of Huffman-compressed coefficients.
 * No data is actually output, so no suspension return is possible.
 */

METHODDEF(boolean)
encode_mcu_gather (j_compress_ptr cinfo, JBLOCKROW *MCU_data)
{
  huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  int blkn;
__boundcheck_metadata_store((void *)(&blkn),(void *)((size_t)(&blkn)+sizeof(blkn)*8-1));
int  ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));


  /* Take care of restart intervals if needed */
  if (cinfo->restart_interval) {
    if (entropy->restarts_to_go == 0) {
      /* Re-initialize DC predictions to 0 */
      for (ci = 0; ci < cinfo->comps_in_scan; ci++)
	entropy->saved.last_dc_val[_RV_insert_check(0,4,616,2,"encode_mcu_gather",ci)] = 0;
      /* Update restart state */
      entropy->restarts_to_go = cinfo->restart_interval;
    }
    entropy->restarts_to_go--;
  }

  for (blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++) {
    ci = cinfo->MCU_membership[_RV_insert_check(0,10,624,10,"encode_mcu_gather",blkn)];
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,625,15,"encode_mcu_gather",ci)];
    htest_one_block((*(JBLOCK *)(__boundcheck_ptr_reference(626,21,"encode_mcu_gather",(void *)(&MCU_data[blkn][0]),(void *)(&MCU_data[blkn][0])))), entropy->saved.last_dc_val[_RV_insert_check(0,4,626,40,"encode_mcu_gather",ci)],
		    entropy->dc_count_ptrs[_RV_insert_check(0,4,627,7,"encode_mcu_gather",compptr->dc_tbl_no)],
		    entropy->ac_count_ptrs[_RV_insert_check(0,4,628,7,"encode_mcu_gather",compptr->ac_tbl_no)]);
    entropy->saved.last_dc_val[_RV_insert_check(0,4,629,5,"encode_mcu_gather",ci)] = (*(JCOEF *)(__boundcheck_ptr_reference(629,38,"encode_mcu_gather",(void *)(&MCU_data[blkn][0][0]),(void *)(&MCU_data[blkn][0][0]))));
  }

  return TRUE;
}


/*
 * Generate the optimal coding for the given counts, fill htbl.
 * Note this is also used by jcphuff.c.
 */

GLOBAL(void)
jpeg_gen_optimal_table (j_compress_ptr cinfo, JHUFF_TBL * htbl, long freq[])
{
#define MAX_CLEN 32		/* assumed maximum initial code length */
  UINT8 bits[MAX_CLEN+1];__boundcheck_metadata_store(&bits[0],&bits[33-1]);
	/* bits[k] = # of symbols with code length k */
  int codesize[257];__boundcheck_metadata_store(&codesize[0],&codesize[257-1]);
		/* codesize[k] = code length of symbol k */
  int others[257];__boundcheck_metadata_store(&others[0],&others[257-1]);
		/* next symbol in current branch of tree */
  int c1;
__boundcheck_metadata_store((void *)(&c1),(void *)((size_t)(&c1)+sizeof(c1)*8-1));
int  c2;
__boundcheck_metadata_store((void *)(&c2),(void *)((size_t)(&c2)+sizeof(c2)*8-1));

  int p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
int  i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));

  long v;
__boundcheck_metadata_store((void *)(&v),(void *)((size_t)(&v)+sizeof(v)*8-1));


  /* This algorithm is explained in section K.2 of the JPEG standard */

  MEMZERO(bits, SIZEOF(bits));
  MEMZERO(codesize, SIZEOF(codesize));
  for (i = 0; i < 257; i++)
    others[_RV_insert_check(0,257,657,5,"jpeg_gen_optimal_table",i)] = -1;		/* init links to empty */
  
  (*(long *)(__boundcheck_ptr_reference(659,3,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[256])))) = 1;		/* make sure there is a nonzero count */
  /* Including the pseudo-symbol 256 in the Huffman procedure guarantees
   * that no real symbol is given code-value of all ones, because 256
   * will be placed in the largest codeword category.
   */

  /* Huffman's basic algorithm to assign optimal code lengths to symbols */

  for (;;) {
    /* Find the smallest nonzero frequency, set c1 = its symbol */
    /* In case of ties, take the larger symbol number */
    c1 = -1;
    v = 1000000000L;
    for (i = 0; i <= 256; i++) {
      if ((*(long *)(__boundcheck_ptr_reference(673,11,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[i])))) && (*(long *)(__boundcheck_ptr_reference(673,22,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[i])))) <= v) {
	v = (*(long *)(__boundcheck_ptr_reference(674,6,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[i]))));
	c1 = i;
      }
    }

    /* Find the next smallest nonzero frequency, set c2 = its symbol */
    /* In case of ties, take the larger symbol number */
    c2 = -1;
    v = 1000000000L;
    for (i = 0; i <= 256; i++) {
      if ((*(long *)(__boundcheck_ptr_reference(684,11,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[i])))) && (*(long *)(__boundcheck_ptr_reference(684,22,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[i])))) <= v && i != c1) {
	v = (*(long *)(__boundcheck_ptr_reference(685,6,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[i]))));
	c2 = i;
      }
    }

    /* Done if we've merged everything into one frequency */
    if (c2 < 0)
      break;
    
    /* Else merge the two counts/trees */
    (*(long *)(__boundcheck_ptr_reference(695,5,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[c1])))) += (*(long *)(__boundcheck_ptr_reference(695,17,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[c2]))));
    (*(long *)(__boundcheck_ptr_reference(696,5,"jpeg_gen_optimal_table",(void *)(&freq[0]),(void *)(&freq[c2])))) = 0;

    /* Increment the codesize of everything in c1's tree branch */
    codesize[_RV_insert_check(0,257,699,5,"jpeg_gen_optimal_table",c1)]++;
    while (others[_RV_insert_check(0,257,700,12,"jpeg_gen_optimal_table",c1)] >= 0) {
      c1 = others[_RV_insert_check(0,257,701,12,"jpeg_gen_optimal_table",c1)];
      codesize[_RV_insert_check(0,257,702,7,"jpeg_gen_optimal_table",c1)]++;
    }
    
    others[_RV_insert_check(0,257,705,5,"jpeg_gen_optimal_table",c1)] = c2;		/* chain c2 onto c1's tree branch */
    
    /* Increment the codesize of everything in c2's tree branch */
    codesize[_RV_insert_check(0,257,708,5,"jpeg_gen_optimal_table",c2)]++;
    while (others[_RV_insert_check(0,257,709,12,"jpeg_gen_optimal_table",c2)] >= 0) {
      c2 = others[_RV_insert_check(0,257,710,12,"jpeg_gen_optimal_table",c2)];
      codesize[_RV_insert_check(0,257,711,7,"jpeg_gen_optimal_table",c2)]++;
    }
  }

  /* Now count the number of symbols of each code length */
  for (i = 0; i <= 256; i++) {
    if (codesize[_RV_insert_check(0,257,717,9,"jpeg_gen_optimal_table",i)]) {
      /* The JPEG standard seems to think that this can't happen, */
      /* but I'm paranoid... */
      if (codesize[_RV_insert_check(0,257,720,11,"jpeg_gen_optimal_table",i)] > MAX_CLEN)
	ERREXIT(cinfo, JERR_HUFF_CLEN_OVERFLOW);

      bits[_RV_insert_check(0,33,723,7,"jpeg_gen_optimal_table",codesize[_RV_insert_check(0,257,723,12,"jpeg_gen_optimal_table",i)])]++;
    }
  }

  /* JPEG doesn't allow symbols with code lengths over 16 bits, so if the pure
   * Huffman procedure assigned any such lengths, we must adjust the coding.
   * Here is what the JPEG spec says about how this next bit works:
   * Since symbols are paired for the longest Huffman code, the symbols are
   * removed from this length category two at a time.  The prefix for the pair
   * (which is one bit shorter) is allocated to one of the pair; then,
   * skipping the BITS entry for that prefix length, a code word from the next
   * shortest nonzero BITS entry is converted into a prefix for two code words
   * one bit longer.
   */
  
  for (i = MAX_CLEN; i > 16; i--) {
    while (bits[_RV_insert_check(0,33,739,12,"jpeg_gen_optimal_table",i)] > 0) {
      j = i - 2;		/* find length of new prefix to be used */
      while (bits[_RV_insert_check(0,33,741,14,"jpeg_gen_optimal_table",j)] == 0)
	j--;
      
      bits[_RV_insert_check(0,33,744,7,"jpeg_gen_optimal_table",i)] -= 2;		/* remove two symbols */
      bits[_RV_insert_check(0,33,745,7,"jpeg_gen_optimal_table",i-1)]++;		/* one goes in this length */
      bits[_RV_insert_check(0,33,746,7,"jpeg_gen_optimal_table",j+1)] += 2;		/* two new symbols in this length */
      bits[_RV_insert_check(0,33,747,7,"jpeg_gen_optimal_table",j)]--;		/* symbol of this length is now a prefix */
    }
  }

  /* Remove the count for the pseudo-symbol 256 from the largest codelength */
  while (bits[_RV_insert_check(0,33,752,10,"jpeg_gen_optimal_table",i)] == 0)		/* find largest codelength still in use */
    i--;
  bits[_RV_insert_check(0,33,754,3,"jpeg_gen_optimal_table",i)]--;
  
  /* Return final symbol counts (only for lengths 0..16) */
  MEMCOPY(htbl->bits, bits, SIZEOF(htbl->bits));
  
  /* Return a list of the symbols sorted by code length */
  /* It's not real clear to me why we don't need to consider the codelength
   * changes made above, but the JPEG spec seems to think this works.
   */
  p = 0;
  for (i = 1; i <= MAX_CLEN; i++) {
    for (j = 0; j <= 255; j++) {
      if (codesize[_RV_insert_check(0,257,766,11,"jpeg_gen_optimal_table",j)] == i) {
	htbl->huffval[_RV_insert_check(0,256,767,2,"jpeg_gen_optimal_table",p)] = (UINT8) j;
	p++;
      }
    }
  }

  /* Set sent_table FALSE so updated table will be written to JPEG file. */
  htbl->sent_table = FALSE;
}


/*
 * Finish up a statistics-gathering pass and create the new Huffman tables.
 */

METHODDEF(void)
finish_pass_gather (j_compress_ptr cinfo)
{
  huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  dctbl;
__boundcheck_metadata_store((void *)(&dctbl),(void *)((size_t)(&dctbl)+sizeof(dctbl)*8-1));
int  actbl;
__boundcheck_metadata_store((void *)(&actbl),(void *)((size_t)(&actbl)+sizeof(actbl)*8-1));

  jpeg_component_info * compptr;
__boundcheck_metadata_store((void *)(&compptr),(void *)((size_t)(&compptr)+sizeof(compptr)*8-1));

  JHUFF_TBL **htblptr;
__boundcheck_metadata_store((void *)(&htblptr),(void *)((size_t)(&htblptr)+sizeof(htblptr)*8-1));

  boolean did_dc[NUM_HUFF_TBLS];__boundcheck_metadata_store(&did_dc[0],&did_dc[4-1]);

  boolean did_ac[NUM_HUFF_TBLS];__boundcheck_metadata_store(&did_ac[0],&did_ac[4-1]);


  /* It's important not to apply jpeg_gen_optimal_table more than once
   * per table, because it clobbers the input frequency counts!
   */
  MEMZERO(did_dc, SIZEOF(did_dc));
  MEMZERO(did_ac, SIZEOF(did_ac));

  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[_RV_insert_check(0,4,799,15,"finish_pass_gather",ci)];
    dctbl = compptr->dc_tbl_no;
    actbl = compptr->ac_tbl_no;
    if (! did_dc[_RV_insert_check(0,4,802,11,"finish_pass_gather",dctbl)]) {
      htblptr = & cinfo->dc_huff_tbl_ptrs[_RV_insert_check(0,4,803,19,"finish_pass_gather",dctbl)];
      if (*(JHUFF_TBL **)(__boundcheck_ptr_reference(804,12,"finish_pass_gather",(void *)(htblptr),(void *)(htblptr))) == NULL)
	*(JHUFF_TBL **)(__boundcheck_ptr_reference(805,3,"finish_pass_gather",(void *)(htblptr),(void *)(htblptr))) = jpeg_alloc_huff_table((j_common_ptr) cinfo);
      jpeg_gen_optimal_table(cinfo, *(JHUFF_TBL **)(__boundcheck_ptr_reference(806,38,"finish_pass_gather",(void *)(htblptr),(void *)(htblptr))), entropy->dc_count_ptrs[_RV_insert_check(0,4,806,47,"finish_pass_gather",dctbl)]);
      did_dc[_RV_insert_check(0,4,807,7,"finish_pass_gather",dctbl)] = TRUE;
    }
    if (! did_ac[_RV_insert_check(0,4,809,11,"finish_pass_gather",actbl)]) {
      htblptr = & cinfo->ac_huff_tbl_ptrs[_RV_insert_check(0,4,810,19,"finish_pass_gather",actbl)];
      if (*(JHUFF_TBL **)(__boundcheck_ptr_reference(811,12,"finish_pass_gather",(void *)(htblptr),(void *)(htblptr))) == NULL)
	*(JHUFF_TBL **)(__boundcheck_ptr_reference(812,3,"finish_pass_gather",(void *)(htblptr),(void *)(htblptr))) = jpeg_alloc_huff_table((j_common_ptr) cinfo);
      jpeg_gen_optimal_table(cinfo, *(JHUFF_TBL **)(__boundcheck_ptr_reference(813,38,"finish_pass_gather",(void *)(htblptr),(void *)(htblptr))), entropy->ac_count_ptrs[_RV_insert_check(0,4,813,47,"finish_pass_gather",actbl)]);
      did_ac[_RV_insert_check(0,4,814,7,"finish_pass_gather",actbl)] = TRUE;
    }
  }
}


#endif /* ENTROPY_OPT_SUPPORTED */


/*
 * Module initialization routine for Huffman entropy encoding.
 */

GLOBAL(void)
jinit_huff_encoder (j_compress_ptr cinfo)
{
  huff_entropy_ptr entropy;
__boundcheck_metadata_store((void *)(&entropy),(void *)((size_t)(&entropy)+sizeof(entropy)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  entropy = (huff_entropy_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(834,19,"jinit_huff_encoder",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(huff_entropy_encoder));
  cinfo->entropy = (struct jpeg_entropy_encoder *) entropy;
  entropy->pub.start_pass = start_pass_huff;

  /* Mark tables unallocated */
  for (i = 0; i < NUM_HUFF_TBLS; i++) {
    entropy->dc_derived_tbls[_RV_insert_check(0,4,841,5,"jinit_huff_encoder",i)] = entropy->ac_derived_tbls[_RV_insert_check(0,4,841,35,"jinit_huff_encoder",i)] = NULL;
#ifdef ENTROPY_OPT_SUPPORTED
    entropy->dc_count_ptrs[_RV_insert_check(0,4,843,5,"jinit_huff_encoder",i)] = entropy->ac_count_ptrs[_RV_insert_check(0,4,843,33,"jinit_huff_encoder",i)] = NULL;
#endif
  }
}
