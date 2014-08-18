/*
 * jquant2.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains 2-pass color quantization (color mapping) routines.
 * These routines provide selection of a custom color map for an image,
 * followed by mapping of the image to that color map, with optional
 * Floyd-Steinberg dithering.
 * It is also possible to use just the second pass to map to an arbitrary
 * externally-given color map.
 *
 * Note: ordered dithering is not supported, since there isn't any fast
 * way to compute intercolor distances; it's unclear that ordered dither's
 * fundamental assumptions even hold with an irregularly spaced color map.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"

#ifdef QUANT_2PASS_SUPPORTED


/*
 * This module implements the well-known Heckbert paradigm for color
 * quantization.  Most of the ideas used here can be traced back to
 * Heckbert's seminal paper
 *   Heckbert, Paul.  "Color Image Quantization for Frame Buffer Display",
 *   Proc. SIGGRAPH '82, Computer Graphics v.16 #3 (July 1982), pp 297-304.
 *
 * In the first pass over the image, we accumulate a histogram showing the
 * usage count of each possible color.  To keep the histogram to a reasonable
 * size, we reduce the precision of the input; typical practice is to retain
 * 5 or 6 bits per color, so that 8 or 4 different input values are counted
 * in the same histogram cell.
 *
 * Next, the color-selection step begins with a box representing the whole
 * color space, and repeatedly splits the "largest" remaining box until we
 * have as many boxes as desired colors.  Then the mean color in each
 * remaining box becomes one of the possible output colors.
 * 
 * The second pass over the image maps each input pixel to the closest output
 * color (optionally after applying a Floyd-Steinberg dithering correction).
 * This mapping is logically trivial, but making it go fast enough requires
 * considerable care.
 *
 * Heckbert-style quantizers vary a good deal in their policies for choosing
 * the "largest" box and deciding where to cut it.  The particular policies
 * used here have proved out well in experimental comparisons, but better ones
 * may yet be found.
 *
 * In earlier versions of the IJG code, this module quantized in YCbCr color
 * space, processing the raw upsampled data without a color conversion step.
 * This allowed the color conversion math to be done only once per colormap
 * entry, not once per pixel.  However, that optimization precluded other
 * useful optimizations (such as merging color conversion with upsampling)
 * and it also interfered with desired capabilities such as quantizing to an
 * externally-supplied colormap.  We have therefore abandoned that approach.
 * The present code works in the post-conversion color space, typically RGB.
 *
 * To improve the visual quality of the results, we actually work in scaled
 * RGB space, giving G distances more weight than R, and R in turn more than
 * B.  To do everything in integer math, we must use integer scale factors.
 * The 2/3/1 scale factors used here correspond loosely to the relative
 * weights of the colors in the NTSC grayscale equation.
 * If you want to use this code to quantize a non-RGB color space, you'll
 * probably need to change these scale factors.
 */

#define R_SCALE 2		/* scale R distances by this much */
#define G_SCALE 3		/* scale G distances by this much */
#define B_SCALE 1		/* and B by this much */

/* Relabel R/G/B as components 0/1/2, respecting the RGB ordering defined
 * in jmorecfg.h.  As the code stands, it will do the right thing for R,G,B
 * and B,G,R orders.  If you define some other weird order in jmorecfg.h,
 * you'll get compile errors until you extend this logic.  In that case
 * you'll probably want to tweak the histogram sizes too.
 */

#if RGB_RED == 0
#define C0_SCALE R_SCALE
#endif
#if RGB_BLUE == 0
#define C0_SCALE B_SCALE
#endif
#if RGB_GREEN == 1
#define C1_SCALE G_SCALE
#endif
#if RGB_RED == 2
#define C2_SCALE R_SCALE
#endif
#if RGB_BLUE == 2
#define C2_SCALE B_SCALE
#endif


/*
 * First we have the histogram data structure and routines for creating it.
 *
 * The number of bits of precision can be adjusted by changing these symbols.
 * We recommend keeping 6 bits for G and 5 each for R and B.
 * If you have plenty of memory and cycles, 6 bits all around gives marginally
 * better results; if you are short of memory, 5 bits all around will save
 * some space but degrade the results.
 * To maintain a fully accurate histogram, we'd need to allocate a "long"
 * (preferably unsigned long) for each cell.  In practice this is overkill;
 * we can get by with 16 bits per cell.  Few of the cell counts will overflow,
 * and clamping those that do overflow to the maximum value will give close-
 * enough results.  This reduces the recommended histogram size from 256Kb
 * to 128Kb, which is a useful savings on PC-class machines.
 * (In the second pass the histogram space is re-used for pixel mapping data;
 * in that capacity, each cell must be able to store zero to the number of
 * desired colors.  16 bits/cell is plenty for that too.)
 * Since the JPEG code is intended to run in small memory model on 80x86
 * machines, we can't just allocate the histogram in one chunk.  Instead
 * of a true 3-D array, we use a row of pointers to 2-D arrays.  Each
 * pointer corresponds to a C0 value (typically 2^5 = 32 pointers) and
 * each 2-D array has 2^6*2^5 = 2048 or 2^6*2^6 = 4096 entries.  Note that
 * on 80x86 machines, the pointer row is in near memory but the actual
 * arrays are in far memory (same arrangement as we use for image arrays).
 */

#define MAXNUMCOLORS  (MAXJSAMPLE+1) /* maximum size of colormap */

/* These will do the right thing for either R,G,B or B,G,R color order,
 * but you may not like the results for other color orders.
 */
#define HIST_C0_BITS  5		/* bits of precision in R/B histogram */
#define HIST_C1_BITS  6		/* bits of precision in G histogram */
#define HIST_C2_BITS  5		/* bits of precision in B/R histogram */

/* Number of elements along histogram axes. */
#define HIST_C0_ELEMS  (1<<HIST_C0_BITS)
#define HIST_C1_ELEMS  (1<<HIST_C1_BITS)
#define HIST_C2_ELEMS  (1<<HIST_C2_BITS)

/* These are the amounts to shift an input value to get a histogram index. */
#define C0_SHIFT  (BITS_IN_JSAMPLE-HIST_C0_BITS)
#define C1_SHIFT  (BITS_IN_JSAMPLE-HIST_C1_BITS)
#define C2_SHIFT  (BITS_IN_JSAMPLE-HIST_C2_BITS)


typedef UINT16 histcell;	/* histogram cell; prefer an unsigned type */

typedef histcell FAR * histptr;	/* for pointers to histogram cells */

typedef histcell hist1d[HIST_C2_ELEMS]; /* typedefs for the array */
typedef hist1d FAR * hist2d;	/* type for the 2nd-level pointers */
typedef hist2d * hist3d;	/* type for top-level pointer */


/* Declarations for Floyd-Steinberg dithering.
 *
 * Errors are accumulated into the array fserrors[], at a resolution of
 * 1/16th of a pixel count.  The error at a given pixel is propagated
 * to its not-yet-processed neighbors using the standard F-S fractions,
 *		...	(here)	7/16
 *		3/16	5/16	1/16
 * We work left-to-right on even rows, right-to-left on odd rows.
 *
 * We can get away with a single array (holding one row's worth of errors)
 * by using it to store the current row's errors at pixel columns not yet
 * processed, but the next row's errors at columns already processed.  We
 * need only a few extra variables to hold the errors immediately around the
 * current column.  (If we are lucky, those variables are in registers, but
 * even if not, they're probably cheaper to access than array elements are.)
 *
 * The fserrors[] array has (#columns + 2) entries; the extra entry at
 * each end saves us from special-casing the first and last pixels.
 * Each entry is three values long, one value for each color component.
 *
 * Note: on a wide image, we might not have enough room in a PC's near data
 * segment to hold the error array; so it is allocated with alloc_large.
 */

#if BITS_IN_JSAMPLE == 8
typedef INT16 FSERROR;		/* 16 bits should be enough */
typedef int LOCFSERROR;		/* use 'int' for calculation temps */
#else
typedef INT32 FSERROR;		/* may need more than 16 bits */
typedef INT32 LOCFSERROR;	/* be sure calculation temps are big enough */
#endif

typedef FSERROR FAR *FSERRPTR;	/* pointer to error array (in FAR storage!) */


/* Private subobject */

typedef struct {
  struct jpeg_color_quantizer pub; /* public fields */

  /* Space for the eventually created colormap is stashed here */
  JSAMPARRAY sv_colormap;	/* colormap allocated at init time */
  int desired;			/* desired # of colors = size of colormap */

  /* Variables for accumulating image statistics */
  hist3d histogram;		/* pointer to the histogram */

  boolean needs_zeroed;		/* TRUE if next pass must zero histogram */

  /* Variables for Floyd-Steinberg dithering */
  FSERRPTR fserrors;		/* accumulated errors */
  boolean on_odd_row;		/* flag to remember which row we are on */
  int * error_limiter;		/* table for clamping the applied error */
} my_cquantizer;

typedef my_cquantizer * my_cquantize_ptr;


/*
 * Prescan some rows of pixels.
 * In this module the prescan simply updates the histogram, which has been
 * initialized to zeroes by start_pass.
 * An output_buf parameter is required by the method signature, but no data
 * is actually output (in fact the buffer controller is probably passing a
 * NULL pointer).
 */

METHODDEF(void)
prescan_quantize (j_decompress_ptr cinfo, JSAMPARRAY input_buf,
		  JSAMPARRAY output_buf, int num_rows)
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  register JSAMPROW ptr;
  register histptr histp;
  register hist3d histogram = cquantize->histogram;
  int row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));

  JDIMENSION col;
__boundcheck_metadata_store((void *)(&col),(void *)((size_t)(&col)+sizeof(col)*8-1));

  JDIMENSION width = cinfo->output_width;
