/*
 * jcomapi.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains application interface routines that are used for both
 * compression and decompression.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/*
 * Abort processing of a JPEG compression or decompression operation,
 * but don't destroy the object itself.
 *
 * For this, we merely clean up all the nonpermanent memory pools.
 * Note that temp files (virtual arrays) are not allowed to belong to
 * the permanent pool, so we will be able to close all temp files here.
 * Closing a data source or destination, if necessary, is the application's
 * responsibility.
 */

GLOBAL(void)
jpeg_abort (j_common_ptr cinfo)
{
  int pool;
__boundcheck_metadata_store((void *)(&pool),(void *)((size_t)(&pool)+sizeof(pool)*8-1));


  /* Releasing pools in reverse order might help avoid fragmentation
   * with some (brain-damaged) malloc libraries.
   */
  for (pool = JPOOL_NUMPOOLS-1; pool > JPOOL_PERMANENT; pool--) {
    (*(void (*)(j_common_ptr, int))(__boundcheck_ptr_reference(37,19,"jpeg_abort",(void *)(cinfo->mem->free_pool),(void *)cinfo->mem->free_pool))) (cinfo, pool);
  }

  /* Reset overall state for possible reuse of object */
  cinfo->global_state = (cinfo->is_decompressor ? DSTATE_START : CSTATE_START);
}


/*
 * Destruction of a JPEG object.
 *
 * Everything gets deallocated except the master jpeg_compress_struct itself
 * and the error manager struct.  Both of these are supplied by the application
 * and must be freed, if necessary, by the application.  (Often they are on
 * the stack and so don't need to be freed anyway.)
 * Closing a data source or destination, if necessary, is the application's
 * responsibility.
 */

GLOBAL(void)
jpeg_destroy (j_common_ptr cinfo)
{
  /* We need only tell the memory manager to release everything. */
  /* NB: mem pointer is NULL if memory mgr failed to initialize. */
  if (cinfo->mem != NULL)
    (*(void (*)(j_common_ptr))(__boundcheck_ptr_reference(62,19,"jpeg_destroy",(void *)(cinfo->mem->self_destruct),(void *)cinfo->mem->self_destruct))) (cinfo);
  cinfo->mem = NULL;		/* be safe if jpeg_destroy is called twice */
  cinfo->global_state = 0;	/* mark it destroyed */
}


/*
 * Convenience routines for allocating quantization and Huffman tables.
 * (Would jutils.c be a more reasonable place to put these?)
 */

GLOBAL(JQUANT_TBL *)
jpeg_alloc_quant_table (j_common_ptr cinfo)
{
  JQUANT_TBL *tbl;
__boundcheck_metadata_store((void *)(&tbl),(void *)((size_t)(&tbl)+sizeof(tbl)*8-1));


  tbl = (JQUANT_TBL *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(79,19,"jpeg_alloc_quant_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) (cinfo, JPOOL_PERMANENT, SIZEOF(JQUANT_TBL));
  tbl->sent_table = FALSE;	/* make sure this is false in any new table */
  return tbl;
}


GLOBAL(JHUFF_TBL *)
jpeg_alloc_huff_table (j_common_ptr cinfo)
{
  JHUFF_TBL *tbl;
__boundcheck_metadata_store((void *)(&tbl),(void *)((size_t)(&tbl)+sizeof(tbl)*8-1));


  tbl = (JHUFF_TBL *)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(91,19,"jpeg_alloc_huff_table",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) (cinfo, JPOOL_PERMANENT, SIZEOF(JHUFF_TBL));
  tbl->sent_table = FALSE;	/* make sure this is false in any new table */
  return tbl;
}