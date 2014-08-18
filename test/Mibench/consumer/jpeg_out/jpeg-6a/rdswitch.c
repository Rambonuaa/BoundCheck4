/*
 * rdswitch.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains routines to process some of cjpeg's more complicated
 * command-line switches.  Switches processed here are:
 *	-qtables file		Read quantization tables from text file
 *	-scans file		Read scan script from text file
 *	-qslots N[,N,...]	Set component quantization table selectors
 *	-sample HxV[,HxV,...]	Set component sampling factors
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include <ctype.h>		/* to declare isdigit(), isspace() */


LOCAL(int)
text_getc (FILE * file)
/* Read next char, skipping over any comments (# to end of line) */
/* A comment/newline sequence is returned as a newline */
{
  register int ch;
  
  ch = getc(file);
  if (ch == '#') {
    do {
      ch = getc(file);
    } while (ch != '\n' && ch != EOF);
  }
  return ch;
}


LOCAL(boolean)
read_text_integer (FILE * file, long * result, int * termchar)
/* Read an unsigned decimal integer from a file, store it in result */
/* Reads one trailing character after the integer; returns it in termchar */
{
  register int ch;
  register long val;
  
  /* Skip any leading whitespace, detect EOF */
  do {
    ch = text_getc(file);
    if (ch == EOF) {
      *(int *)(__boundcheck_ptr_reference(49,8,"read_text_integer",(void *)(termchar),(void *)(termchar))) = ch;
      return FALSE;
    }
  } while (isspace(ch));
  
  if (! isdigit(ch)) {
    *(int *)(__boundcheck_ptr_reference(55,6,"read_text_integer",(void *)(termchar),(void *)(termchar))) = ch;
    return FALSE;
  }

  val = ch - '0';
  while ((ch = text_getc(file)) != EOF) {
    if (! isdigit(ch))
      break;
    val *= 10;
    val += ch - '0';
  }
  *(long *)(__boundcheck_ptr_reference(66,4,"read_text_integer",(void *)(result),(void *)(result))) = val;
  *(int *)(__boundcheck_ptr_reference(67,4,"read_text_integer",(void *)(termchar),(void *)(termchar))) = ch;
  return TRUE;
}


GLOBAL(boolean)
read_quant_tables (j_compress_ptr cinfo, char * filename,
		   int scale_factor, boolean force_baseline)
/* Read a set of quantization tables from the specified file.
 * The file is plain ASCII text: decimal numbers with whitespace between.
 * Comments preceded by '#' may be included in the file.
 * There may be one to NUM_QUANT_TBLS tables in the file, each of 64 values.
 * The tables are implicitly numbered 0,1,etc.
 * NOTE: does not affect the qslots mapping, which will default to selecting
 * table 0 for luminance (or primary) components, 1 for chrominance components.
 * You must use -qslots if you want a different component->table mapping.
 */
{
  FILE * fp;
__boundcheck_metadata_store((void *)(&fp),(void *)((size_t)(&fp)+sizeof(fp)*8-1));

  int tblno;
__boundcheck_metadata_store((void *)(&tblno),(void *)((size_t)(&tblno)+sizeof(tblno)*8-1));
int  i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  termchar;
__boundcheck_metadata_store((void *)(&termchar),(void *)((size_t)(&termchar)+sizeof(termchar)*8-1));

  long val;
__boundcheck_metadata_store((void *)(&val),(void *)((size_t)(&val)+sizeof(val)*8-1));

  unsigned int table[DCTSIZE2];__boundcheck_metadata_store(&table[0],&table[64-1]);


  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Can't open table file %s\n", filename);
    return FALSE;
  }
  tblno = 0;

  while (read_text_integer(fp, &val, &termchar)) { /* read 1st element of table */
    if (tblno >= NUM_QUANT_TBLS) {
      fprintf(stderr, "Too many tables in file %s\n", filename);
      fclose(fp);
      return FALSE;
    }
    table[_RV_insert_check(0,64,102,5,"read_quant_tables",0)] = (unsigned int) val;
    for (i = 1; i < DCTSIZE2; i++) {
      if (! read_text_integer(fp, &val, &termchar)) {
	fprintf(stderr, "Invalid table data in file %s\n", filename);
	fclose(fp);
	return FALSE;
      }
      table[_RV_insert_check(0,64,109,7,"read_quant_tables",i)] = (unsigned int) val;
    }
    jpeg_add_quant_table(cinfo, tblno, table, scale_factor, force_baseline);
    tblno++;
  }

  if (termchar != EOF) {
    fprintf(stderr, "Non-numeric data in file %s\n", filename);
    fclose(fp);
    return FALSE;
  }

  fclose(fp);
  return TRUE;
}


#ifdef C_MULTISCAN_FILES_SUPPORTED