__boundcheck_metadata_store((void *)(&width),(void *)((size_t)(&width)+sizeof(width)*8-1));


  for (row = 0; row < num_rows; row++) {
    ptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(236,11,"prescan_quantize",(void *)(&input_buf[0]),(void *)(&input_buf[row]))));
    for (col = width; col > 0; col--) {
      /* get pixel value and index into the histogram */
      histp = & (*(histcell *)(__boundcheck_ptr_reference(239,17,"prescan_quantize",(void *)(&histogram[((int)(ptr[0])) >> (8 - 5)][((int)(ptr[1])) >> (8 - 6)][0]),(void *)(&histogram[((int)(ptr[0])) >> (8 - 5)][((int)(ptr[1])) >> (8 - 6)][((int)(ptr[2])) >> (8 - 5)]))));
      /* increment, check for overflow and undo increment if so. */
      if (++(*(histptr)(__boundcheck_ptr_reference(243,15,"prescan_quantize",(void *)(histp),(void *)(histp)))) <= 0)
	(*(histptr)(__boundcheck_ptr_reference(244,4,"prescan_quantize",(void *)(histp),(void *)(histp))))--;
      ptr += 3;
    }
  }
}


/*
 * Next we have the really interesting routines: selection of a colormap
 * given the completed histogram.
 * These routines work with a list of "boxes", each representing a rectangular
 * subset of the input color space (to histogram precision).
 */

typedef struct {
  /* The bounds of the box (inclusive); expressed as histogram indexes */
  int c0min, c0max;
  int c1min, c1max;
  int c2min, c2max;
  /* The volume (actually 2-norm) of the box */
  INT32 volume;
  /* The number of nonzero histogram cells within this box */
  long colorcount;
} box;

typedef box * boxptr;


LOCAL(boxptr)
find_biggest_color_pop (boxptr boxlist, int numboxes)
/* Find the splittable box with the largest color population */
/* Returns NULL if no splittable boxes remain */
{
  register boxptr boxp;
  register int i;
  register long maxc = 0;
  boxptr which = NULL;
__boundcheck_metadata_store((void *)(&which),(void *)((size_t)(&which)+sizeof(which)*8-1));

  
  for (i = 0, boxp = boxlist; i < numboxes; i++, boxp++) {
    if (boxp->colorcount > maxc && boxp->volume > 0) {
      which = boxp;
__boundcheck_metadata_trans_check((void *)(which),(void *)(boxp),(void *)(boxp));

      maxc = boxp->colorcount;
    }
  }
  return which;
}


LOCAL(boxptr)
find_biggest_volume (boxptr boxlist, int numboxes)
/* Find the splittable box with the largest (scaled) volume */
/* Returns NULL if no splittable boxes remain */
{
  register boxptr boxp;
  register int i;
  register INT32 maxv = 0;
  boxptr which = NULL;
__boundcheck_metadata_store((void *)(&which),(void *)((size_t)(&which)+sizeof(which)*8-1));

  
  for (i = 0, boxp = boxlist; i < numboxes; i++, boxp++) {
    if (boxp->volume > maxv) {
      which = boxp;
__boundcheck_metadata_trans_check((void *)(which),(void *)(boxp),(void *)(boxp));

      maxv = boxp->volume;
    }
  }
  return which;
}


LOCAL(void)
update_box (j_decompress_ptr cinfo, boxptr boxp)
/* Shrink the min/max bounds of a box to enclose only nonzero elements, */
/* and recompute its volume and population */
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  hist3d histogram = cquantize->histogram;
__boundcheck_metadata_store((void *)(&histogram),(void *)((size_t)(&histogram)+sizeof(histogram)*8-1));

  histptr histp;
__boundcheck_metadata_store((void *)(&histp),(void *)((size_t)(&histp)+sizeof(histp)*8-1));

  int c0;
__boundcheck_metadata_store((void *)(&c0),(void *)((size_t)(&c0)+sizeof(c0)*8-1));
int c1;
__boundcheck_metadata_store((void *)(&c1),(void *)((size_t)(&c1)+sizeof(c1)*8-1));
int c2;
__boundcheck_metadata_store((void *)(&c2),(void *)((size_t)(&c2)+sizeof(c2)*8-1));

  int c0min;
__boundcheck_metadata_store((void *)(&c0min),(void *)((size_t)(&c0min)+sizeof(c0min)*8-1));
int c0max;
__boundcheck_metadata_store((void *)(&c0max),(void *)((size_t)(&c0max)+sizeof(c0max)*8-1));
int c1min;
__boundcheck_metadata_store((void *)(&c1min),(void *)((size_t)(&c1min)+sizeof(c1min)*8-1));
int c1max;
__boundcheck_metadata_store((void *)(&c1max),(void *)((size_t)(&c1max)+sizeof(c1max)*8-1));
int c2min;
__boundcheck_metadata_store((void *)(&c2min),(void *)((size_t)(&c2min)+sizeof(c2min)*8-1));
int c2max;
__boundcheck_metadata_store((void *)(&c2max),(void *)((size_t)(&c2max)+sizeof(c2max)*8-1));

  INT32 dist0;
__boundcheck_metadata_store((void *)(&dist0),(void *)((size_t)(&dist0)+sizeof(dist0)*8-1));
INT32 dist1;
__boundcheck_metadata_store((void *)(&dist1),(void *)((size_t)(&dist1)+sizeof(dist1)*8-1));
INT32 dist2;
__boundcheck_metadata_store((void *)(&dist2),(void *)((size_t)(&dist2)+sizeof(dist2)*8-1));

  long ccount;
__boundcheck_metadata_store((void *)(&ccount),(void *)((size_t)(&ccount)+sizeof(ccount)*8-1));

  
  c0min = boxp->c0min;  c0max = boxp->c0max;
  c1min = boxp->c1min;  c1max = boxp->c1max;
  c2min = boxp->c2min;  c2max = boxp->c2max;
  
  if (c0max > c0min)
    for (c0 = c0min; c0 <= c0max; c0++)
      for (c1 = c1min; c1 <= c1max; c1++) {
	histp = & (*(histcell *)(__boundcheck_ptr_reference(332,12,"update_box",(void *)(&histogram[c0][c1][0]),(void *)(&histogram[c0][c1][c2min]))));
	for (c2 = c2min; c2 <= c2max; c2++)
	  if (*(histptr)(__boundcheck_ptr_reference(334,14,"update_box",(void *)(histp),(void *)(histp++))) != 0) {
	    boxp->c0min = c0min = c0;
	    goto have_c0min;
	  }
      }
 have_c0min:
  if (c0max > c0min)
    for (c0 = c0max; c0 >= c0min; c0--)
      for (c1 = c1min; c1 <= c1max; c1++) {
	histp = & (*(histcell *)(__boundcheck_ptr_reference(343,12,"update_box",(void *)(&histogram[c0][c1][0]),(void *)(&histogram[c0][c1][c2min]))));
	for (c2 = c2min; c2 <= c2max; c2++)
	  if (*(histptr)(__boundcheck_ptr_reference(345,14,"update_box",(void *)(histp),(void *)(histp++))) != 0) {
	    boxp->c0max = c0max = c0;
	    goto have_c0max;
	  }
      }
 have_c0max:
  if (c1max > c1min)
    for (c1 = c1min; c1 <= c1max; c1++)
      for (c0 = c0min; c0 <= c0max; c0++) {
	histp = & (*(histcell *)(__boundcheck_ptr_reference(354,12,"update_box",(void *)(&histogram[c0][c1][0]),(void *)(&histogram[c0][c1][c2min]))));
	for (c2 = c2min; c2 <= c2max; c2++)
	  if (*(histptr)(__boundcheck_ptr_reference(356,14,"update_box",(void *)(histp),(void *)(histp++))) != 0) {
	    boxp->c1min = c1min = c1;
	    goto have_c1min;
	  }
      }
 have_c1min:
  if (c1max > c1min)
    for (c1 = c1max; c1 >= c1min; c1--)
      for (c0 = c0min; c0 <= c0max; c0++) {
	histp = & (*(histcell *)(__boundcheck_ptr_reference(365,12,"update_box",(void *)(&histogram[c0][c1][0]),(void *)(&histogram[c0][c1][c2min]))));
	for (c2 = c2min; c2 <= c2max; c2++)
	  if (*(histptr)(__boundcheck_ptr_reference(367,14,"update_box",(void *)(histp),(void *)(histp++))) != 0) {
	    boxp->c1max = c1max = c1;
	    goto have_c1max;
	  }
      }
 have_c1max:
  if (c2max > c2min)
    for (c2 = c2min; c2 <= c2max; c2++)
      for (c0 = c0min; c0 <= c0max; c0++) {
	histp = & (*(histcell *)(__boundcheck_ptr_reference(376,12,"update_box",(void *)(&histogram[c0][c1min][0]),(void *)(&histogram[c0][c1min][c2]))));
	for (c1 = c1min; c1 <= c1max; c1++, histp += HIST_C2_ELEMS)
	  if (*(histptr)(__boundcheck_ptr_reference(378,9,"update_box",(void *)(histp),(void *)(histp))) != 0) {
	    boxp->c2min = c2min = c2;
	    goto have_c2min;
	  }
      }
 have_c2min:
  if (c2max > c2min)
    for (c2 = c2max; c2 >= c2min; c2--)
      for (c0 = c0min; c0 <= c0max; c0++) {
	histp = & (*(histcell *)(__boundcheck_ptr_reference(387,12,"update_box",(void *)(&histogram[c0][c1min][0]),(void *)(&histogram[c0][c1min][c2]))));
	for (c1 = c1min; c1 <= c1max; c1++, histp += HIST_C2_ELEMS)
	  if (*(histptr)(__boundcheck_ptr_reference(389,9,"update_box",(void *)(histp),(void *)(histp))) != 0) {
	    boxp->c2max = c2max = c2;
	    goto have_c2max;
	  }
      }
 have_c2max:

  /* Update box volume.
   * We use 2-norm rather than real volume here; this biases the method
   * against making long narrow boxes, and it has the side benefit that
   * a box is splittable iff norm > 0.
   * Since the differences are expressed in histogram-cell units,
   * we have to shift back to JSAMPLE units to get consistent distances;
   * after which, we scale according to the selected distance scale factors.
   */
  dist0 = ((c0max - c0min) << C0_SHIFT) * C0_SCALE;
  dist1 = ((c1max - c1min) << C1_SHIFT) * C1_SCALE;
  dist2 = ((c2max - c2min) << C2_SHIFT) * C2_SCALE;
  boxp->volume = dist0*dist0 + dist1*dist1 + dist2*dist2;
  
  /* Now scan remaining volume of box and compute population */
  ccount = 0;
  for (c0 = c0min; c0 <= c0max; c0++)
    for (c1 = c1min; c1 <= c1max; c1++) {
      histp = & (*(histcell *)(__boundcheck_ptr_reference(413,17,"update_box",(void *)(&histogram[c0][c1][0]),(void *)(&histogram[c0][c1][c2min]))));
      for (c2 = c2min; c2 <= c2max; c2++, histp++)
	if (*(histptr)(__boundcheck_ptr_reference(415,7,"update_box",(void *)(histp),(void *)(histp))) != 0) {
	  ccount++;
	}
    }
  boxp->colorcount = ccount;
}


LOCAL(int)
median_cut (j_decompress_ptr cinfo, boxptr boxlist, int numboxes,
	    int desired_colors)
/* Repeatedly select and split the largest box until we have enough boxes */
{
  int n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));
int lb;
__boundcheck_metadata_store((void *)(&lb),(void *)((size_t)(&lb)+sizeof(lb)*8-1));

  int c0;
__boundcheck_metadata_store((void *)(&c0),(void *)((size_t)(&c0)+sizeof(c0)*8-1));
int c1;
__boundcheck_metadata_store((void *)(&c1),(void *)((size_t)(&c1)+sizeof(c1)*8-1));
int c2;
__boundcheck_metadata_store((void *)(&c2),(void *)((size_t)(&c2)+sizeof(c2)*8-1));
int cmax;
__boundcheck_metadata_store((void *)(&cmax),(void *)((size_t)(&cmax)+sizeof(cmax)*8-1));

  register boxptr b1,b2;

  while (numboxes < desired_colors) {
    /* Select box to split.
     * Current algorithm: by population for first half, then by volume.
     */
    if (numboxes*2 <= desired_colors) {
      b1 = find_biggest_color_pop(boxlist, numboxes);
    } else {
      b1 = find_biggest_volume(boxlist, numboxes);
    }
    if (b1 == NULL)		/* no splittable boxes left! */
      break;
    b2 = &(*(box *)(__boundcheck_ptr_reference(443,11,"median_cut",(void *)(&boxlist[0]),(void *)(&boxlist[numboxes]))));	/* where new box will go */
    /* Copy the color bounds to the new box. */
    b2->c0max = b1->c0max; b2->c1max = b1->c1max; b2->c2max = b1->c2max;
    b2->c0min = b1->c0min; b2->c1min = b1->c1min; b2->c2min = b1->c2min;
    /* Choose which axis to split the box on.
     * Current algorithm: longest scaled axis.
     * See notes in update_box about scaling distances.
     */
    c0 = ((b1->c0max - b1->c0min) << C0_SHIFT) * C0_SCALE;
    c1 = ((b1->c1max - b1->c1min) << C1_SHIFT) * C1_SCALE;
    c2 = ((b1->c2max - b1->c2min) << C2_SHIFT) * C2_SCALE;
    /* We want to break any ties in favor of green, then red, blue last.
     * This code does the right thing for R,G,B or B,G,R color orders only.
     */
#if RGB_RED == 0
    cmax = c1; n = 1;
    if (c0 > cmax) { cmax = c0; n = 0; }
    if (c2 > cmax) { n = 2; }
#else
    cmax = c1; n = 1;
    if (c2 > cmax) { cmax = c2; n = 2; }
    if (c0 > cmax) { n = 0; }
#endif
    /* Choose split point along selected axis, and update box bounds.
     * Current algorithm: split at halfway point.
     * (Since the box has been shrunk to minimum volume,
     * any split will produce two nonempty subboxes.)
     * Note that lb value is max for lower box, so must be < old max.
     */
    switch (n) {
    case 0:
      lb = (b1->c0max + b1->c0min) / 2;
      b1->c0max = lb;
      b2->c0min = lb+1;
      break;
    case 1:
      lb = (b1->c1max + b1->c1min) / 2;
      b1->c1max = lb;
      b2->c1min = lb+1;
      break;
    case 2:
      lb = (b1->c2max + b1->c2min) / 2;
      b1->c2max = lb;
      b2->c2min = lb+1;
      break;
    }
    /* Update stats for boxes */
    update_box(cinfo, b1);
    update_box(cinfo, b2);
    numboxes++;
  }
  return numboxes;
}


LOCAL(void)
compute_color (j_decompress_ptr cinfo, boxptr boxp, int icolor)
/* Compute representative color for a box, put it in colormap[icolor] */
{
  /* Current algorithm: mean weighted by pixels (not colors) */
  /* Note it is important to get the rounding correct! */
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  hist3d histogram = cquantize->histogram;
__boundcheck_metadata_store((void *)(&histogram),(void *)((size_t)(&histogram)+sizeof(histogram)*8-1));

  histptr histp;
__boundcheck_metadata_store((void *)(&histp),(void *)((size_t)(&histp)+sizeof(histp)*8-1));

  int c0;
__boundcheck_metadata_store((void *)(&c0),(void *)((size_t)(&c0)+sizeof(c0)*8-1));
int c1;
__boundcheck_metadata_store((void *)(&c1),(void *)((size_t)(&c1)+sizeof(c1)*8-1));
int c2;
__boundcheck_metadata_store((void *)(&c2),(void *)((size_t)(&c2)+sizeof(c2)*8-1));

  int c0min;
__boundcheck_metadata_store((void *)(&c0min),(void *)((size_t)(&c0min)+sizeof(c0min)*8-1));
int c0max;
__boundcheck_metadata_store((void *)(&c0max),(void *)((size_t)(&c0max)+sizeof(c0max)*8-1));
int c1min;
__boundcheck_metadata_store((void *)(&c1min),(void *)((size_t)(&c1min)+sizeof(c1min)*8-1));
int c1max;
__boundcheck_metadata_store((void *)(&c1max),(void *)((size_t)(&c1max)+sizeof(c1max)*8-1));
int c2min;
__boundcheck_metadata_store((void *)(&c2min),(void *)((size_t)(&c2min)+sizeof(c2min)*8-1));
int c2max;
__boundcheck_metadata_store((void *)(&c2max),(void *)((size_t)(&c2max)+sizeof(c2max)*8-1));

  long count;
__boundcheck_metadata_store((void *)(&count),(void *)((size_t)(&count)+sizeof(count)*8-1));

  long total = 0;
__boundcheck_metadata_store((void *)(&total),(void *)((size_t)(&total)+sizeof(total)*8-1));

  long c0total = 0;
__boundcheck_metadata_store((void *)(&c0total),(void *)((size_t)(&c0total)+sizeof(c0total)*8-1));

  long c1total = 0;
__boundcheck_metadata_store((void *)(&c1total),(void *)((size_t)(&c1total)+sizeof(c1total)*8-1));

  long c2total = 0;
__boundcheck_metadata_store((void *)(&c2total),(void *)((size_t)(&c2total)+sizeof(c2total)*8-1));

  
  c0min = boxp->c0min;  c0max = boxp->c0max;
  c1min = boxp->c1min;  c1max = boxp->c1max;
  c2min = boxp->c2min;  c2max = boxp->c2max;
  
  for (c0 = c0min; c0 <= c0max; c0++)
    for (c1 = c1min; c1 <= c1max; c1++) {
      histp = & (*(histcell *)(__boundcheck_ptr_reference(521,17,"compute_color",(void *)(&histogram[c0][c1][0]),(void *)(&histogram[c0][c1][c2min]))));
      for (c2 = c2min; c2 <= c2max; c2++) {
	if ((count = *(histptr)(__boundcheck_ptr_reference(523,21,"compute_color",(void *)(histp),(void *)(histp++)))) != 0) {
	  total += count;
	  c0total += ((c0 << C0_SHIFT) + ((1<<C0_SHIFT)>>1)) * count;
	  c1total += ((c1 << C1_SHIFT) + ((1<<C1_SHIFT)>>1)) * count;
	  c2total += ((c2 << C2_SHIFT) + ((1<<C2_SHIFT)>>1)) * count;
	}
      }
    }
  
  cinfo->colormap[0][icolor] = (JSAMPLE) ((c0total + (total>>1)) / total);
  cinfo->colormap[1][icolor] = (JSAMPLE) ((c1total + (total>>1)) / total);
  cinfo->colormap[2][icolor] = (JSAMPLE) ((c2total + (total>>1)) / total);
}