LOCAL(boolean)
read_scan_integer (FILE * file, long * result, int * termchar)
/* Variant of read_text_integer that always looks for a non-space termchar;
 * this simplifies parsing of punctuation in scan scripts.
 */
{
  register int ch;

  if (! read_text_integer(file, result, termchar))
    return FALSE;
  ch = *(int *)(__boundcheck_ptr_reference(138,9,"read_scan_integer",(void *)(termchar),(void *)(termchar)));
  while (ch != EOF && isspace(ch))
    ch = text_getc(file);
  if (isdigit(ch)) {		/* oops, put it back */
    if (ungetc(ch, file) == EOF)
      return FALSE;
    ch = ' ';
  } else {
    /* Any separators other than ';' and ':' are ignored;
     * this allows user to insert commas, etc, if desired.
     */
    if (ch != EOF && ch != ';' && ch != ':')
      ch = ' ';
  }
  *(int *)(__boundcheck_ptr_reference(152,4,"read_scan_integer",(void *)(termchar),(void *)(termchar))) = ch;
  return TRUE;
}


GLOBAL(boolean)
read_scan_script (j_compress_ptr cinfo, char * filename)
/* Read a scan script from the specified text file.
 * Each entry in the file defines one scan to be emitted.
 * Entries are separated by semicolons ';'.
 * An entry contains one to four component indexes,
 * optionally followed by a colon ':' and four progressive-JPEG parameters.
 * The component indexes denote which component(s) are to be transmitted
 * in the current scan.  The first component has index 0.
 * Sequential JPEG is used if the progressive-JPEG parameters are omitted.
 * The file is free format text: any whitespace may appear between numbers
 * and the ':' and ';' punctuation marks.  Also, other punctuation (such
 * as commas or dashes) can be placed between numbers if desired.
 * Comments preceded by '#' may be included in the file.
 * Note: we do very little validity checking here;
 * jcmaster.c will validate the script parameters.
 */
{
  FILE * fp;
__boundcheck_metadata_store((void *)(&fp),(void *)((size_t)(&fp)+sizeof(fp)*8-1));

  int scanno;
__boundcheck_metadata_store((void *)(&scanno),(void *)((size_t)(&scanno)+sizeof(scanno)*8-1));
int  ncomps;
__boundcheck_metadata_store((void *)(&ncomps),(void *)((size_t)(&ncomps)+sizeof(ncomps)*8-1));
int  termchar;
__boundcheck_metadata_store((void *)(&termchar),(void *)((size_t)(&termchar)+sizeof(termchar)*8-1));

  long val;
__boundcheck_metadata_store((void *)(&val),(void *)((size_t)(&val)+sizeof(val)*8-1));

  jpeg_scan_info * scanptr;
__boundcheck_metadata_store((void *)(&scanptr),(void *)((size_t)(&scanptr)+sizeof(scanptr)*8-1));

#define MAX_SCANS  100		/* quite arbitrary limit */
  jpeg_scan_info scans[MAX_SCANS];__boundcheck_metadata_store(&scans[0],&scans[100-1]);


  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Can't open scan definition file %s\n", filename);
    return FALSE;
  }
  scanptr = scans;
__boundcheck_metadata_trans_check((void *)(scanptr),(void *)(scans),(void *)(scans));

  scanno = 0;

  while (read_scan_integer(fp, &val, &termchar)) {
    if (scanno >= MAX_SCANS) {
      fprintf(stderr, "Too many scans defined in file %s\n", filename);
      fclose(fp);
      return FALSE;
    }
    scanptr->component_index[_RV_insert_check(0,4,195,5,"read_scan_script",0)] = (int) val;
    ncomps = 1;
    while (termchar == ' ') {
      if (ncomps >= MAX_COMPS_IN_SCAN) {
	fprintf(stderr, "Too many components in one scan in file %s\n",
		filename);
	fclose(fp);
	return FALSE;
      }
      if (! read_scan_integer(fp, &val, &termchar))
	goto bogus;
      scanptr->component_index[_RV_insert_check(0,4,206,7,"read_scan_script",ncomps)] = (int) val;
      ncomps++;
    }
    scanptr->comps_in_scan = ncomps;
    if (termchar == ':') {
      if (! read_scan_integer(fp, &val, &termchar) || termchar != ' ')
	goto bogus;
      scanptr->Ss = (int) val;
      if (! read_scan_integer(fp, &val, &termchar) || termchar != ' ')
	goto bogus;
      scanptr->Se = (int) val;
      if (! read_scan_integer(fp, &val, &termchar) || termchar != ' ')
	goto bogus;
      scanptr->Ah = (int) val;
      if (! read_scan_integer(fp, &val, &termchar))
	goto bogus;
      scanptr->Al = (int) val;
    } else {
      /* set non-progressive parameters */
      scanptr->Ss = 0;
      scanptr->Se = DCTSIZE2-1;
      scanptr->Ah = 0;
      scanptr->Al = 0;
    }
    if (termchar != ';' && termchar != EOF) {
bogus:
      fprintf(stderr, "Invalid scan entry format in file %s\n", filename);
      fclose(fp);
      return FALSE;
    }
    scanptr++, scanno++;
  }

  if (termchar != EOF) {
    fprintf(stderr, "Non-numeric data in file %s\n", filename);
    fclose(fp);
    return FALSE;
  }

  if (scanno > 0) {
    /* Stash completed scan list in cinfo structure.
     * NOTE: for cjpeg's use, JPOOL_IMAGE is the right lifetime for this data,
     * but if you want to compress multiple images you'd want JPOOL_PERMANENT.
     */
    scanptr = (jpeg_scan_info *)
      (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(251,21,"read_scan_script",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  scanno * SIZEOF(jpeg_scan_info));
    MEMCOPY(scanptr, scans, scanno * SIZEOF(jpeg_scan_info));
    cinfo->scan_info = scanptr;
    cinfo->num_scans = scanno;
  }

  fclose(fp);
  return TRUE;
}