LOCAL(void)
select_colors (j_decompress_ptr cinfo, int desired_colors)
/* Master routine for color selection */
{
  boxptr boxlist;
__boundcheck_metadata_store((void *)(&boxlist),(void *)((size_t)(&boxlist)+sizeof(boxlist)*8-1));

  int numboxes;
__boundcheck_metadata_store((void *)(&numboxes),(void *)((size_t)(&numboxes)+sizeof(numboxes)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  /* Allocate workspace for box list */
  boxlist = (boxptr) (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(547,36,"select_colors",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE, desired_colors * SIZEOF(box));
  /* Initialize one box containing whole space */
  numboxes = 1;
  (*(box *)(__boundcheck_ptr_reference(551,3,"select_colors",(void *)(&boxlist[0]),(void *)(&boxlist[0])))).c0min = 0;
  (*(box *)(__boundcheck_ptr_reference(552,3,"select_colors",(void *)(&boxlist[0]),(void *)(&boxlist[0])))).c0max = MAXJSAMPLE >> C0_SHIFT;
  (*(box *)(__boundcheck_ptr_reference(553,3,"select_colors",(void *)(&boxlist[0]),(void *)(&boxlist[0])))).c1min = 0;
  (*(box *)(__boundcheck_ptr_reference(554,3,"select_colors",(void *)(&boxlist[0]),(void *)(&boxlist[0])))).c1max = MAXJSAMPLE >> C1_SHIFT;
  (*(box *)(__boundcheck_ptr_reference(555,3,"select_colors",(void *)(&boxlist[0]),(void *)(&boxlist[0])))).c2min = 0;
  (*(box *)(__boundcheck_ptr_reference(556,3,"select_colors",(void *)(&boxlist[0]),(void *)(&boxlist[0])))).c2max = MAXJSAMPLE >> C2_SHIFT;
  /* Shrink it to actually-used volume and set its statistics */
  update_box(cinfo, & (*(box *)(__boundcheck_ptr_reference(558,23,"select_colors",(void *)(&boxlist[0]),(void *)(&boxlist[0])))));
  /* Perform median-cut to produce final box list */
  numboxes = median_cut(cinfo, boxlist, numboxes, desired_colors);
  /* Compute the representative color for each box, fill colormap */
  for (i = 0; i < numboxes; i++)
    compute_color(cinfo, & (*(box *)(__boundcheck_ptr_reference(563,28,"select_colors",(void *)(&boxlist[0]),(void *)(&boxlist[i])))), i);
  cinfo->actual_number_of_colors = numboxes;
  TRACEMS1(cinfo, 1, JTRC_QUANT_SELECTED, numboxes);
}


/*
 * These routines are concerned with the time-critical task of mapping input
 * colors to the nearest color in the selected colormap.
 *
 * We re-use the histogram space as an "inverse color map", essentially a
 * cache for the results of nearest-color searches.  All colors within a
 * histogram cell will be mapped to the same colormap entry, namely the one
 * closest to the cell's center.  This may not be quite the closest entry to
 * the actual input color, but it's almost as good.  A zero in the cache
 * indicates we haven't found the nearest color for that cell yet; the array
 * is cleared to zeroes before starting the mapping pass.  When we find the
 * nearest color for a cell, its colormap index plus one is recorded in the
 * cache for future use.  The pass2 scanning routines call fill_inverse_cmap
 * when they need to use an unfilled entry in the cache.
 *
 * Our method of efficiently finding nearest colors is based on the "locally
 * sorted search" idea described by Heckbert and on the incremental distance
 * calculation described by Spencer W. Thomas in chapter III.1 of Graphics
 * Gems II (James Arvo, ed.  Academic Press, 1991).  Thomas points out that
 * the distances from a given colormap entry to each cell of the histogram can
 * be computed quickly using an incremental method: the differences between
 * distances to adjacent cells themselves differ by a constant.  This allows a
 * fairly fast implementation of the "brute force" approach of computing the
 * distance from every colormap entry to every histogram cell.  Unfortunately,
 * it needs a work array to hold the best-distance-so-far for each histogram
 * cell (because the inner loop has to be over cells, not colormap entries).
 * The work array elements have to be INT32s, so the work array would need
 * 256Kb at our recommended precision.  This is not feasible in DOS machines.
 *
 * To get around these problems, we apply Thomas' method to compute the
 * nearest colors for only the cells within a small subbox of the histogram.
 * The work array need be only as big as the subbox, so the memory usage
 * problem is solved.  Furthermore, we need not fill subboxes that are never
 * referenced in pass2; many images use only part of the color gamut, so a
 * fair amount of work is saved.  An additional advantage of this
 * approach is that we can apply Heckbert's locality criterion to quickly
 * eliminate colormap entries that are far away from the subbox; typically
 * three-fourths of the colormap entries are rejected by Heckbert's criterion,
 * and we need not compute their distances to individual cells in the subbox.
 * The speed of this approach is heavily influenced by the subbox size: too
 * small means too much overhead, too big loses because Heckbert's criterion
 * can't eliminate as many colormap entries.  Empirically the best subbox
 * size seems to be about 1/512th of the histogram (1/8th in each direction).
 *
 * Thomas' article also describes a refined method which is asymptotically
 * faster than the brute-force method, but it is also far more complex and
 * cannot efficiently be applied to small subboxes.  It is therefore not
 * useful for programs intended to be portable to DOS machines.  On machines
 * with plenty of memory, filling the whole histogram in one shot with Thomas'
 * refined method might be faster than the present code --- but then again,
 * it might not be any faster, and it's certainly more complicated.
 */


/* log2(histogram cells in update box) for each axis; this can be adjusted */
#define BOX_C0_LOG  (HIST_C0_BITS-3)
#define BOX_C1_LOG  (HIST_C1_BITS-3)
#define BOX_C2_LOG  (HIST_C2_BITS-3)

#define BOX_C0_ELEMS  (1<<BOX_C0_LOG) /* # of hist cells in update box */
#define BOX_C1_ELEMS  (1<<BOX_C1_LOG)
#define BOX_C2_ELEMS  (1<<BOX_C2_LOG)

#define BOX_C0_SHIFT  (C0_SHIFT + BOX_C0_LOG)
#define BOX_C1_SHIFT  (C1_SHIFT + BOX_C1_LOG)
#define BOX_C2_SHIFT  (C2_SHIFT + BOX_C2_LOG)


/*
 * The next three routines implement inverse colormap filling.  They could
 * all be folded into one big routine, but splitting them up this way saves
 * some stack space (the mindist[] and bestdist[] arrays need not coexist)
 * and may allow some compilers to produce better code by registerizing more
 * inner-loop variables.
 */

LOCAL(int)
find_nearby_colors (j_decompress_ptr cinfo, int minc0, int minc1, int minc2,
		    JSAMPLE colorlist[])
/* Locate the colormap entries close enough to an update box to be candidates
 * for the nearest entry to some cell(s) in the update box.  The update box
 * is specified by the center coordinates of its first cell.  The number of
 * candidate colormap entries is returned, and their colormap indexes are
 * placed in colorlist[].
 * This routine uses Heckbert's "locally sorted search" criterion to select
 * the colors that need further consideration.
 */
{
  int numcolors = cinfo->actual_number_of_colors;
__boundcheck_metadata_store((void *)(&numcolors),(void *)((size_t)(&numcolors)+sizeof(numcolors)*8-1));

  int maxc0;
__boundcheck_metadata_store((void *)(&maxc0),(void *)((size_t)(&maxc0)+sizeof(maxc0)*8-1));
int  maxc1;
__boundcheck_metadata_store((void *)(&maxc1),(void *)((size_t)(&maxc1)+sizeof(maxc1)*8-1));
int  maxc2;
__boundcheck_metadata_store((void *)(&maxc2),(void *)((size_t)(&maxc2)+sizeof(maxc2)*8-1));

  int centerc0;
__boundcheck_metadata_store((void *)(&centerc0),(void *)((size_t)(&centerc0)+sizeof(centerc0)*8-1));
int  centerc1;
__boundcheck_metadata_store((void *)(&centerc1),(void *)((size_t)(&centerc1)+sizeof(centerc1)*8-1));
int  centerc2;
__boundcheck_metadata_store((void *)(&centerc2),(void *)((size_t)(&centerc2)+sizeof(centerc2)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));
int  ncolors;
__boundcheck_metadata_store((void *)(&ncolors),(void *)((size_t)(&ncolors)+sizeof(ncolors)*8-1));

  INT32 minmaxdist;
__boundcheck_metadata_store((void *)(&minmaxdist),(void *)((size_t)(&minmaxdist)+sizeof(minmaxdist)*8-1));
INT32  min_dist;
__boundcheck_metadata_store((void *)(&min_dist),(void *)((size_t)(&min_dist)+sizeof(min_dist)*8-1));
INT32  max_dist;
__boundcheck_metadata_store((void *)(&max_dist),(void *)((size_t)(&max_dist)+sizeof(max_dist)*8-1));
INT32  tdist;
__boundcheck_metadata_store((void *)(&tdist),(void *)((size_t)(&tdist)+sizeof(tdist)*8-1));

  INT32 mindist[MAXNUMCOLORS];__boundcheck_metadata_store(&mindist[0],&mindist[256-1]);
	/* min distance to colormap entry i */

  /* Compute true coordinates of update box's upper corner and center.
   * Actually we compute the coordinates of the center of the upper-corner
   * histogram cell, which are the upper bounds of the volume we care about.
   * Note that since ">>" rounds down, the "center" values may be closer to
   * min than to max; hence comparisons to them must be "<=", not "<".
   */
  maxc0 = minc0 + ((1 << BOX_C0_SHIFT) - (1 << C0_SHIFT));
  centerc0 = (minc0 + maxc0) >> 1;
  maxc1 = minc1 + ((1 << BOX_C1_SHIFT) - (1 << C1_SHIFT));
  centerc1 = (minc1 + maxc1) >> 1;
  maxc2 = minc2 + ((1 << BOX_C2_SHIFT) - (1 << C2_SHIFT));
  centerc2 = (minc2 + maxc2) >> 1;

  /* For each color in colormap, find:
   *  1. its minimum squared-distance to any point in the update box
   *     (zero if color is within update box);
   *  2. its maximum squared-distance to any point in the update box.
   * Both of these can be found by considering only the corners of the box.
   * We save the minimum distance for each color in mindist[];
   * only the smallest maximum distance is of interest.
   */
  minmaxdist = 0x7FFFFFFFL;

  for (i = 0; i < numcolors; i++) {
    /* We compute the squared-c0-distance term, then add in the other two. */
    x = GETJSAMPLE(cinfo->colormap[0][i]);
    if (x < minc0) {
      tdist = (x - minc0) * C0_SCALE;
      min_dist = tdist*tdist;
      tdist = (x - maxc0) * C0_SCALE;
      max_dist = tdist*tdist;
    } else if (x > maxc0) {
      tdist = (x - maxc0) * C0_SCALE;
      min_dist = tdist*tdist;
      tdist = (x - minc0) * C0_SCALE;
      max_dist = tdist*tdist;
    } else {
      /* within cell range so no contribution to min_dist */
      min_dist = 0;
      if (x <= centerc0) {
	tdist = (x - maxc0) * C0_SCALE;
	max_dist = tdist*tdist;
      } else {
	tdist = (x - minc0) * C0_SCALE;
	max_dist = tdist*tdist;
      }
    }

    x = GETJSAMPLE(cinfo->colormap[1][i]);
    if (x < minc1) {
      tdist = (x - minc1) * C1_SCALE;
      min_dist += tdist*tdist;
      tdist = (x - maxc1) * C1_SCALE;
      max_dist += tdist*tdist;
    } else if (x > maxc1) {
      tdist = (x - maxc1) * C1_SCALE;
      min_dist += tdist*tdist;
      tdist = (x - minc1) * C1_SCALE;
      max_dist += tdist*tdist;
    } else {
      /* within cell range so no contribution to min_dist */
      if (x <= centerc1) {
	tdist = (x - maxc1) * C1_SCALE;
	max_dist += tdist*tdist;
      } else {
	tdist = (x - minc1) * C1_SCALE;
	max_dist += tdist*tdist;
      }
    }

    x = GETJSAMPLE(cinfo->colormap[2][i]);
    if (x < minc2) {
      tdist = (x - minc2) * C2_SCALE;
      min_dist += tdist*tdist;
      tdist = (x - maxc2) * C2_SCALE;
      max_dist += tdist*tdist;
    } else if (x > maxc2) {
      tdist = (x - maxc2) * C2_SCALE;
      min_dist += tdist*tdist;
      tdist = (x - minc2) * C2_SCALE;
      max_dist += tdist*tdist;
    } else {
      /* within cell range so no contribution to min_dist */
      if (x <= centerc2) {
	tdist = (x - maxc2) * C2_SCALE;
	max_dist += tdist*tdist;
      } else {
	tdist = (x - minc2) * C2_SCALE;
	max_dist += tdist*tdist;
      }
    }

    mindist[_RV_insert_check(0,256,756,5,"find_nearby_colors",i)] = min_dist;	/* save away the results */
    if (max_dist < minmaxdist)
      minmaxdist = max_dist;
  }

  /* Now we know that no cell in the update box is more than minmaxdist
   * away from some colormap entry.  Therefore, only colors that are
   * within minmaxdist of some part of the box need be considered.
   */
  ncolors = 0;
  for (i = 0; i < numcolors; i++) {
    if (mindist[_RV_insert_check(0,256,767,9,"find_nearby_colors",i)] <= minmaxdist)
      (*(JSAMPLE *)(__boundcheck_ptr_reference(768,7,"find_nearby_colors",(void *)(&colorlist[0]),(void *)(&colorlist[ncolors++])))) = (JSAMPLE) i;
  }
  return ncolors;
}


LOCAL(void)
find_best_colors (j_decompress_ptr cinfo, int minc0, int minc1, int minc2,
		  int numcolors, JSAMPLE colorlist[], JSAMPLE bestcolor[])
/* Find the closest colormap entry for each cell in the update box,
 * given the list of candidate colors prepared by find_nearby_colors.
 * Return the indexes of the closest entries in the bestcolor[] array.
 * This routine uses Thomas' incremental distance calculation method to
 * find the distance from a colormap entry to successive cells in the box.
 */
{
  int ic0;
__boundcheck_metadata_store((void *)(&ic0),(void *)((size_t)(&ic0)+sizeof(ic0)*8-1));
int  ic1;
__boundcheck_metadata_store((void *)(&ic1),(void *)((size_t)(&ic1)+sizeof(ic1)*8-1));
int  ic2;
__boundcheck_metadata_store((void *)(&ic2),(void *)((size_t)(&ic2)+sizeof(ic2)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  icolor;
__boundcheck_metadata_store((void *)(&icolor),(void *)((size_t)(&icolor)+sizeof(icolor)*8-1));

  register INT32 * bptr;	/* pointer into bestdist[] array */
  JSAMPLE * cptr;
__boundcheck_metadata_store((void *)(&cptr),(void *)((size_t)(&cptr)+sizeof(cptr)*8-1));
		/* pointer into bestcolor[] array */
  INT32 dist0;
__boundcheck_metadata_store((void *)(&dist0),(void *)((size_t)(&dist0)+sizeof(dist0)*8-1));
INT32  dist1;
__boundcheck_metadata_store((void *)(&dist1),(void *)((size_t)(&dist1)+sizeof(dist1)*8-1));
		/* initial distance values */
  register INT32 dist2;		/* current distance in inner loop */
  INT32 xx0;
__boundcheck_metadata_store((void *)(&xx0),(void *)((size_t)(&xx0)+sizeof(xx0)*8-1));
INT32  xx1;
__boundcheck_metadata_store((void *)(&xx1),(void *)((size_t)(&xx1)+sizeof(xx1)*8-1));
		/* distance increments */
  register INT32 xx2;
  INT32 inc0;
__boundcheck_metadata_store((void *)(&inc0),(void *)((size_t)(&inc0)+sizeof(inc0)*8-1));
INT32  inc1;
__boundcheck_metadata_store((void *)(&inc1),(void *)((size_t)(&inc1)+sizeof(inc1)*8-1));
INT32  inc2;
__boundcheck_metadata_store((void *)(&inc2),(void *)((size_t)(&inc2)+sizeof(inc2)*8-1));
	/* initial values for increments */
  /* This array holds the distance to the nearest-so-far color for each cell */
  INT32 bestdist[BOX_C0_ELEMS * BOX_C1_ELEMS * BOX_C2_ELEMS];__boundcheck_metadata_store(&bestdist[0],&bestdist[128-1]);


  /* Initialize best-distance for each cell of the update box */
  bptr = bestdist;
__boundcheck_metadata_trans_check((void *)(bptr),(void *)(bestdist),(void *)(bestdist));

  for (i = BOX_C0_ELEMS*BOX_C1_ELEMS*BOX_C2_ELEMS-1; i >= 0; i--)
    *(INT32 *)(__boundcheck_ptr_reference(799,10,"find_best_colors",(void *)(bptr),(void *)(bptr++))) = 0x7FFFFFFFL;
  
  /* For each color selected by find_nearby_colors,
   * compute its distance to the center of each cell in the box.
   * If that's less than best-so-far, update best distance and color number.
   */
  
  /* Nominal steps between cell centers ("x" in Thomas article) */
#define STEP_C0  ((1 << C0_SHIFT) * C0_SCALE)
#define STEP_C1  ((1 << C1_SHIFT) * C1_SCALE)
#define STEP_C2  ((1 << C2_SHIFT) * C2_SCALE)
  
  for (i = 0; i < numcolors; i++) {
    icolor = GETJSAMPLE(colorlist[i]);
    /* Compute (square of) distance from minc0/c1/c2 to this color */
    inc0 = (minc0 - GETJSAMPLE(cinfo->colormap[0][icolor])) * C0_SCALE;
    dist0 = inc0*inc0;
    inc1 = (minc1 - GETJSAMPLE(cinfo->colormap[1][icolor])) * C1_SCALE;
    dist0 += inc1*inc1;
    inc2 = (minc2 - GETJSAMPLE(cinfo->colormap[2][icolor])) * C2_SCALE;
    dist0 += inc2*inc2;
    /* Form the initial difference increments */
    inc0 = inc0 * (2 * STEP_C0) + STEP_C0 * STEP_C0;
    inc1 = inc1 * (2 * STEP_C1) + STEP_C1 * STEP_C1;
    inc2 = inc2 * (2 * STEP_C2) + STEP_C2 * STEP_C2;
    /* Now loop over all cells in box, updating distance per Thomas method */
    bptr = bestdist;
__boundcheck_metadata_trans_check((void *)(bptr),(void *)(bestdist),(void *)(bestdist));

    cptr = bestcolor;
__boundcheck_metadata_trans_check((void *)(cptr),(void *)(bestcolor),(void *)(bestcolor));

    xx0 = inc0;
    for (ic0 = BOX_C0_ELEMS-1; ic0 >= 0; ic0--) {
      dist1 = dist0;
      xx1 = inc1;
      for (ic1 = BOX_C1_ELEMS-1; ic1 >= 0; ic1--) {
	dist2 = dist1;
	xx2 = inc2;
	for (ic2 = BOX_C2_ELEMS-1; ic2 >= 0; ic2--) {
	  if (dist2 < *(INT32 *)(__boundcheck_ptr_reference(835,17,"find_best_colors",(void *)(bptr),(void *)(bptr)))) {
	    *(INT32 *)(__boundcheck_ptr_reference(836,7,"find_best_colors",(void *)(bptr),(void *)(bptr))) = dist2;
	    *(JSAMPLE *)(__boundcheck_ptr_reference(837,7,"find_best_colors",(void *)(cptr),(void *)(cptr))) = (JSAMPLE) icolor;
	  }
	  dist2 += xx2;
	  xx2 += 2 * STEP_C2 * STEP_C2;
	  bptr++;
	  cptr++;
	}
	dist1 += xx1;
	xx1 += 2 * STEP_C1 * STEP_C1;
      }
      dist0 += xx0;
      xx0 += 2 * STEP_C0 * STEP_C0;
    }
  }
}


LOCAL(void)
fill_inverse_cmap (j_decompress_ptr cinfo, int c0, int c1, int c2)
/* Fill the inverse-colormap entries in the update box that contains */
/* histogram cell c0/c1/c2.  (Only that one cell MUST be filled, but */
/* we can fill as many others as we wish.) */
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  hist3d histogram = cquantize->histogram;
__boundcheck_metadata_store((void *)(&histogram),(void *)((size_t)(&histogram)+sizeof(histogram)*8-1));

  int minc0;
__boundcheck_metadata_store((void *)(&minc0),(void *)((size_t)(&minc0)+sizeof(minc0)*8-1));
int  minc1;
__boundcheck_metadata_store((void *)(&minc1),(void *)((size_t)(&minc1)+sizeof(minc1)*8-1));
int  minc2;
__boundcheck_metadata_store((void *)(&minc2),(void *)((size_t)(&minc2)+sizeof(minc2)*8-1));
	/* lower left corner of update box */
  int ic0;
__boundcheck_metadata_store((void *)(&ic0),(void *)((size_t)(&ic0)+sizeof(ic0)*8-1));
int  ic1;
__boundcheck_metadata_store((void *)(&ic1),(void *)((size_t)(&ic1)+sizeof(ic1)*8-1));
int  ic2;
__boundcheck_metadata_store((void *)(&ic2),(void *)((size_t)(&ic2)+sizeof(ic2)*8-1));

  register JSAMPLE * cptr;	/* pointer into bestcolor[] array */
  register histptr cachep;	/* pointer into main cache array */
  /* This array lists the candidate colormap indexes. */
  JSAMPLE colorlist[MAXNUMCOLORS];__boundcheck_metadata_store(&colorlist[0],&colorlist[256-1]);

  int numcolors;
__boundcheck_metadata_store((void *)(&numcolors),(void *)((size_t)(&numcolors)+sizeof(numcolors)*8-1));
		/* number of candidate colors */
  /* This array holds the actually closest colormap index for each cell. */
  JSAMPLE bestcolor[BOX_C0_ELEMS * BOX_C1_ELEMS * BOX_C2_ELEMS];__boundcheck_metadata_store(&bestcolor[0],&bestcolor[128-1]);


  /* Convert cell coordinates to update box ID */
  c0 >>= BOX_C0_LOG;
  c1 >>= BOX_C1_LOG;
  c2 >>= BOX_C2_LOG;

  /* Compute true coordinates of update box's origin corner.
   * Actually we compute the coordinates of the center of the corner
   * histogram cell, which are the lower bounds of the volume we care about.
   */
  minc0 = (c0 << BOX_C0_SHIFT) + ((1 << C0_SHIFT) >> 1);
  minc1 = (c1 << BOX_C1_SHIFT) + ((1 << C1_SHIFT) >> 1);
  minc2 = (c2 << BOX_C2_SHIFT) + ((1 << C2_SHIFT) >> 1);
  
  /* Determine which colormap entries are close enough to be candidates
   * for the nearest entry to some cell in the update box.
   */
  numcolors = find_nearby_colors(cinfo, minc0, minc1, minc2, colorlist);

  /* Determine the actually nearest colors. */
  find_best_colors(cinfo, minc0, minc1, minc2, numcolors, colorlist,
		   bestcolor);

  /* Save the best color numbers (plus 1) in the main cache array */
  c0 <<= BOX_C0_LOG;		/* convert ID back to base cell indexes */
  c1 <<= BOX_C1_LOG;
  c2 <<= BOX_C2_LOG;
  cptr = bestcolor;
__boundcheck_metadata_trans_check((void *)(cptr),(void *)(bestcolor),(void *)(bestcolor));

  for (ic0 = 0; ic0 < BOX_C0_ELEMS; ic0++) {
    for (ic1 = 0; ic1 < BOX_C1_ELEMS; ic1++) {
      cachep = & (*(histcell *)(__boundcheck_ptr_reference(901,18,"fill_inverse_cmap",(void *)(&histogram[c0 + ic0][c1 + ic1][0]),(void *)(&histogram[c0 + ic0][c1 + ic1][c2]))));
      for (ic2 = 0; ic2 < BOX_C2_ELEMS; ic2++) {
	*(histptr)(__boundcheck_ptr_reference(903,9,"fill_inverse_cmap",(void *)(cachep),(void *)(cachep++))) = (histcell) (GETJSAMPLE(*cptr++) + 1);
      }
    }
  }
}


/*
 * Map some rows of pixels to the output colormapped representation.
 */

METHODDEF(void)
pass2_no_dither (j_decompress_ptr cinfo,
		 JSAMPARRAY input_buf, JSAMPARRAY output_buf, int num_rows)
/* This version performs no dithering */
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  hist3d histogram = cquantize->histogram;
__boundcheck_metadata_store((void *)(&histogram),(void *)((size_t)(&histogram)+sizeof(histogram)*8-1));

  register JSAMPROW inptr, outptr;
  register histptr cachep;
  register int c0, c1, c2;
  int row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));

  JDIMENSION col;
__boundcheck_metadata_store((void *)(&col),(void *)((size_t)(&col)+sizeof(col)*8-1));

  JDIMENSION width = cinfo->output_width;
__boundcheck_metadata_store((void *)(&width),(void *)((size_t)(&width)+sizeof(width)*8-1));


  for (row = 0; row < num_rows; row++) {
    inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(929,13,"pass2_no_dither",(void *)(&input_buf[0]),(void *)(&input_buf[row]))));
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(930,14,"pass2_no_dither",(void *)(&output_buf[0]),(void *)(&output_buf[row]))));
    for (col = width; col > 0; col--) {
      /* get pixel value and index into the cache */
      c0 = GETJSAMPLE(*inptr++) >> C0_SHIFT;
      c1 = GETJSAMPLE(*inptr++) >> C1_SHIFT;
      c2 = GETJSAMPLE(*inptr++) >> C2_SHIFT;
      cachep = & (*(histcell *)(__boundcheck_ptr_reference(936,18,"pass2_no_dither",(void *)(&histogram[c0][c1][0]),(void *)(&histogram[c0][c1][c2]))));
      /* If we have not seen this color before, find nearest colormap entry */
      /* and update the cache */
      if (*(histptr)(__boundcheck_ptr_reference(939,12,"pass2_no_dither",(void *)(cachep),(void *)(cachep))) == 0)
	fill_inverse_cmap(cinfo, c0,c1,c2);
      /* Now emit the colormap index for this cell */
      *(JSAMPROW)(__boundcheck_ptr_reference(942,14,"pass2_no_dither",(void *)(outptr),(void *)(outptr++))) = (JSAMPLE) (*(histptr)(__boundcheck_ptr_reference(942,31,"pass2_no_dither",(void *)(cachep),(void *)(cachep))) - 1);
    }
  }
}


METHODDEF(void)
pass2_fs_dither (j_decompress_ptr cinfo,
		 JSAMPARRAY input_buf, JSAMPARRAY output_buf, int num_rows)
/* This version performs Floyd-Steinberg dithering */
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  hist3d histogram = cquantize->histogram;
__boundcheck_metadata_store((void *)(&histogram),(void *)((size_t)(&histogram)+sizeof(histogram)*8-1));

  register LOCFSERROR cur0, cur1, cur2;	/* current error or pixel value */
  LOCFSERROR belowerr0;
__boundcheck_metadata_store((void *)(&belowerr0),(void *)((size_t)(&belowerr0)+sizeof(belowerr0)*8-1));
LOCFSERROR  belowerr1;
__boundcheck_metadata_store((void *)(&belowerr1),(void *)((size_t)(&belowerr1)+sizeof(belowerr1)*8-1));
LOCFSERROR  belowerr2;
__boundcheck_metadata_store((void *)(&belowerr2),(void *)((size_t)(&belowerr2)+sizeof(belowerr2)*8-1));
 /* error for pixel below cur */
  LOCFSERROR bpreverr0;
__boundcheck_metadata_store((void *)(&bpreverr0),(void *)((size_t)(&bpreverr0)+sizeof(bpreverr0)*8-1));
LOCFSERROR  bpreverr1;
__boundcheck_metadata_store((void *)(&bpreverr1),(void *)((size_t)(&bpreverr1)+sizeof(bpreverr1)*8-1));
LOCFSERROR  bpreverr2;
__boundcheck_metadata_store((void *)(&bpreverr2),(void *)((size_t)(&bpreverr2)+sizeof(bpreverr2)*8-1));
 /* error for below/prev col */
  register FSERRPTR errorptr;	/* => fserrors[] at column before current */
  JSAMPROW inptr;