#endif /* C_MULTISCAN_FILES_SUPPORTED */


GLOBAL(boolean)
set_quant_slots (j_compress_ptr cinfo, char *arg)
/* Process a quantization-table-selectors parameter string, of the form
 *     N[,N,...]
 * If there are more components than parameters, the last value is replicated.
 */
{
  int val = 0;
__boundcheck_metadata_store((void *)(&val),(void *)((size_t)(&val)+sizeof(val)*8-1));
			/* default table # */
  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));

  char ch;
__boundcheck_metadata_store((void *)(&ch),(void *)((size_t)(&ch)+sizeof(ch)*8-1));


  for (ci = 0; ci < MAX_COMPONENTS; ci++) {
    if (*(char *)(__boundcheck_ptr_reference(277,10,"set_quant_slots",(void *)(arg),(void *)(arg)))) {
      ch = ',';			/* if not set by sscanf, will be ',' */
      if (sscanf(arg, "%d%c", &val, &ch) < 1)
	return FALSE;
      if (ch != ',')		/* syntax check */
	return FALSE;
      if (val < 0 || val >= NUM_QUANT_TBLS) {
	fprintf(stderr, "JPEG quantization tables are numbered 0..%d\n",
		NUM_QUANT_TBLS-1);
	return FALSE;
      }
      cinfo->comp_info[ci].quant_tbl_no = val;
      while (*(char *)(__boundcheck_ptr_reference(289,15,"set_quant_slots",(void *)(arg),(void *)(arg))) && *(char *)(__boundcheck_ptr_reference(289,26,"set_quant_slots",(void *)(arg),(void *)(arg++))) != ',') /* advance to next segment of arg string */
	;
    } else {
      /* reached end of parameter, set remaining components to last table */
      cinfo->comp_info[ci].quant_tbl_no = val;
    }
  }
  return TRUE;
}


GLOBAL(boolean)
set_sample_factors (j_compress_ptr cinfo, char *arg)
/* Process a sample-factors parameter string, of the form
 *     HxV[,HxV,...]
 * If there are more components than parameters, "1x1" is assumed for the rest.
 */
{
  int ci;
__boundcheck_metadata_store((void *)(&ci),(void *)((size_t)(&ci)+sizeof(ci)*8-1));
int  val1;
__boundcheck_metadata_store((void *)(&val1),(void *)((size_t)(&val1)+sizeof(val1)*8-1));
int  val2;
__boundcheck_metadata_store((void *)(&val2),(void *)((size_t)(&val2)+sizeof(val2)*8-1));

  char ch1;
__boundcheck_metadata_store((void *)(&ch1),(void *)((size_t)(&ch1)+sizeof(ch1)*8-1));
char  ch2;
__boundcheck_metadata_store((void *)(&ch2),(void *)((size_t)(&ch2)+sizeof(ch2)*8-1));


  for (ci = 0; ci < MAX_COMPONENTS; ci++) {
    if (*(char *)(__boundcheck_ptr_reference(311,10,"set_sample_factors",(void *)(arg),(void *)(arg)))) {
      ch2 = ',';		/* if not set by sscanf, will be ',' */
      if (sscanf(arg, "%d%c%d%c", &val1, &ch1, &val2, &ch2) < 3)
	return FALSE;
      if ((ch1 != 'x' && ch1 != 'X') || ch2 != ',') /* syntax check */
	return FALSE;
      if (val1 <= 0 || val1 > 4 || val2 <= 0 || val2 > 4) {
	fprintf(stderr, "JPEG sampling factors must be 1..4\n");
	return FALSE;
      }
      cinfo->comp_info[ci].h_samp_factor = val1;
      cinfo->comp_info[ci].v_samp_factor = val2;
      while (*(char *)(__boundcheck_ptr_reference(323,15,"set_sample_factors",(void *)(arg),(void *)(arg))) && *(char *)(__boundcheck_ptr_reference(323,26,"set_sample_factors",(void *)(arg),(void *)(arg++))) != ',') /* advance to next segment of arg string */
	;
    } else {
      /* reached end of parameter, set remaining components to 1x1 sampling */
      cinfo->comp_info[ci].h_samp_factor = 1;
      cinfo->comp_info[ci].v_samp_factor = 1;
    }
  }
  return TRUE;
}