__boundcheck_metadata_store((void *)(&inptr),(void *)((size_t)(&inptr)+sizeof(inptr)*8-1));
		/* => current input pixel */
  JSAMPROW outptr;
__boundcheck_metadata_store((void *)(&outptr),(void *)((size_t)(&outptr)+sizeof(outptr)*8-1));
		/* => current output pixel */
  histptr cachep;
__boundcheck_metadata_store((void *)(&cachep),(void *)((size_t)(&cachep)+sizeof(cachep)*8-1));

  int dir;
__boundcheck_metadata_store((void *)(&dir),(void *)((size_t)(&dir)+sizeof(dir)*8-1));
			/* +1 or -1 depending on direction */
  int dir3;
__boundcheck_metadata_store((void *)(&dir3),(void *)((size_t)(&dir3)+sizeof(dir3)*8-1));
			/* 3*dir, for advancing inptr & errorptr */
  int row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));

  JDIMENSION col;
__boundcheck_metadata_store((void *)(&col),(void *)((size_t)(&col)+sizeof(col)*8-1));

  JDIMENSION width = cinfo->output_width;
__boundcheck_metadata_store((void *)(&width),(void *)((size_t)(&width)+sizeof(width)*8-1));

  JSAMPLE *range_limit = cinfo->sample_range_limit;
__boundcheck_metadata_store((void *)(&range_limit),(void *)((size_t)(&range_limit)+sizeof(range_limit)*8-1));

  int *error_limit = cquantize->error_limiter;
__boundcheck_metadata_store((void *)(&error_limit),(void *)((size_t)(&error_limit)+sizeof(error_limit)*8-1));

  JSAMPROW colormap0 = cinfo->colormap[0];
__boundcheck_metadata_store((void *)(&colormap0),(void *)((size_t)(&colormap0)+sizeof(colormap0)*8-1));

  JSAMPROW colormap1 = cinfo->colormap[1];
__boundcheck_metadata_store((void *)(&colormap1),(void *)((size_t)(&colormap1)+sizeof(colormap1)*8-1));

  JSAMPROW colormap2 = cinfo->colormap[2];
__boundcheck_metadata_store((void *)(&colormap2),(void *)((size_t)(&colormap2)+sizeof(colormap2)*8-1));

  SHIFT_TEMPS

  for (row = 0; row < num_rows; row++) {
    inptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(975,13,"pass2_fs_dither",(void *)(&input_buf[0]),(void *)(&input_buf[row]))));
    outptr = (*(JSAMPROW *)(__boundcheck_ptr_reference(976,14,"pass2_fs_dither",(void *)(&output_buf[0]),(void *)(&output_buf[row]))));
    if (cquantize->on_odd_row) {
      /* work right to left in this row */
      inptr += (width-1) * 3;	/* so point to rightmost pixel */
      outptr += width-1;
      dir = -1;
      dir3 = -3;
      errorptr = cquantize->fserrors + (width+1)*3; /* => entry after last column */
      cquantize->on_odd_row = FALSE; /* flip for next time */
    } else {
      /* work left to right in this row */
      dir = 1;
      dir3 = 3;
      errorptr = cquantize->fserrors; /* => entry before first real column */
      cquantize->on_odd_row = TRUE; /* flip for next time */
    }
    /* Preset error values: no error propagated to first pixel from left */
    cur0 = cur1 = cur2 = 0;
    /* and no error propagated to row below yet */
    belowerr0 = belowerr1 = belowerr2 = 0;
    bpreverr0 = bpreverr1 = bpreverr2 = 0;

    for (col = width; col > 0; col--) {
      /* curN holds the error propagated from the previous pixel on the
       * current line.  Add the error propagated from the previous line
       * to form the complete error correction term for this pixel, and
       * round the error term (which is expressed * 16) to an integer.
       * RIGHT_SHIFT rounds towards minus infinity, so adding 8 is correct
       * for either sign of the error value.
       * Note: errorptr points to *previous* column's array entry.
       */
      cur0 = RIGHT_SHIFT(cur0 + errorptr[dir3+0] + 8, 4);
      cur1 = RIGHT_SHIFT(cur1 + errorptr[dir3+1] + 8, 4);
      cur2 = RIGHT_SHIFT(cur2 + errorptr[dir3+2] + 8, 4);
      /* Limit the error using transfer function set by init_error_limit.
       * See comments with init_error_limit for rationale.
       */
      cur0 = (*(int *)(__boundcheck_ptr_reference(1013,14,"pass2_fs_dither",(void *)(&error_limit[0]),(void *)(&error_limit[cur0]))));
      cur1 = (*(int *)(__boundcheck_ptr_reference(1014,14,"pass2_fs_dither",(void *)(&error_limit[0]),(void *)(&error_limit[cur1]))));
      cur2 = (*(int *)(__boundcheck_ptr_reference(1015,14,"pass2_fs_dither",(void *)(&error_limit[0]),(void *)(&error_limit[cur2]))));
      /* Form pixel value + error, and range-limit to 0..MAXJSAMPLE.
       * The maximum error is +- MAXJSAMPLE (or less with error limiting);
       * this sets the required size of the range_limit array.
       */
      cur0 += GETJSAMPLE(inptr[0]);
      cur1 += GETJSAMPLE(inptr[1]);
      cur2 += GETJSAMPLE(inptr[2]);
      cur0 = GETJSAMPLE(range_limit[cur0]);
      cur1 = GETJSAMPLE(range_limit[cur1]);
      cur2 = GETJSAMPLE(range_limit[cur2]);
      /* Index into the cache with adjusted pixel value */
      cachep = & (*(histcell *)(__boundcheck_ptr_reference(1027,18,"pass2_fs_dither",(void *)(&histogram[cur0 >> (8 - 5)][cur1 >> (8 - 6)][0]),(void *)(&histogram[cur0 >> (8 - 5)][cur1 >> (8 - 6)][cur2 >> (8 - 5)]))));
      /* If we have not seen this color before, find nearest colormap */
      /* entry and update the cache */
      if (*(histptr)(__boundcheck_ptr_reference(1030,12,"pass2_fs_dither",(void *)(cachep),(void *)(cachep))) == 0)
	fill_inverse_cmap(cinfo, cur0>>C0_SHIFT,cur1>>C1_SHIFT,cur2>>C2_SHIFT);
      /* Now emit the colormap index for this cell */
      { register int pixcode = *(histptr)(__boundcheck_ptr_reference(1033,33,"pass2_fs_dither",(void *)(cachep),(void *)(cachep))) - 1;
	*(JSAMPROW)(__boundcheck_ptr_reference(1034,3,"pass2_fs_dither",(void *)(outptr),(void *)(outptr))) = (JSAMPLE) pixcode;
	/* Compute representation error for this pixel */
	cur0 -= GETJSAMPLE(colormap0[pixcode]);
	cur1 -= GETJSAMPLE(colormap1[pixcode]);
	cur2 -= GETJSAMPLE(colormap2[pixcode]);
      }
      /* Compute error fractions to be propagated to adjacent pixels.
       * Add these into the running sums, and simultaneously shift the
       * next-line error sums left by 1 column.
       */
      { register LOCFSERROR bnexterr, delta;

	bnexterr = cur0;	/* Process component 0 */
	delta = cur0 * 2;
	cur0 += delta;		/* form error * 3 */
	(*(FSERROR *)(__boundcheck_ptr_reference(1049,2,"pass2_fs_dither",(void *)(&errorptr[0]),(void *)(&errorptr[0])))) = (FSERROR) (bpreverr0 + cur0);
	cur0 += delta;		/* form error * 5 */
	bpreverr0 = belowerr0 + cur0;
	belowerr0 = bnexterr;
	cur0 += delta;		/* form error * 7 */
	bnexterr = cur1;	/* Process component 1 */
	delta = cur1 * 2;
	cur1 += delta;		/* form error * 3 */
	(*(FSERROR *)(__boundcheck_ptr_reference(1057,2,"pass2_fs_dither",(void *)(&errorptr[0]),(void *)(&errorptr[1])))) = (FSERROR) (bpreverr1 + cur1);
	cur1 += delta;		/* form error * 5 */
	bpreverr1 = belowerr1 + cur1;
	belowerr1 = bnexterr;
	cur1 += delta;		/* form error * 7 */
	bnexterr = cur2;	/* Process component 2 */
	delta = cur2 * 2;
	cur2 += delta;		/* form error * 3 */
	(*(FSERROR *)(__boundcheck_ptr_reference(1065,2,"pass2_fs_dither",(void *)(&errorptr[0]),(void *)(&errorptr[2])))) = (FSERROR) (bpreverr2 + cur2);
	cur2 += delta;		/* form error * 5 */
	bpreverr2 = belowerr2 + cur2;
	belowerr2 = bnexterr;
	cur2 += delta;		/* form error * 7 */
      }
      /* At this point curN contains the 7/16 error value to be propagated
       * to the next pixel on the current line, and all the errors for the
       * next line have been shifted over.  We are therefore ready to move on.
       */
      inptr += dir3;		/* Advance pixel pointers to next column */
      outptr += dir;
      errorptr += dir3;		/* advance errorptr to current column */
    }
    /* Post-loop cleanup: we must unload the final error values into the
     * final fserrors[] entry.  Note we need not unload belowerrN because
     * it is for the dummy column before or after the actual array.
     */
    (*(FSERROR *)(__boundcheck_ptr_reference(1083,5,"pass2_fs_dither",(void *)(&errorptr[0]),(void *)(&errorptr[0])))) = (FSERROR) bpreverr0; /* unload prev errs into array */
    (*(FSERROR *)(__boundcheck_ptr_reference(1084,5,"pass2_fs_dither",(void *)(&errorptr[0]),(void *)(&errorptr[1])))) = (FSERROR) bpreverr1;
    (*(FSERROR *)(__boundcheck_ptr_reference(1085,5,"pass2_fs_dither",(void *)(&errorptr[0]),(void *)(&errorptr[2])))) = (FSERROR) bpreverr2;
  }
}


/*
 * Initialize the error-limiting transfer function (lookup table).
 * The raw F-S error computation can potentially compute error values of up to
 * +- MAXJSAMPLE.  But we want the maximum correction applied to a pixel to be
 * much less, otherwise obviously wrong pixels will be created.  (Typical
 * effects include weird fringes at color-area boundaries, isolated bright
 * pixels in a dark area, etc.)  The standard advice for avoiding this problem
 * is to ensure that the "corners" of the color cube are allocated as output
 * colors; then repeated errors in the same direction cannot cause cascading
 * error buildup.  However, that only prevents the error from getting
 * completely out of hand; Aaron Giles reports that error limiting improves
 * the results even with corner colors allocated.
 * A simple clamping of the error values to about +- MAXJSAMPLE/8 works pretty
 * well, but the smoother transfer function used below is even better.  Thanks
 * to Aaron Giles for this idea.
 */

LOCAL(void)
init_error_limit (j_decompress_ptr cinfo)
/* Allocate and fill in the error_limiter table */
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  int * table;
__boundcheck_metadata_store((void *)(&table),(void *)((size_t)(&table)+sizeof(table)*8-1));

  int in;
__boundcheck_metadata_store((void *)(&in),(void *)((size_t)(&in)+sizeof(in)*8-1));
int  out;
__boundcheck_metadata_store((void *)(&out),(void *)((size_t)(&out)+sizeof(out)*8-1));


  table = (int *) (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(1115,33,"init_error_limit",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE, (MAXJSAMPLE*2+1) * SIZEOF(int));
  table += MAXJSAMPLE;		/* so can index -MAXJSAMPLE .. +MAXJSAMPLE */
  cquantize->error_limiter = table;

#define STEPSIZE ((MAXJSAMPLE+1)/16)
  /* Map errors 1:1 up to +- MAXJSAMPLE/16 */
  out = 0;
  for (in = 0; in < STEPSIZE; in++, out++) {
    (*(int *)(__boundcheck_ptr_reference(1124,5,"init_error_limit",(void *)(&table[0]),(void *)(&table[in])))) = out; (*(int *)(__boundcheck_ptr_reference(1124,22,"init_error_limit",(void *)(&table[0]),(void *)(&table[-in])))) = -out;
  }
  /* Map errors 1:2 up to +- 3*MAXJSAMPLE/16 */
  for (; in < STEPSIZE*3; in++, out += (in&1) ? 0 : 1) {
    (*(int *)(__boundcheck_ptr_reference(1128,5,"init_error_limit",(void *)(&table[0]),(void *)(&table[in])))) = out; (*(int *)(__boundcheck_ptr_reference(1128,22,"init_error_limit",(void *)(&table[0]),(void *)(&table[-in])))) = -out;
  }
  /* Clamp the rest to final out value (which is (MAXJSAMPLE+1)/8) */
  for (; in <= MAXJSAMPLE; in++) {
    (*(int *)(__boundcheck_ptr_reference(1132,5,"init_error_limit",(void *)(&table[0]),(void *)(&table[in])))) = out; (*(int *)(__boundcheck_ptr_reference(1132,22,"init_error_limit",(void *)(&table[0]),(void *)(&table[-in])))) = -out;
  }
#undef STEPSIZE
}


/*
 * Finish up at the end of each pass.
 */

METHODDEF(void)
finish_pass1 (j_decompress_ptr cinfo)
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));


  /* Select the representative colors and fill in cinfo->colormap */
  cinfo->colormap = cquantize->sv_colormap;
  select_colors(cinfo, cquantize->desired);
  /* Force next pass to zero the color index table */
  cquantize->needs_zeroed = TRUE;
}


METHODDEF(void)
finish_pass2 (j_decompress_ptr cinfo)
{
  /* no work */
}


/*
 * Initialize for each processing pass.
 */

METHODDEF(void)
start_pass_2_quant (j_decompress_ptr cinfo, boolean is_pre_scan)
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  hist3d histogram = cquantize->histogram;
__boundcheck_metadata_store((void *)(&histogram),(void *)((size_t)(&histogram)+sizeof(histogram)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  /* Only F-S dithering or no dithering is supported. */
  /* If user asks for ordered dither, give him F-S. */
  if (cinfo->dither_mode != JDITHER_NONE)
    cinfo->dither_mode = JDITHER_FS;

  if (is_pre_scan) {
    /* Set up method pointers */
    cquantize->pub.color_quantize = prescan_quantize;
    cquantize->pub.finish_pass = finish_pass1;
    cquantize->needs_zeroed = TRUE; /* Always zero histogram */
  } else {
    /* Set up method pointers */
    if (cinfo->dither_mode == JDITHER_FS)
      cquantize->pub.color_quantize = pass2_fs_dither;
    else
      cquantize->pub.color_quantize = pass2_no_dither;
    cquantize->pub.finish_pass = finish_pass2;

    /* Make sure color count is acceptable */
    i = cinfo->actual_number_of_colors;
    if (i < 1)
      ERREXIT1(cinfo, JERR_QUANT_FEW_COLORS, 1);
    if (i > MAXNUMCOLORS)
      ERREXIT1(cinfo, JERR_QUANT_MANY_COLORS, MAXNUMCOLORS);

    if (cinfo->dither_mode == JDITHER_FS) {
      size_t arraysize = (size_t) ((cinfo->output_width + 2) *
				   (3 * SIZEOF(FSERROR)));
__boundcheck_metadata_store((void *)(&arraysize),(void *)((size_t)(&arraysize)+sizeof(arraysize)*8-1));

      /* Allocate Floyd-Steinberg workspace if we didn't already. */
      if (cquantize->fserrors == NULL)
	cquantize->fserrors = (FSERRPTR) (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(1203,49,"start_pass_2_quant",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large)))
	  ((j_common_ptr) cinfo, JPOOL_IMAGE, arraysize);
      /* Initialize the propagated errors to zero. */
      jzero_far((void FAR *) cquantize->fserrors, arraysize);
      /* Make the error-limit table if we didn't already. */
      if (cquantize->error_limiter == NULL)
	init_error_limit(cinfo);
      cquantize->on_odd_row = FALSE;
    }

  }
  /* Zero the histogram or inverse color map, if necessary */
  if (cquantize->needs_zeroed) {
    for (i = 0; i < HIST_C0_ELEMS; i++) {
      jzero_far((void FAR *) (*(hist2d *)(__boundcheck_ptr_reference(1217,30,"start_pass_2_quant",(void *)(&histogram[0]),(void *)(&histogram[i])))),
		HIST_C1_ELEMS*HIST_C2_ELEMS * SIZEOF(histcell));
    }
    cquantize->needs_zeroed = FALSE;
  }
}


/*
 * Switch to a new external colormap between output passes.
 */

METHODDEF(void)
new_color_map_2_quant (j_decompress_ptr cinfo)
{
  my_cquantize_ptr cquantize = (my_cquantize_ptr) cinfo->cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));


  /* Reset the inverse color map */
  cquantize->needs_zeroed = TRUE;
}


/*
 * Module initialization routine for 2-pass color quantization.
 */

GLOBAL(void)
jinit_2pass_quantizer (j_decompress_ptr cinfo)
{
  my_cquantize_ptr cquantize;
__boundcheck_metadata_store((void *)(&cquantize),(void *)((size_t)(&cquantize)+sizeof(cquantize)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


  cquantize = (my_cquantize_ptr)
    (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(1250,19,"jinit_2pass_quantizer",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small))) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_cquantizer));
  cinfo->cquantize = (struct jpeg_color_quantizer *) cquantize;
  cquantize->pub.start_pass = start_pass_2_quant;
  cquantize->pub.new_color_map = new_color_map_2_quant;
  cquantize->fserrors = NULL;	/* flag optional arrays not allocated */
  cquantize->error_limiter = NULL;

  /* Make sure jdmaster didn't give me a case I can't handle */
  if (cinfo->out_color_components != 3)
    ERREXIT(cinfo, JERR_NOTIMPL);

  /* Allocate the histogram/inverse colormap storage */
  cquantize->histogram = (hist3d) (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(1263,49,"jinit_2pass_quantizer",(void *)(cinfo->mem->alloc_small),(void *)cinfo->mem->alloc_small)))
    ((j_common_ptr) cinfo, JPOOL_IMAGE, HIST_C0_ELEMS * SIZEOF(hist2d));
  for (i = 0; i < HIST_C0_ELEMS; i++) {
    cquantize->histogram[i] = (hist2d) (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(1266,54,"jinit_2pass_quantizer",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large)))
      ((j_common_ptr) cinfo, JPOOL_IMAGE,
       HIST_C1_ELEMS*HIST_C2_ELEMS * SIZEOF(histcell));
  }
  cquantize->needs_zeroed = TRUE; /* histogram is garbage now */

  /* Allocate storage for the completed colormap, if required.
   * We do this now since it is FAR storage and may affect
   * the memory manager's space calculations.
   */
  if (cinfo->enable_2pass_quant) {
    /* Make sure color count is acceptable */
    int desired = cinfo->desired_number_of_colors;
__boundcheck_metadata_store((void *)(&desired),(void *)((size_t)(&desired)+sizeof(desired)*8-1));

    /* Lower bound on # of colors ... somewhat arbitrary as long as > 0 */
    if (desired < 8)
      ERREXIT1(cinfo, JERR_QUANT_FEW_COLORS, 8);
    /* Make sure colormap indexes can be represented by JSAMPLEs */
    if (desired > MAXNUMCOLORS)
      ERREXIT1(cinfo, JERR_QUANT_MANY_COLORS, MAXNUMCOLORS);
    cquantize->sv_colormap = (*(JSAMPARRAY (*)(j_common_ptr, int, JDIMENSION, JDIMENSION))(__boundcheck_ptr_reference(1285,44,"jinit_2pass_quantizer",(void *)(cinfo->mem->alloc_sarray),(void *)cinfo->mem->alloc_sarray)))
      ((j_common_ptr) cinfo,JPOOL_IMAGE, (JDIMENSION) desired, (JDIMENSION) 3);
    cquantize->desired = desired;
  } else
    cquantize->sv_colormap = NULL;

  /* Only F-S dithering or no dithering is supported. */
  /* If user asks for ordered dither, give him F-S. */
  if (cinfo->dither_mode != JDITHER_NONE)
    cinfo->dither_mode = JDITHER_FS;

  /* Allocate Floyd-Steinberg workspace if necessary.
   * This isn't really needed until pass 2, but again it is FAR storage.
   * Although we will cope with a later change in dither_mode,
   * we do not promise to honor max_memory_to_use if dither_mode changes.
   */
  if (cinfo->dither_mode == JDITHER_FS) {
    cquantize->fserrors = (FSERRPTR) (*(void *(*)(j_common_ptr, int, size_t))(__boundcheck_ptr_reference(1302,52,"jinit_2pass_quantizer",(void *)(cinfo->mem->alloc_large),(void *)cinfo->mem->alloc_large)))
      ((j_common_ptr) cinfo, JPOOL_IMAGE,
       (size_t) ((cinfo->output_width + 2) * (3 * SIZEOF(FSERROR))));
    /* Might as well create the error-limiting table too. */
    init_error_limit(cinfo);
  }
}

#endif /* QUANT_2PASS_SUPPORTED */
