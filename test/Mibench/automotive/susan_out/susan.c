/* {{{ Copyright etc. */

/**********************************************************************\

  SUSAN Version 2l by Stephen Smith
  Oxford Centre for Functional Magnetic Resonance Imaging of the Brain,
  Department of Clinical Neurology, Oxford University, Oxford, UK
  (Previously in Computer Vision and Image Processing Group - now
  Computer Vision and Electro Optics Group - DERA Chertsey, UK)
  Email:    steve@fmrib.ox.ac.uk
  WWW:      http://www.fmrib.ox.ac.uk/~steve

  (C) Crown Copyright (1995-1999), Defence Evaluation and Research Agency,
  Farnborough, Hampshire, GU14 6TD, UK
  DERA WWW site:
  http://www.dera.gov.uk/
  DERA Computer Vision and Electro Optics Group WWW site:
  http://www.dera.gov.uk/imageprocessing/dera/group_home.html
  DERA Computer Vision and Electro Optics Group point of contact:
  Dr. John Savage, jtsavage@dera.gov.uk, +44 1344 633203

  A UK patent has been granted: "Method for digitally processing
  images to determine the position of edges and/or corners therein for
  guidance of unmanned vehicle", UK Patent 2272285. Proprietor:
  Secretary of State for Defence, UK. 15 January 1997

  This code is issued for research purposes only and remains the
  property of the UK Secretary of State for Defence. This code must
  not be passed on without this header information being kept
  intact. This code must not be sold.

\**********************************************************************/

/* }}} */
/* {{{ Readme First */

/**********************************************************************\

  SUSAN Version 2l
  SUSAN = Smallest Univalue Segment Assimilating Nucleus

  Email:    steve@fmrib.ox.ac.uk
  WWW:      http://www.fmrib.ox.ac.uk/~steve

  Related paper:
  @article{Smith97,
        author = "Smith, S.M. and Brady, J.M.",
        title = "{SUSAN} - A New Approach to Low Level Image Processing",
        journal = "Int. Journal of Computer Vision",
        pages = "45--78",
        volume = "23",
        number = "1",
        month = "May",
        year = 1997}

  To be registered for automatic (bug) updates of SUSAN, send an email.

  Compile with:
  gcc -O4 -o susan susan2l.c -lm

  See following section for different machine information. Please
  report any bugs (and fixes). There are a few optional changes that
  can be made in the "defines" section which follows shortly.

  Usage: type "susan" to get usage. Only PGM format files can be input
  and output. Utilities such as the netpbm package and XV can be used
  to convert to and from other formats. Any size of image can be
  processed.

  This code is written using an emacs folding mode, making moving
  around the different sections very easy. This is why there are
  various marks within comments and why comments are indented.


  SUSAN QUICK:

  This version of the SUSAN corner finder does not do all the
  false-corner suppression and thus is faster and produced some false
  positives, particularly on strong edges. However, because there are
  less stages involving thresholds etc., the corners that are
  correctly reported are usually more stable than those reported with
  the full algorithm. Thus I recommend at least TRYING this algorithm
  for applications where stability is important, e.g., tracking.

  THRESHOLDS:

  There are two thresholds which can be set at run-time. These are the
  brightness threshold (t) and the distance threshold (d).

  SPATIAL CONTROL: d

  In SUSAN smoothing d controls the size of the Gaussian mask; its
  default is 4.0. Increasing d gives more smoothing. In edge finding,
  a fixed flat mask is used, either 37 pixels arranged in a "circle"
  (default), or a 3 by 3 mask which gives finer detail. In corner
  finding, only the larger 37 pixel mask is used; d is not
  variable. In smoothing, the flat 3 by 3 mask can be used instead of
  a larger Gaussian mask; this gives low smoothing and fast operation.

  BRIGHTNESS CONTROL: t

  In all three algorithms, t can be varied (default=20); this is the
  main threshold to be varied. It determines the maximum difference in
  greylevels between two pixels which allows them to be considered
  part of the same "region" in the image. Thus it can be reduced to
  give more edges or corners, i.e. to be more sensitive, and vice
  versa. In smoothing, reducing t gives less smoothing, and vice
  versa. Set t=10 for the test image available from the SUSAN web
  page.

  ITERATIONS:

  With SUSAN smoothing, more smoothing can also be obtained by
  iterating the algorithm several times. This has a different effect
  from varying d or t.

  FIXED MASKS:

  37 pixel mask:    ooo       3 by 3 mask:  ooo
                   ooooo                    ooo
                  ooooooo                   ooo
                  ooooooo
                  ooooooo
                   ooooo
                    ooo

  CORNER ATTRIBUTES dx, dy and I
  (Only read this if you are interested in the C implementation or in
  using corner attributes, e.g., for corner matching)

  Corners reported in the corner list have attributes associated with
  them as well as positions. This is useful, for example, when
  attempting to match corners from one image to another, as these
  attributes can often be fairly unchanged between images. The
  attributes are dx, dy and I. I is the value of image brightness at
  the position of the corner. In the case of susan_corners_quick, dx
  and dy are the first order derivatives (differentials) of the image
  brightness in the x and y directions respectively, at the position
  of the corner. In the case of normal susan corner finding, dx and dy
  are scaled versions of the position of the centre of gravity of the
  USAN with respect to the centre pixel (nucleus).

  BRIGHTNESS FUNCTION LUT IMPLEMENTATION:
  (Only read this if you are interested in the C implementation)

  The SUSAN brightness function is implemented as a LUT
  (Look-Up-Table) for speed. The resulting pointer-based code is a
  little hard to follow, so here is a brief explanation. In
  setup_brightness_lut() the LUT is setup. This mallocs enough space
  for *bp and then repositions the pointer to the centre of the
  malloced space. The SUSAN function e^-(x^6) or e^-(x^2) is
  calculated and converted to a uchar in the range 0-100, for all
  possible image brightness differences (including negative
  ones). Thus bp[23] is the output for a brightness difference of 23
  greylevels. In the SUSAN algorithms this LUT is used as follows:

  p=in + (i-3)*x_size + j - 1;
  p points to the first image pixel in the circular mask surrounding
  point (x,y).

  cp=bp + in[i*x_size+j];
  cp points to a position in the LUT corresponding to the brightness
  of the centre pixel (x,y).

  now for every pixel within the mask surrounding (x,y),
  n+=*(cp-*p++);
  the brightness difference function is found by moving the cp pointer
  down by an amount equal to the value of the pixel pointed to by p,
  thus subtracting the two brightness values and performing the
  exponential function. This value is added to n, the running USAN
  area.

  in SUSAN smoothing, the variable height mask is implemented by
  multiplying the above by the moving mask pointer, reset for each new
  centre pixel.
  tmp = *dpt++ * *(cp-brightness);

\**********************************************************************/

/* }}} */
/* {{{ Machine Information */

/**********************************************************************\

  Success has been reported with the following:

  MACHINE  OS         COMPILER

  Sun      4.1.4      bundled C, gcc

  Next

  SGI      IRIX       SGI cc

  DEC      Unix V3.2+ 

  IBM RISC AIX        gcc

  PC                  Borland 5.0

  PC       Linux      gcc-2.6.3

  PC       Win32      Visual C++ 4.0 (Console Application)

  PC       Win95      Visual C++ 5.0 (Console Application)
                      Thanks to Niu Yongsheng <niuysbit@163.net>:
                      Use the FOPENB option below

  PC       DOS        djgpp gnu C
                      Thanks to Mark Pettovello <mpettove@umdsun2.umd.umich.edu>:
                      Use the FOPENB option below

  HP       HP-UX      bundled cc
                      Thanks to Brian Dixon <briand@hpcvsgen.cv.hp.com>:
                      in ksh:
                      export CCOPTS="-Aa -D_HPUX_SOURCE | -lM"
                      cc -O3 -o susan susan2l.c

\**********************************************************************/

/* }}} */
/* {{{ History */

/**********************************************************************\

  SUSAN Version 2l, 12/2/99
  Changed GNUDOS option to FOPENB.
  (Thanks to Niu Yongsheng <niuysbit@163.net>.)
  Took out redundant "sq=sq/2;".

  SUSAN Version 2k, 19/8/98:
  In corner finding:
  Changed if(yy<sq) {...} else if(xx<sq) {...} to
          if(yy<xx) {...} else {...}
  (Thanks to adq@cim.mcgill.edu - Alain Domercq.)

  SUSAN Version 2j, 22/10/97:
  Fixed (mask_size>x_size) etc. tests in smoothing.
  Added a couple of free() calls for cgx and cgy.
  (Thanks to geoffb@ucs.ed.ac.uk - Geoff Browitt.)

  SUSAN Version 2i, 21/7/97:
  Added information about corner attributes.

  SUSAN Version 2h, 16/12/96:
  Added principle (initial enhancement) option.

  SUSAN Version 2g, 2/7/96:
  Minor superficial changes to code.

  SUSAN Version 2f, 16/1/96:
  Added GNUDOS option (now called FOPENB; see options below).

  SUSAN Version 2e, 9/1/96:
  Added -b option.
  Fixed 1 pixel horizontal offset error for drawing edges.

  SUSAN Version 2d, 27/11/95:
  Fixed loading of certain PGM files in get_image (again!)

  SUSAN Version 2c, 22/11/95:
  Fixed loading of certain PGM files in get_image.
  (Thanks to qu@San-Jose.ate.slb.com - Gongyuan Qu.)

  SUSAN Version 2b, 9/11/95:
  removed "z==" error in edges routines.

  SUSAN Version 2a, 6/11/95:
  Removed a few unnecessary variable declarations.
  Added different machine information.
  Changed "header" in get_image to char.

  SUSAN Version 2, 1/11/95: first combined version able to take any
  image sizes.

  SUSAN "Versions 1", circa 1992: the various SUSAN algorithms were
  developed during my doctorate within different programs and for
  fixed image sizes. The algorithms themselves are virtually unaltered
  between "versions 1" and the combined program, version 2.

\**********************************************************************/

/* }}} */
/* {{{ defines, includes and typedefs */

/* ********** Optional settings */

#ifndef PPC
typedef int        TOTAL_TYPE; /* this is faster for "int" but should be "float" for large d masks */
#else
typedef float      TOTAL_TYPE; /* for my PowerPC accelerator only */
#endif

/*#define FOPENB*/           /* uncomment if using djgpp gnu C for DOS or certain Win95 compilers */
#define SEVEN_SUPP           /* size for non-max corner suppression; SEVEN_SUPP or FIVE_SUPP */
#define MAX_CORNERS   15000  /* max corners per frame */

/* ********** Leave the rest - but you may need to remove one or both of sys/file.h and malloc.h lines */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/file.h>    /* may want to remove this line */
#include <malloc.h>      /* may want to remove this line */
#define  exit_error(IFB,IFC) { fprintf(stderr,IFB,IFC); exit(0); }
#define  FTOI(a) ( (a) < 0 ? ((int)(a-0.5)) : ((int)(a+0.5)) )
typedef  unsigned char uchar;
typedef  struct {int x,y,info, dx, dy, I;} CORNER_LIST[MAX_CORNERS];

/* }}} */
/* {{{ usage() */

usage()
{
  printf("Usage: susan <in.pgm> <out.pgm> [options]\n\n");

  printf("-s : Smoothing mode (default)\n");
  printf("-e : Edges mode\n");
  printf("-c : Corners mode\n\n");

  printf("See source code for more information about setting the thresholds\n");
  printf("-t <thresh> : Brightness threshold, all modes (default=20)\n");
  printf("-d <thresh> : Distance threshold, smoothing mode, (default=4) (use next option instead for flat 3x3 mask)\n");
  printf("-3 : Use flat 3x3 mask, edges or smoothing mode\n");
  printf("-n : No post-processing on the binary edge map (runs much faster); edges mode\n");
  printf("-q : Use faster (and usually stabler) corner mode; edge-like corner suppression not carried out; corners mode\n");
  printf("-b : Mark corners/edges with single black points instead of black with white border; corners or edges mode\n");
  printf("-p : Output initial enhancement image only; corners or edges mode (default is edges mode)\n");

  printf("\nSUSAN Version 2l (C) 1995-1997 Stephen Smith, DRA UK. steve@fmrib.ox.ac.uk\n");

  exit(0);
}

/* }}} */
/* {{{ get_image(filename,in,x_size,y_size) */

/* {{{ int getint(fp) derived from XV */

int getint(fd)
  FILE *fd;
{
  int c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));
int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  char dummy[10000];
__boundcheck_metadata_store((void *)(&dummy),(void *)((size_t)(&dummy)+sizeof(dummy)*8-1));


  c = getc(fd);
  while (1) /* find next integer */
  {
    if (c=='#')    /* if we're at a comment, read to end of line */
      fgets(dummy,9000,fd);
    if (c==EOF)
      exit_error("Image %s not binary PGM.\n","is");
    if (c>='0' && c<='9')
      break;   /* found what we were looking for */
    c = getc(fd);
  }

  /* we're at the start of a number, continue until we hit a non-number */
  i = 0;
  while (1) {
    i = (i*10) + (c - '0');
    c = getc(fd);
    if (c==EOF) return (i);
    if (c<'0' || c>'9') break;
  }

  return (i);
}

/* }}} */

void get_image(filename,in,x_size,y_size)
  char           filename[200];
  unsigned char  **in;
  int            *x_size, *y_size;
{
FILE  *fd;
__boundcheck_metadata_store((void *)(&fd),(void *)((size_t)(&fd)+sizeof(fd)*8-1));

char header [100];
__boundcheck_metadata_store((void *)(&header),(void *)((size_t)(&header)+sizeof(header)*8-1));

int  tmp;
__boundcheck_metadata_store((void *)(&tmp),(void *)((size_t)(&tmp)+sizeof(tmp)*8-1));


#ifdef FOPENB
  if ((fd=fopen(filename,"rb")) == NULL)
#else
  if ((fd=fopen(filename,"r")) == NULL)
#endif
    exit_error("Can't input image %s.\n",filename);

  /* {{{ read header */

  header[_RV_insert_check(0,100,391,10,"get_image",0)]=fgetc(fd);
  header[_RV_insert_check(0,100,392,10,"get_image",1)]=fgetc(fd);
  if(!(header[_RV_insert_check(0,100,393,15,"get_image",0)]=='P' && header[_RV_insert_check(0,100,393,33,"get_image",1)]=='5'))
    exit_error("Image %s does not have binary PGM header.\n",filename);

  *(int *)(__boundcheck_ptr_reference(396,4,"get_image",(void *)(x_size),(void *)(x_size))) = getint(fd);
  *(int *)(__boundcheck_ptr_reference(397,4,"get_image",(void *)(y_size),(void *)(y_size))) = getint(fd);
  tmp = getint(fd);

/* }}} */

  *(unsigned char **)(__boundcheck_ptr_reference(402,4,"get_image",(void *)(in),(void *)(in))) = (uchar *) __boundcheck_malloc(*(int *)(__boundcheck_ptr_reference(402,27,"get_image",(void *)(x_size),(void *)(x_size))) * *(int *)(__boundcheck_ptr_reference(402,37,"get_image",(void *)(y_size),(void *)(y_size))));

  if (fread(*(unsigned char **)(__boundcheck_ptr_reference(404,14,"get_image",(void *)(in),(void *)(in))),1,*(int *)(__boundcheck_ptr_reference(404,20,"get_image",(void *)(x_size),(void *)(x_size))) * *(int *)(__boundcheck_ptr_reference(404,30,"get_image",(void *)(y_size),(void *)(y_size))),fd) == 0)
    exit_error("Image %s is wrong size.\n",filename);

  fclose(fd);
}

/* }}} */
/* {{{ put_image(filename,in,x_size,y_size) */

put_image(filename,in,x_size,y_size)
  char filename [100],
       *in;
  int  x_size,
       y_size;
{
FILE  *fd;
__boundcheck_metadata_store((void *)(&fd),(void *)((size_t)(&fd)+sizeof(fd)*8-1));


#ifdef FOPENB
  if ((fd=fopen(filename,"wb")) == NULL) 
#else
  if ((fd=fopen(filename,"w")) == NULL) 
#endif
    exit_error("Can't output image%s.\n",filename);

  fprintf(fd,"P5\n");
  fprintf(fd,"%d %d\n",x_size,y_size);
  fprintf(fd,"255\n");
  
  if (fwrite(in,x_size*y_size,1,fd) != 1)
    exit_error("Can't write image %s.\n",filename);

  fclose(fd);
}

/* }}} */
/* {{{ int_to_uchar(r,in,size) */

int_to_uchar(r,in,size)
  uchar *in;
  int   *r, size;
{
int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

   int max_r=r[__boundcheck_ptr_cast_to_array_reference(446,13,"int_to_uchar",(void *)(r),(void *)(r+0),0)];
__boundcheck_metadata_store((void *)(&max_r),(void *)((size_t)(&max_r)+sizeof(max_r)*8-1));

   int min_r=r[__boundcheck_ptr_cast_to_array_reference(447,13,"int_to_uchar",(void *)(r),(void *)(r+0),0)];
__boundcheck_metadata_store((void *)(&min_r),(void *)((size_t)(&min_r)+sizeof(min_r)*8-1));


  for (i=0; i<size; i++)
    {
      if ( r[__boundcheck_ptr_cast_to_array_reference(451,14,"int_to_uchar",(void *)(r),(void *)(r+i),i)] > max_r )
        max_r=r[__boundcheck_ptr_cast_to_array_reference(452,17,"int_to_uchar",(void *)(r),(void *)(r+i),i)];
      if ( r[__boundcheck_ptr_cast_to_array_reference(453,14,"int_to_uchar",(void *)(r),(void *)(r+i),i)] < min_r )
        min_r=r[__boundcheck_ptr_cast_to_array_reference(454,17,"int_to_uchar",(void *)(r),(void *)(r+i),i)];
    }

  /*printf("min=%d max=%d\n",min_r,max_r);*/

  max_r-=min_r;

  for (i=0; i<size; i++)
    in[__boundcheck_ptr_cast_to_array_reference(462,8,"int_to_uchar",(void *)(in),(void *)(in+i),i)] = (uchar)((int)((int)(r[__boundcheck_ptr_cast_to_array_reference(462,35,"int_to_uchar",(void *)(r),(void *)(r+i),i)]-min_r)*255)/max_r);
}

/* }}} */
/* {{{ setup_brightness_lut(bp,thresh,form) */

void setup_brightness_lut(bp,thresh,form)
  uchar **bp;
  int   thresh, form;
{
int   k;
__boundcheck_metadata_store((void *)(&k),(void *)((size_t)(&k)+sizeof(k)*8-1));

float temp;
__boundcheck_metadata_store((void *)(&temp),(void *)((size_t)(&temp)+sizeof(temp)*8-1));


  *(uchar **)(__boundcheck_ptr_reference(475,4,"setup_brightness_lut",(void *)(bp),(void *)(bp)))=(unsigned char *)__boundcheck_malloc(516);
  *(uchar **)(__boundcheck_ptr_reference(476,4,"setup_brightness_lut",(void *)(bp),(void *)(bp)))=*(uchar **)(__boundcheck_ptr_reference(476,8,"setup_brightness_lut",(void *)(bp),(void *)(bp)))+258;

  for(k=-256;k<257;k++)
  {
    temp=((float)k)/((float)thresh);
    temp=temp*temp;
    if (form==6)
      temp=temp*temp*temp;
    temp=100.0*exp(-temp);
    *(uchar *)(__boundcheck_ptr_reference(485,6,"setup_brightness_lut",(void *)(*bp),(void *)(*(uchar **)(__boundcheck_ptr_reference(485,8,"setup_brightness_lut",(void *)(bp),(void *)(bp)))+k)))= (uchar)temp;
  }
}

/* }}} */
/* {{{ susan principle */

/* {{{ susan_principle(in,r,bp,max_no,x_size,y_size) */

susan_principle(in,r,bp,max_no,x_size,y_size)
  uchar *in, *bp;
  int   *r, max_no, x_size, y_size;
{
int   i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));

uchar *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
uchar * cp;
__boundcheck_metadata_store((void *)(&cp),(void *)((size_t)(&cp)+sizeof(cp)*8-1));


  memset (r,0,x_size * y_size * sizeof(int));

  for (i=3;i<y_size-3;i++)
    for (j=3;j<x_size-3;j++)
    {
      n=100;
      p=in + (i-3)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 3) * x_size));

      cp=bp + in[__boundcheck_ptr_cast_to_array_reference(508,18,"susan_principle",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + in[i * x_size + j]));


      n+=*(uchar *)(__boundcheck_ptr_reference(510,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(510,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(511,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(511,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(512,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(512,16,"susan_principle",(void *)(p),(void *)(p)))))));
      p+=x_size-3; 

      n+=*(uchar *)(__boundcheck_ptr_reference(515,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(515,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(516,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(516,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(517,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(517,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(518,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(518,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(519,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(519,16,"susan_principle",(void *)(p),(void *)(p)))))));
      p+=x_size-5;

      n+=*(uchar *)(__boundcheck_ptr_reference(522,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(522,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(523,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(523,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(524,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(524,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(525,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(525,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(526,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(526,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(527,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(527,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(528,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(528,16,"susan_principle",(void *)(p),(void *)(p)))))));
      p+=x_size-6;

      n+=*(uchar *)(__boundcheck_ptr_reference(531,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(531,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(532,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(532,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(533,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(533,16,"susan_principle",(void *)(p),(void *)(p)))))));
      p+=2;
      n+=*(uchar *)(__boundcheck_ptr_reference(535,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(535,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(536,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(536,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(537,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(537,16,"susan_principle",(void *)(p),(void *)(p)))))));
      p+=x_size-6;

      n+=*(uchar *)(__boundcheck_ptr_reference(540,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(540,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(541,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(541,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(542,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(542,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(543,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(543,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(544,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(544,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(545,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(545,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(546,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(546,16,"susan_principle",(void *)(p),(void *)(p)))))));
      p+=x_size-5;

      n+=*(uchar *)(__boundcheck_ptr_reference(549,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(549,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(550,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(550,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(551,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(551,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(552,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(552,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(553,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(553,16,"susan_principle",(void *)(p),(void *)(p)))))));
      p+=x_size-3;

      n+=*(uchar *)(__boundcheck_ptr_reference(556,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(556,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(557,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(557,16,"susan_principle",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(558,11,"susan_principle",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(558,16,"susan_principle",(void *)(p),(void *)(p)))))));

      if (n<=max_no)
        r[__boundcheck_ptr_cast_to_array_reference(561,11,"susan_principle",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)] = max_no - n;
    }
}

/* }}} */
/* {{{ susan_principle_small(in,r,bp,max_no,x_size,y_size) */

susan_principle_small(in,r,bp,max_no,x_size,y_size)
  uchar *in, *bp;
  int   *r, max_no, x_size, y_size;
{
int   i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));

uchar *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
uchar * cp;
__boundcheck_metadata_store((void *)(&cp),(void *)((size_t)(&cp)+sizeof(cp)*8-1));


  memset (r,0,x_size * y_size * sizeof(int));

  max_no = 730; /* ho hum ;) */

  for (i=1;i<y_size-1;i++)
    for (j=1;j<x_size-1;j++)
    {
      n=100;
      p=in + (i-1)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 1) * x_size));

      cp=bp + in[__boundcheck_ptr_cast_to_array_reference(584,18,"susan_principle_small",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + in[i * x_size + j]));


      n+=*(uchar *)(__boundcheck_ptr_reference(586,11,"susan_principle_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(586,16,"susan_principle_small",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(587,11,"susan_principle_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(587,16,"susan_principle_small",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(588,11,"susan_principle_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(588,16,"susan_principle_small",(void *)(p),(void *)(p)))))));
      p+=x_size-2; 

      n+=*(uchar *)(__boundcheck_ptr_reference(591,11,"susan_principle_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(591,16,"susan_principle_small",(void *)(p),(void *)(p)))))));
      p+=2;
      n+=*(uchar *)(__boundcheck_ptr_reference(593,11,"susan_principle_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(593,16,"susan_principle_small",(void *)(p),(void *)(p)))))));
      p+=x_size-2;

      n+=*(uchar *)(__boundcheck_ptr_reference(596,11,"susan_principle_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(596,16,"susan_principle_small",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(597,11,"susan_principle_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(597,16,"susan_principle_small",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(598,11,"susan_principle_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(598,16,"susan_principle_small",(void *)(p),(void *)(p)))))));

      if (n<=max_no)
        r[__boundcheck_ptr_cast_to_array_reference(601,11,"susan_principle_small",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)] = max_no - n;
    }
}

/* }}} */

/* }}} */
/* {{{ smoothing */

/* {{{ median(in,i,j,x_size) */

uchar median(in,i,j,x_size)
  uchar *in;
  int   i, j, x_size;
{
int p[8];
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
int k;
__boundcheck_metadata_store((void *)(&k),(void *)((size_t)(&k)+sizeof(k)*8-1));
int l;
__boundcheck_metadata_store((void *)(&l),(void *)((size_t)(&l)+sizeof(l)*8-1));
int tmp;
__boundcheck_metadata_store((void *)(&tmp),(void *)((size_t)(&tmp)+sizeof(tmp)*8-1));


  p[_RV_insert_check(0,8,618,5,"median",0)]=in[__boundcheck_ptr_cast_to_array_reference(618,11,"median",(void *)(in),(void *)(in+(i-1)*x_size+j-1),(i-1)*x_size+j-1)];
  p[_RV_insert_check(0,8,619,5,"median",1)]=in[__boundcheck_ptr_cast_to_array_reference(619,11,"median",(void *)(in),(void *)(in+(i-1)*x_size+j ),(i-1)*x_size+j  )];
  p[_RV_insert_check(0,8,620,5,"median",2)]=in[__boundcheck_ptr_cast_to_array_reference(620,11,"median",(void *)(in),(void *)(in+(i-1)*x_size+j+1),(i-1)*x_size+j+1)];
  p[_RV_insert_check(0,8,621,5,"median",3)]=in[__boundcheck_ptr_cast_to_array_reference(621,11,"median",(void *)(in),(void *)(in+(i  )*x_size+j-1),(i  )*x_size+j-1)];
  p[_RV_insert_check(0,8,622,5,"median",4)]=in[__boundcheck_ptr_cast_to_array_reference(622,11,"median",(void *)(in),(void *)(in+(i  )*x_size+j+1),(i  )*x_size+j+1)];
  p[_RV_insert_check(0,8,623,5,"median",5)]=in[__boundcheck_ptr_cast_to_array_reference(623,11,"median",(void *)(in),(void *)(in+(i+1)*x_size+j-1),(i+1)*x_size+j-1)];
  p[_RV_insert_check(0,8,624,5,"median",6)]=in[__boundcheck_ptr_cast_to_array_reference(624,11,"median",(void *)(in),(void *)(in+(i+1)*x_size+j ),(i+1)*x_size+j  )];
  p[_RV_insert_check(0,8,625,5,"median",7)]=in[__boundcheck_ptr_cast_to_array_reference(625,11,"median",(void *)(in),(void *)(in+(i+1)*x_size+j+1),(i+1)*x_size+j+1)];

  for(k=0; k<7; k++)
    for(l=0; l<(7-k); l++)
      if (p[_RV_insert_check(0,8,629,13,"median",l)]>p[_RV_insert_check(0,8,629,18,"median",l+1)])
      {
        tmp=p[_RV_insert_check(0,8,631,15,"median",l)]; p[_RV_insert_check(0,8,631,21,"median",l)]=p[_RV_insert_check(0,8,631,26,"median",l+1)]; p[_RV_insert_check(0,8,631,34,"median",l+1)]=tmp;
      }

  return( (p[_RV_insert_check(0,8,634,14,"median",3)]+p[_RV_insert_check(0,8,634,19,"median",4)]) / 2 );
}

/* }}} */
/* {{{ enlarge(in,tmp_image,x_size,y_size,border) */

/* this enlarges "in" so that borders can be dealt with easily */

enlarge(in,tmp_image,x_size,y_size,border)
  uchar **in;
  uchar *tmp_image;
  int   *x_size, *y_size, border;
{
int   i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));


  for(i=0; i<*(int *)(__boundcheck_ptr_reference(649,15,"enlarge",(void *)(y_size),(void *)(y_size))); i++)   /* copy *in into tmp_image */
    memcpy(tmp_image+(i+border)*(*(int *)(__boundcheck_ptr_reference(650,35,"enlarge",(void *)(x_size),(void *)(x_size)))+2*border)+border, *(uchar **)(__boundcheck_ptr_reference(650,61,"enlarge",(void *)(in),(void *)(in)))+i* *(int *)(__boundcheck_ptr_reference(650,68,"enlarge",(void *)(x_size),(void *)(x_size))), *(int *)(__boundcheck_ptr_reference(650,77,"enlarge",(void *)(x_size),(void *)(x_size))));

  for(i=0; i<border; i++) /* copy top and bottom rows; invert as many as necessary */
  {
    memcpy(tmp_image+(border-1-i)*(*(int *)(__boundcheck_ptr_reference(654,37,"enlarge",(void *)(x_size),(void *)(x_size)))+2*border)+border,*(uchar **)(__boundcheck_ptr_reference(654,62,"enlarge",(void *)(in),(void *)(in)))+i* *(int *)(__boundcheck_ptr_reference(654,69,"enlarge",(void *)(x_size),(void *)(x_size))),*(int *)(__boundcheck_ptr_reference(654,77,"enlarge",(void *)(x_size),(void *)(x_size))));
    memcpy(tmp_image+(*(int *)(__boundcheck_ptr_reference(655,24,"enlarge",(void *)(y_size),(void *)(y_size)))+border+i)*(*(int *)(__boundcheck_ptr_reference(655,43,"enlarge",(void *)(x_size),(void *)(x_size)))+2*border)+border,*(uchar **)(__boundcheck_ptr_reference(655,68,"enlarge",(void *)(in),(void *)(in)))+(*(int *)(__boundcheck_ptr_reference(655,73,"enlarge",(void *)(y_size),(void *)(y_size)))-i-1)* *(int *)(__boundcheck_ptr_reference(655,87,"enlarge",(void *)(x_size),(void *)(x_size))),*(int *)(__boundcheck_ptr_reference(655,95,"enlarge",(void *)(x_size),(void *)(x_size))));
  }

  for(i=0; i<border; i++) /* copy left and right columns */
    for(j=0; j<*(int *)(__boundcheck_ptr_reference(659,17,"enlarge",(void *)(y_size),(void *)(y_size)))+2*border; j++)
    {
      tmp_image[__boundcheck_ptr_cast_to_array_reference(661,17,"enlarge",(void *)(tmp_image),(void *)(tmp_image+j*(*x_size+2*border)+border-1-i),j*(*(int *)(__boundcheck_ptr_reference(661,21,"enlarge",(void *)(x_size),(void *)(x_size)))+2*border)+border-1-i)]=tmp_image[__boundcheck_ptr_cast_to_array_reference(661,60,"enlarge",(void *)(tmp_image),(void *)(tmp_image+j*(*x_size+2*border)+border+i),j*(*(int *)(__boundcheck_ptr_reference(661,64,"enlarge",(void *)(x_size),(void *)(x_size)))+2*border)+border+i)];
      tmp_image[__boundcheck_ptr_cast_to_array_reference(662,17,"enlarge",(void *)(tmp_image),(void *)(tmp_image+j*(*x_size+2*border)+ *x_size+border+i),j*(*(int *)(__boundcheck_ptr_reference(662,21,"enlarge",(void *)(x_size),(void *)(x_size)))+2*border)+ *(int *)(__boundcheck_ptr_reference(662,40,"enlarge",(void *)(x_size),(void *)(x_size)))+border+i)]=tmp_image[__boundcheck_ptr_cast_to_array_reference(662,67,"enlarge",(void *)(tmp_image),(void *)(tmp_image+j*(*x_size+2*border)+ *x_size+border-1-i),j*(*(int *)(__boundcheck_ptr_reference(662,71,"enlarge",(void *)(x_size),(void *)(x_size)))+2*border)+ *(int *)(__boundcheck_ptr_reference(662,90,"enlarge",(void *)(x_size),(void *)(x_size)))+border-1-i)];
    }

  *(int *)(__boundcheck_ptr_reference(665,4,"enlarge",(void *)(x_size),(void *)(x_size)))+=2*border;  /* alter image size */
  *(int *)(__boundcheck_ptr_reference(666,4,"enlarge",(void *)(y_size),(void *)(y_size)))+=2*border;
  *(uchar **)(__boundcheck_ptr_reference(667,4,"enlarge",(void *)(in),(void *)(in)))=tmp_image;      /* repoint in */
}

/* }}} */
/* {{{ void susan_smoothing(three_by_three,in,dt,x_size,y_size,bp) */

void susan_smoothing(three_by_three,in,dt,x_size,y_size,bp)
  int   three_by_three, x_size, y_size;
  uchar *in, *bp;
  float dt;
{
/* {{{ vars */

float temp;
__boundcheck_metadata_store((void *)(&temp),(void *)((size_t)(&temp)+sizeof(temp)*8-1));

int   n_max;
__boundcheck_metadata_store((void *)(&n_max),(void *)((size_t)(&n_max)+sizeof(n_max)*8-1));
int increment;
__boundcheck_metadata_store((void *)(&increment),(void *)((size_t)(&increment)+sizeof(increment)*8-1));
int mask_size;
__boundcheck_metadata_store((void *)(&mask_size),(void *)((size_t)(&mask_size)+sizeof(mask_size)*8-1));

     int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));
int y;
__boundcheck_metadata_store((void *)(&y),(void *)((size_t)(&y)+sizeof(y)*8-1));
int area;
__boundcheck_metadata_store((void *)(&area),(void *)((size_t)(&area)+sizeof(area)*8-1));
int brightness;
__boundcheck_metadata_store((void *)(&brightness),(void *)((size_t)(&brightness)+sizeof(brightness)*8-1));
int tmp;
__boundcheck_metadata_store((void *)(&tmp),(void *)((size_t)(&tmp)+sizeof(tmp)*8-1));
int centre;
__boundcheck_metadata_store((void *)(&centre),(void *)((size_t)(&centre)+sizeof(centre)*8-1));

uchar *ip;
__boundcheck_metadata_store((void *)(&ip),(void *)((size_t)(&ip)+sizeof(ip)*8-1));
 uchar * dp;
__boundcheck_metadata_store((void *)(&dp),(void *)((size_t)(&dp)+sizeof(dp)*8-1));
 uchar * dpt;
__boundcheck_metadata_store((void *)(&dpt),(void *)((size_t)(&dpt)+sizeof(dpt)*8-1));
 uchar * cp;
__boundcheck_metadata_store((void *)(&cp),(void *)((size_t)(&cp)+sizeof(cp)*8-1));
 uchar * out=in;
__boundcheck_metadata_store((void *)(&out),(void *)((size_t)(&out)+sizeof(out)*8-1));

      uchar * tmp_image;
__boundcheck_metadata_store((void *)(&tmp_image),(void *)((size_t)(&tmp_image)+sizeof(tmp_image)*8-1));

TOTAL_TYPE total;
__boundcheck_metadata_store((void *)(&total),(void *)((size_t)(&total)+sizeof(total)*8-1));


/* }}} */

  /* {{{ setup larger image and border sizes */

  if (three_by_three==0)
    mask_size = ((int)(1.5 * dt)) + 1;
  else
    mask_size = 1;

  total=0.1; /* test for total's type */
  if ( (dt>15) && (total==0) )
  {
    printf("Distance_thresh (%f) too big for integer arithmetic.\n",dt);
    printf("Either reduce it to <=15 or recompile with variable \"total\"\n");
    printf("as a float: see top \"defines\" section.\n");
    exit(0);
  }

  if ( (2*mask_size+1>x_size) || (2*mask_size+1>y_size) )
  {
    printf("Mask size (1.5*distance_thresh+1=%d) too big for image (%dx%d).\n",mask_size,x_size,y_size);
    exit(0);
  }

  tmp_image = (uchar *) __boundcheck_malloc( (x_size+mask_size*2) * (y_size+mask_size*2) );
  enlarge(&in,tmp_image,&x_size,&y_size,mask_size);

/* }}} */

  if (three_by_three==0)
  {     /* large Gaussian masks */
    /* {{{ setup distance lut */

  n_max = (mask_size*2) + 1;

  increment = x_size - n_max;

  dp     = (unsigned char *)__boundcheck_malloc(n_max*n_max);
  dpt    = dp;
__boundcheck_metadata_trans_check((void *)(dpt),(void *)(dp),(void *)(dp));

  temp   = -(dt*dt);

  for(i=-mask_size; i<=mask_size; i++)
    for(j=-mask_size; j<=mask_size; j++)
    {
      x = (int) (100.0 * exp( ((float)((i*i)+(j*j))) / temp ));
      *(uchar *)(__boundcheck_ptr_reference(732,8,"susan_smoothing",(void *)(dpt),(void *)(dpt++))) = (unsigned char)x;
    }

/* }}} */
    /* {{{ main section */

  for (i=mask_size;i<y_size-mask_size;i++)
  {
    for (j=mask_size;j<x_size-mask_size;j++)
    {
      area = 0;
      total = 0;
      dpt = dp;
__boundcheck_metadata_trans_check((void *)(dpt),(void *)(dp),(void *)(dp));

      ip = in + ((i-mask_size)*x_size) + j - mask_size;
__boundcheck_metadata_trans_check((void *)(ip),(void *)(in),(void *)(in + ((i - mask_size) * x_size)));

      centre = in[__boundcheck_ptr_cast_to_array_reference(746,19,"susan_smoothing",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
      cp = bp + centre;
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + centre));

      for(y=-mask_size; y<=mask_size; y++)
      {
        for(x=-mask_size; x<=mask_size; x++)
	{
          brightness = *(uchar *)(__boundcheck_ptr_reference(752,25,"susan_smoothing",(void *)(ip),(void *)(ip++)));
          tmp = *(uchar *)(__boundcheck_ptr_reference(753,18,"susan_smoothing",(void *)(dpt),(void *)(dpt++))) * *(uchar *)(__boundcheck_ptr_reference(753,27,"susan_smoothing",(void *)(cp),(void *)((cp-brightness))));
          area += tmp;
          total += tmp * brightness;
        }
        ip += increment;
      }
      tmp = area-10000;
      if (tmp==0)
        *(uchar *)(__boundcheck_ptr_reference(761,10,"susan_smoothing",(void *)(out),(void *)(out++)))=median(in,i,j,x_size);
      else
        *(uchar *)(__boundcheck_ptr_reference(763,10,"susan_smoothing",(void *)(out),(void *)(out++)))=((total-(centre*10000))/tmp);
    }
  }

/* }}} */
  }
  else
  {     /* 3x3 constant mask */
    /* {{{ main section */

  for (i=1;i<y_size-1;i++)
  {
    for (j=1;j<x_size-1;j++)
    {
      area = 0;
      total = 0;
      ip = in + ((i-1)*x_size) + j - 1;
__boundcheck_metadata_trans_check((void *)(ip),(void *)(in),(void *)(in + ((i - 1) * x_size)));

      centre = in[__boundcheck_ptr_cast_to_array_reference(780,19,"susan_smoothing",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
      cp = bp + centre;
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + centre));


      brightness=*(uchar *)(__boundcheck_ptr_reference(783,19,"susan_smoothing",(void *)(ip),(void *)(ip++))); tmp=*(uchar *)(__boundcheck_ptr_reference(783,30,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;
      brightness=*(uchar *)(__boundcheck_ptr_reference(784,19,"susan_smoothing",(void *)(ip),(void *)(ip++))); tmp=*(uchar *)(__boundcheck_ptr_reference(784,30,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;
      brightness=*(uchar *)(__boundcheck_ptr_reference(785,19,"susan_smoothing",(void *)(ip),(void *)(ip))); tmp=*(uchar *)(__boundcheck_ptr_reference(785,28,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;
      ip += x_size-2;
      brightness=*(uchar *)(__boundcheck_ptr_reference(787,19,"susan_smoothing",(void *)(ip),(void *)(ip++))); tmp=*(uchar *)(__boundcheck_ptr_reference(787,30,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;
      brightness=*(uchar *)(__boundcheck_ptr_reference(788,19,"susan_smoothing",(void *)(ip),(void *)(ip++))); tmp=*(uchar *)(__boundcheck_ptr_reference(788,30,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;
      brightness=*(uchar *)(__boundcheck_ptr_reference(789,19,"susan_smoothing",(void *)(ip),(void *)(ip))); tmp=*(uchar *)(__boundcheck_ptr_reference(789,28,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;
      ip += x_size-2;
      brightness=*(uchar *)(__boundcheck_ptr_reference(791,19,"susan_smoothing",(void *)(ip),(void *)(ip++))); tmp=*(uchar *)(__boundcheck_ptr_reference(791,30,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;
      brightness=*(uchar *)(__boundcheck_ptr_reference(792,19,"susan_smoothing",(void *)(ip),(void *)(ip++))); tmp=*(uchar *)(__boundcheck_ptr_reference(792,30,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;
      brightness=*(uchar *)(__boundcheck_ptr_reference(793,19,"susan_smoothing",(void *)(ip),(void *)(ip))); tmp=*(uchar *)(__boundcheck_ptr_reference(793,28,"susan_smoothing",(void *)(cp),(void *)((cp-brightness)))); area += tmp; total += tmp * brightness;

      tmp = area-100;
      if (tmp==0)
        *(uchar *)(__boundcheck_ptr_reference(797,10,"susan_smoothing",(void *)(out),(void *)(out++)))=median(in,i,j,x_size);
      else
        *(uchar *)(__boundcheck_ptr_reference(799,10,"susan_smoothing",(void *)(out),(void *)(out++)))=(total-(centre*100))/tmp;
    }
  }

/* }}} */
  }
}

/* }}} */

/* }}} */
/* {{{ edges */

/* {{{ edge_draw(in,corner_list,drawing_mode) */

edge_draw(in,mid,x_size,y_size,drawing_mode)
  uchar *in, *mid;
  int x_size, y_size, drawing_mode;
{
int   i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

uchar *inp;
__boundcheck_metadata_store((void *)(&inp),(void *)((size_t)(&inp)+sizeof(inp)*8-1));
 uchar * midp;
__boundcheck_metadata_store((void *)(&midp),(void *)((size_t)(&midp)+sizeof(midp)*8-1));


  if (drawing_mode==0)
  {
    /* mark 3x3 white block around each edge point */
    midp=mid;
__boundcheck_metadata_trans_check((void *)(midp),(void *)(mid),(void *)(mid));

    for (i=0; i<x_size*y_size; i++)
    {
      if (*(uchar *)(__boundcheck_ptr_reference(827,12,"edge_draw",(void *)(midp),(void *)(midp)))<8) 
      {
        inp = in + (midp - mid) - x_size - 1;
__boundcheck_metadata_trans_check((void *)(inp),(void *)(in),(void *)(in + (midp - mid)));

        *(uchar *)(__boundcheck_ptr_reference(830,10,"edge_draw",(void *)(inp),(void *)(inp++)))=255; *(uchar *)(__boundcheck_ptr_reference(830,22,"edge_draw",(void *)(inp),(void *)(inp++)))=255; *(uchar *)(__boundcheck_ptr_reference(830,34,"edge_draw",(void *)(inp),(void *)(inp)))=255; inp+=x_size-2;
        *(uchar *)(__boundcheck_ptr_reference(831,10,"edge_draw",(void *)(inp),(void *)(inp++)))=255; *(uchar *)(__boundcheck_ptr_reference(831,22,"edge_draw",(void *)(inp),(void *)(inp++)));     *(uchar *)(__boundcheck_ptr_reference(831,34,"edge_draw",(void *)(inp),(void *)(inp)))=255; inp+=x_size-2;
        *(uchar *)(__boundcheck_ptr_reference(832,10,"edge_draw",(void *)(inp),(void *)(inp++)))=255; *(uchar *)(__boundcheck_ptr_reference(832,22,"edge_draw",(void *)(inp),(void *)(inp++)))=255; *(uchar *)(__boundcheck_ptr_reference(832,34,"edge_draw",(void *)(inp),(void *)(inp)))=255;
      }
      midp++;
    }
  }

  /* now mark 1 black pixel at each edge point */
  midp=mid;
__boundcheck_metadata_trans_check((void *)(midp),(void *)(mid),(void *)(mid));

  for (i=0; i<x_size*y_size; i++)
  {
    if (*(uchar *)(__boundcheck_ptr_reference(842,10,"edge_draw",(void *)(midp),(void *)(midp)))<8) 
      *(uchar *)(__boundcheck_ptr_reference(843,8,"edge_draw",(void *)(in),(void *)((in + (midp - mid))))) = 0;
    midp++;
  }
}

/* }}} */
/* {{{ susan_thin(r,mid,x_size,y_size) */

/* only one pass is needed as i,j are decremented if necessary to go
   back and do bits again */

susan_thin(r,mid,x_size,y_size)
  uchar *mid;
  int   *r, x_size, y_size;
{
int   l[9];
__boundcheck_metadata_store((void *)(&l),(void *)((size_t)(&l)+sizeof(l)*8-1));
int centre;
__boundcheck_metadata_store((void *)(&centre),(void *)((size_t)(&centre)+sizeof(centre)*8-1));
int nlinks;
__boundcheck_metadata_store((void *)(&nlinks),(void *)((size_t)(&nlinks)+sizeof(nlinks)*8-1));
int npieces;
__boundcheck_metadata_store((void *)(&npieces),(void *)((size_t)(&npieces)+sizeof(npieces)*8-1));

     int b01;
__boundcheck_metadata_store((void *)(&b01),(void *)((size_t)(&b01)+sizeof(b01)*8-1));
int b12;
__boundcheck_metadata_store((void *)(&b12),(void *)((size_t)(&b12)+sizeof(b12)*8-1));
int b21;
__boundcheck_metadata_store((void *)(&b21),(void *)((size_t)(&b21)+sizeof(b21)*8-1));
int b10;
__boundcheck_metadata_store((void *)(&b10),(void *)((size_t)(&b10)+sizeof(b10)*8-1));

     int p1;
__boundcheck_metadata_store((void *)(&p1),(void *)((size_t)(&p1)+sizeof(p1)*8-1));
int p2;
__boundcheck_metadata_store((void *)(&p2),(void *)((size_t)(&p2)+sizeof(p2)*8-1));
int p3;
__boundcheck_metadata_store((void *)(&p3),(void *)((size_t)(&p3)+sizeof(p3)*8-1));
int p4;
__boundcheck_metadata_store((void *)(&p4),(void *)((size_t)(&p4)+sizeof(p4)*8-1));

     int b00;
__boundcheck_metadata_store((void *)(&b00),(void *)((size_t)(&b00)+sizeof(b00)*8-1));
int b02;
__boundcheck_metadata_store((void *)(&b02),(void *)((size_t)(&b02)+sizeof(b02)*8-1));
int b20;
__boundcheck_metadata_store((void *)(&b20),(void *)((size_t)(&b20)+sizeof(b20)*8-1));
int b22;
__boundcheck_metadata_store((void *)(&b22),(void *)((size_t)(&b22)+sizeof(b22)*8-1));

     int m;
__boundcheck_metadata_store((void *)(&m),(void *)((size_t)(&m)+sizeof(m)*8-1));
int n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));
int a;
__boundcheck_metadata_store((void *)(&a),(void *)((size_t)(&a)+sizeof(a)*8-1));
int b;
__boundcheck_metadata_store((void *)(&b),(void *)((size_t)(&b)+sizeof(b)*8-1));
int x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));
int y;
__boundcheck_metadata_store((void *)(&y),(void *)((size_t)(&y)+sizeof(y)*8-1));
int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));

uchar *mp;
__boundcheck_metadata_store((void *)(&mp),(void *)((size_t)(&mp)+sizeof(mp)*8-1));


  for (i=4;i<y_size-4;i++)
    for (j=4;j<x_size-4;j++)
      if (mid[__boundcheck_ptr_cast_to_array_reference(867,15,"susan_thin",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)]<8)
      {
        centre = r[__boundcheck_ptr_cast_to_array_reference(869,20,"susan_thin",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)];
        /* {{{ count number of neighbours */

        mp=mid + (i-1)*x_size + j-1;
__boundcheck_metadata_trans_check((void *)(mp),(void *)(mid),(void *)(mid + (i - 1) * x_size));


        n = (*(uchar *)(__boundcheck_ptr_reference(874,15,"susan_thin",(void *)(mp),(void *)(mp)))<8) +
            (*(uchar *)(__boundcheck_ptr_reference(875,15,"susan_thin",(void *)(mp),(void *)((mp+1))))<8) +
            (*(uchar *)(__boundcheck_ptr_reference(876,15,"susan_thin",(void *)(mp),(void *)((mp+2))))<8) +
            (*(uchar *)(__boundcheck_ptr_reference(877,15,"susan_thin",(void *)(mp),(void *)((mp+x_size))))<8) +
            (*(uchar *)(__boundcheck_ptr_reference(878,15,"susan_thin",(void *)(mp),(void *)((mp+x_size+2))))<8) +
            (*(uchar *)(__boundcheck_ptr_reference(879,15,"susan_thin",(void *)(mp),(void *)((mp+x_size+x_size))))<8) +
            (*(uchar *)(__boundcheck_ptr_reference(880,15,"susan_thin",(void *)(mp),(void *)((mp+x_size+x_size+1))))<8) +
            (*(uchar *)(__boundcheck_ptr_reference(881,15,"susan_thin",(void *)(mp),(void *)((mp+x_size+x_size+2))))<8);

/* }}} */
        /* {{{ n==0 no neighbours - remove point */

        if (n==0)
          mid[__boundcheck_ptr_cast_to_array_reference(887,15,"susan_thin",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)]=100;

/* }}} */
        /* {{{ n==1 - extend line if I can */

        /* extension is only allowed a few times - the value of mid is used to control this */

        if ( (n==1) && (mid[__boundcheck_ptr_cast_to_array_reference(894,29,"susan_thin",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)]<6) )
        {
          /* find maximum neighbour weighted in direction opposite the
             neighbour already present. e.g.
             have: O O O  weight r by 0 2 3
                   X X O              0 0 4
                   O O O              0 2 3     */

          l[_RV_insert_check(0,9,902,13,"susan_thin",0)]=r[__boundcheck_ptr_cast_to_array_reference(902,18,"susan_thin",(void *)(r),(void *)(r+(i-1)*x_size+j-1),(i-1)*x_size+j-1)]; l[_RV_insert_check(0,9,902,39,"susan_thin",1)]=r[__boundcheck_ptr_cast_to_array_reference(902,44,"susan_thin",(void *)(r),(void *)(r+(i-1)*x_size+j),(i-1)*x_size+j)]; l[_RV_insert_check(0,9,902,63,"susan_thin",2)]=r[__boundcheck_ptr_cast_to_array_reference(902,68,"susan_thin",(void *)(r),(void *)(r+(i-1)*x_size+j+1),(i-1)*x_size+j+1)];
          l[_RV_insert_check(0,9,903,13,"susan_thin",3)]=r[__boundcheck_ptr_cast_to_array_reference(903,18,"susan_thin",(void *)(r),(void *)(r+(i  )*x_size+j-1),(i  )*x_size+j-1)]; l[_RV_insert_check(0,9,903,39,"susan_thin",4)]=0;                 l[_RV_insert_check(0,9,903,63,"susan_thin",5)]=r[__boundcheck_ptr_cast_to_array_reference(903,68,"susan_thin",(void *)(r),(void *)(r+(i  )*x_size+j+1),(i  )*x_size+j+1)];
          l[_RV_insert_check(0,9,904,13,"susan_thin",6)]=r[__boundcheck_ptr_cast_to_array_reference(904,18,"susan_thin",(void *)(r),(void *)(r+(i+1)*x_size+j-1),(i+1)*x_size+j-1)]; l[_RV_insert_check(0,9,904,39,"susan_thin",7)]=r[__boundcheck_ptr_cast_to_array_reference(904,44,"susan_thin",(void *)(r),(void *)(r+(i+1)*x_size+j),(i+1)*x_size+j)]; l[_RV_insert_check(0,9,904,63,"susan_thin",8)]=r[__boundcheck_ptr_cast_to_array_reference(904,68,"susan_thin",(void *)(r),(void *)(r+(i+1)*x_size+j+1),(i+1)*x_size+j+1)];

          if (mid[__boundcheck_ptr_cast_to_array_reference(906,19,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j-1),(i-1)*x_size+j-1)]<8)        { l[_RV_insert_check(0,9,906,51,"susan_thin",0)]=0; l[_RV_insert_check(0,9,906,59,"susan_thin",1)]=0; l[_RV_insert_check(0,9,906,67,"susan_thin",3)]=0; l[_RV_insert_check(0,9,906,75,"susan_thin",2)]*=2; 
                                                l[_RV_insert_check(0,9,907,51,"susan_thin",6)]*=2; l[_RV_insert_check(0,9,907,60,"susan_thin",5)]*=3; l[_RV_insert_check(0,9,907,69,"susan_thin",7)]*=3; l[_RV_insert_check(0,9,907,78,"susan_thin",8)]*=4; }
          else { if (mid[__boundcheck_ptr_cast_to_array_reference(908,26,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j),(i-1)*x_size+j)]<8)   { l[_RV_insert_check(0,9,908,51,"susan_thin",1)]=0; l[_RV_insert_check(0,9,908,59,"susan_thin",0)]=0; l[_RV_insert_check(0,9,908,67,"susan_thin",2)]=0; l[_RV_insert_check(0,9,908,75,"susan_thin",3)]*=2; 
                                                l[_RV_insert_check(0,9,909,51,"susan_thin",5)]*=2; l[_RV_insert_check(0,9,909,60,"susan_thin",6)]*=3; l[_RV_insert_check(0,9,909,69,"susan_thin",8)]*=3; l[_RV_insert_check(0,9,909,78,"susan_thin",7)]*=4; }
          else { if (mid[__boundcheck_ptr_cast_to_array_reference(910,26,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j+1),(i-1)*x_size+j+1)]<8) { l[_RV_insert_check(0,9,910,51,"susan_thin",2)]=0; l[_RV_insert_check(0,9,910,59,"susan_thin",1)]=0; l[_RV_insert_check(0,9,910,67,"susan_thin",5)]=0; l[_RV_insert_check(0,9,910,75,"susan_thin",0)]*=2; 
                                                l[_RV_insert_check(0,9,911,51,"susan_thin",8)]*=2; l[_RV_insert_check(0,9,911,60,"susan_thin",3)]*=3; l[_RV_insert_check(0,9,911,69,"susan_thin",7)]*=3; l[_RV_insert_check(0,9,911,78,"susan_thin",6)]*=4; }
          else { if (mid[__boundcheck_ptr_cast_to_array_reference(912,26,"susan_thin",(void *)(mid),(void *)(mid+(i)*x_size+j-1),(i)*x_size+j-1)]<8)   { l[_RV_insert_check(0,9,912,51,"susan_thin",3)]=0; l[_RV_insert_check(0,9,912,59,"susan_thin",0)]=0; l[_RV_insert_check(0,9,912,67,"susan_thin",6)]=0; l[_RV_insert_check(0,9,912,75,"susan_thin",1)]*=2; 
                                                l[_RV_insert_check(0,9,913,51,"susan_thin",7)]*=2; l[_RV_insert_check(0,9,913,60,"susan_thin",2)]*=3; l[_RV_insert_check(0,9,913,69,"susan_thin",8)]*=3; l[_RV_insert_check(0,9,913,78,"susan_thin",5)]*=4; }
          else { if (mid[__boundcheck_ptr_cast_to_array_reference(914,26,"susan_thin",(void *)(mid),(void *)(mid+(i)*x_size+j+1),(i)*x_size+j+1)]<8)   { l[_RV_insert_check(0,9,914,51,"susan_thin",5)]=0; l[_RV_insert_check(0,9,914,59,"susan_thin",2)]=0; l[_RV_insert_check(0,9,914,67,"susan_thin",8)]=0; l[_RV_insert_check(0,9,914,75,"susan_thin",1)]*=2; 
                                                l[_RV_insert_check(0,9,915,51,"susan_thin",7)]*=2; l[_RV_insert_check(0,9,915,60,"susan_thin",0)]*=3; l[_RV_insert_check(0,9,915,69,"susan_thin",6)]*=3; l[_RV_insert_check(0,9,915,78,"susan_thin",3)]*=4; }
          else { if (mid[__boundcheck_ptr_cast_to_array_reference(916,26,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j-1),(i+1)*x_size+j-1)]<8) { l[_RV_insert_check(0,9,916,51,"susan_thin",6)]=0; l[_RV_insert_check(0,9,916,59,"susan_thin",3)]=0; l[_RV_insert_check(0,9,916,67,"susan_thin",7)]=0; l[_RV_insert_check(0,9,916,75,"susan_thin",0)]*=2; 
                                                l[_RV_insert_check(0,9,917,51,"susan_thin",8)]*=2; l[_RV_insert_check(0,9,917,60,"susan_thin",1)]*=3; l[_RV_insert_check(0,9,917,69,"susan_thin",5)]*=3; l[_RV_insert_check(0,9,917,78,"susan_thin",2)]*=4; }
          else { if (mid[__boundcheck_ptr_cast_to_array_reference(918,26,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j),(i+1)*x_size+j)]<8)   { l[_RV_insert_check(0,9,918,51,"susan_thin",7)]=0; l[_RV_insert_check(0,9,918,59,"susan_thin",6)]=0; l[_RV_insert_check(0,9,918,67,"susan_thin",8)]=0; l[_RV_insert_check(0,9,918,75,"susan_thin",3)]*=2; 
                                                l[_RV_insert_check(0,9,919,51,"susan_thin",5)]*=2; l[_RV_insert_check(0,9,919,60,"susan_thin",0)]*=3; l[_RV_insert_check(0,9,919,69,"susan_thin",2)]*=3; l[_RV_insert_check(0,9,919,78,"susan_thin",1)]*=4; }
          else { if (mid[__boundcheck_ptr_cast_to_array_reference(920,26,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j+1),(i+1)*x_size+j+1)]<8) { l[_RV_insert_check(0,9,920,51,"susan_thin",8)]=0; l[_RV_insert_check(0,9,920,59,"susan_thin",5)]=0; l[_RV_insert_check(0,9,920,67,"susan_thin",7)]=0; l[_RV_insert_check(0,9,920,75,"susan_thin",6)]*=2; 
                                                l[_RV_insert_check(0,9,921,51,"susan_thin",2)]*=2; l[_RV_insert_check(0,9,921,60,"susan_thin",1)]*=3; l[_RV_insert_check(0,9,921,69,"susan_thin",3)]*=3; l[_RV_insert_check(0,9,921,78,"susan_thin",0)]*=4; } }}}}}}}

          m=0;     /* find the highest point */
          for(y=0; y<3; y++)
            for(x=0; x<3; x++)
              if (l[_RV_insert_check(0,9,926,21,"susan_thin",y+y+y+x)]>m) { m=l[_RV_insert_check(0,9,926,39,"susan_thin",y+y+y+x)]; a=y; b=x; }

          if (m>0)
          {
            if (mid[__boundcheck_ptr_cast_to_array_reference(930,21,"susan_thin",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)]<4)
              mid[__boundcheck_ptr_cast_to_array_reference(931,19,"susan_thin",(void *)(mid),(void *)(mid+(i+a-1)*x_size+j+b-1),(i+a-1)*x_size+j+b-1)] = 4;
            else
              mid[__boundcheck_ptr_cast_to_array_reference(933,19,"susan_thin",(void *)(mid),(void *)(mid+(i+a-1)*x_size+j+b-1),(i+a-1)*x_size+j+b-1)] = mid[__boundcheck_ptr_cast_to_array_reference(933,47,"susan_thin",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)]+1;
            if ( (a+a+b) < 3 ) /* need to jump back in image */
	    {
              i+=a-1;
              j+=b-2;
              if (i<4) i=4;
              if (j<4) j=4;
	    }
	  }
        }

/* }}} */
        /* {{{ n==2 */

        if (n==2)
	{
          /* put in a bit here to straighten edges */
          b00 = mid[__boundcheck_ptr_cast_to_array_reference(950,21,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j-1),(i-1)*x_size+j-1)]<8; /* corners of 3x3 */
          b02 = mid[__boundcheck_ptr_cast_to_array_reference(951,21,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j+1),(i-1)*x_size+j+1)]<8;
	  b20 = mid[__boundcheck_ptr_cast_to_array_reference(952,14,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j-1),(i+1)*x_size+j-1)]<8;
          b22 = mid[__boundcheck_ptr_cast_to_array_reference(953,21,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j+1),(i+1)*x_size+j+1)]<8;
          if ( ((b00+b02+b20+b22)==2) && ((b00|b22)&(b02|b20)))
	  {  /* case: move a point back into line.
                e.g. X O X  CAN  become X X X
                     O X O              O O O
                     O O O              O O O    */
            if (b00) 
	    {
              if (b02) { x=0; y=-1; }
              else     { x=-1; y=0; }
	    }
            else
	    {
              if (b02) { x=1; y=0; }
              else     { x=0; y=1; }
	    }
            if (((float)r[__boundcheck_ptr_cast_to_array_reference(969,27,"susan_thin",(void *)(r),(void *)(r+(i+y)*x_size+j+x),(i+y)*x_size+j+x)]/(float)centre) > 0.7)
	    {
              if ( ( (x==0) && (mid[__boundcheck_ptr_cast_to_array_reference(971,37,"susan_thin",(void *)(mid),(void *)(mid+(i+(2*y))*x_size+j),(i+(2*y))*x_size+j)]>7) && (mid[__boundcheck_ptr_cast_to_array_reference(971,68,"susan_thin",(void *)(mid),(void *)(mid+(i+(2*y))*x_size+j-1),(i+(2*y))*x_size+j-1)]>7) && (mid[__boundcheck_ptr_cast_to_array_reference(971,101,"susan_thin",(void *)(mid),(void *)(mid+(i+(2*y))*x_size+j+1),(i+(2*y))*x_size+j+1)]>7) ) ||
                   ( (y==0) && (mid[__boundcheck_ptr_cast_to_array_reference(972,37,"susan_thin",(void *)(mid),(void *)(mid+(i)*x_size+j+(2*x)),(i)*x_size+j+(2*x))]>7) && (mid[__boundcheck_ptr_cast_to_array_reference(972,68,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j+(2*x)),(i+1)*x_size+j+(2*x))]>7) && (mid[__boundcheck_ptr_cast_to_array_reference(972,101,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j+(2*x)),(i-1)*x_size+j+(2*x))]>7) ) )
	      {
                mid[__boundcheck_ptr_cast_to_array_reference(974,21,"susan_thin",(void *)(mid),(void *)(mid+(i)*x_size+j),(i)*x_size+j)]=100;
                mid[__boundcheck_ptr_cast_to_array_reference(975,21,"susan_thin",(void *)(mid),(void *)(mid+(i+y)*x_size+j+x),(i+y)*x_size+j+x)]=3;  /* no jumping needed */
	      }
	    }
	  }
          else
          {
            b01 = mid[__boundcheck_ptr_cast_to_array_reference(981,23,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j ),(i-1)*x_size+j  )]<8;
            b12 = mid[__boundcheck_ptr_cast_to_array_reference(982,23,"susan_thin",(void *)(mid),(void *)(mid+(i  )*x_size+j+1),(i  )*x_size+j+1)]<8;
            b21 = mid[__boundcheck_ptr_cast_to_array_reference(983,23,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j ),(i+1)*x_size+j  )]<8;
            b10 = mid[__boundcheck_ptr_cast_to_array_reference(984,23,"susan_thin",(void *)(mid),(void *)(mid+(i  )*x_size+j-1),(i  )*x_size+j-1)]<8;
            /* {{{ right angle ends - not currently used */

#ifdef IGNORETHIS
            if ( (b00&b01)|(b00&b10)|(b02&b01)|(b02&b12)|(b20&b10)|(b20&b21)|(b22&b21)|(b22&b12) )
	    { /* case; right angle ends. clean up.
                 e.g.; X X O  CAN  become X X O
                       O X O              O O O
                       O O O              O O O        */
              if ( ((b01)&(mid[(i-2)*x_size+j-1]>7)&(mid[(i-2)*x_size+j]>7)&(mid[(i-2)*x_size+j+1]>7)&
                                    ((b00&((2*r[(i-1)*x_size+j+1])>centre))|(b02&((2*r[(i-1)*x_size+j-1])>centre)))) |
                   ((b10)&(mid[(i-1)*x_size+j-2]>7)&(mid[(i)*x_size+j-2]>7)&(mid[(i+1)*x_size+j-2]>7)&
                                    ((b00&((2*r[(i+1)*x_size+j-1])>centre))|(b20&((2*r[(i-1)*x_size+j-1])>centre)))) |
                   ((b12)&(mid[(i-1)*x_size+j+2]>7)&(mid[(i)*x_size+j+2]>7)&(mid[(i+1)*x_size+j+2]>7)&
                                    ((b02&((2*r[(i+1)*x_size+j+1])>centre))|(b22&((2*r[(i-1)*x_size+j+1])>centre)))) |
                   ((b21)&(mid[(i+2)*x_size+j-1]>7)&(mid[(i+2)*x_size+j]>7)&(mid[(i+2)*x_size+j+1]>7)&
                                    ((b20&((2*r[(i+1)*x_size+j+1])>centre))|(b22&((2*r[(i+1)*x_size+j-1])>centre)))) )
	      {
                mid[(i)*x_size+j]=100;
                if (b10&b20) j-=2;
                if (b00|b01|b02) { i--; j-=2; }
  	      }
	    }
#endif

/* }}} */
            if ( ((b01+b12+b21+b10)==2) && ((b10|b12)&(b01|b21)) &&
                 ((b01&((mid[__boundcheck_ptr_cast_to_array_reference(1011,30,"susan_thin",(void *)(mid),(void *)(mid+(i-2)*x_size+j-1),(i-2)*x_size+j-1)]<8)|(mid[__boundcheck_ptr_cast_to_array_reference(1011,56,"susan_thin",(void *)(mid),(void *)(mid+(i-2)*x_size+j+1),(i-2)*x_size+j+1)]<8)))|(b10&((mid[__boundcheck_ptr_cast_to_array_reference(1011,90,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j-2),(i-1)*x_size+j-2)]<8)|(mid[__boundcheck_ptr_cast_to_array_reference(1011,116,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j-2),(i+1)*x_size+j-2)]<8)))|
                (b12&((mid[__boundcheck_ptr_cast_to_array_reference(1012,28,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j+2),(i-1)*x_size+j+2)]<8)|(mid[__boundcheck_ptr_cast_to_array_reference(1012,54,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j+2),(i+1)*x_size+j+2)]<8)))|(b21&((mid[__boundcheck_ptr_cast_to_array_reference(1012,88,"susan_thin",(void *)(mid),(void *)(mid+(i+2)*x_size+j-1),(i+2)*x_size+j-1)]<8)|(mid[__boundcheck_ptr_cast_to_array_reference(1012,114,"susan_thin",(void *)(mid),(void *)(mid+(i+2)*x_size+j+1),(i+2)*x_size+j+1)]<8)))) )
	    { /* case; clears odd right angles.
                 e.g.; O O O  becomes O O O
                       X X O          X O O
                       O X O          O X O     */
              mid[__boundcheck_ptr_cast_to_array_reference(1017,19,"susan_thin",(void *)(mid),(void *)(mid+(i)*x_size+j),(i)*x_size+j)]=100;
              i--;               /* jump back */
              j-=2;
              if (i<4) i=4;
              if (j<4) j=4;
	    }
	  }
	}

/* }}} */
        /* {{{ n>2 the thinning is done here without breaking connectivity */

        if (n>2)
        {
          b01 = mid[__boundcheck_ptr_cast_to_array_reference(1031,21,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j ),(i-1)*x_size+j  )]<8;
          b12 = mid[__boundcheck_ptr_cast_to_array_reference(1032,21,"susan_thin",(void *)(mid),(void *)(mid+(i  )*x_size+j+1),(i  )*x_size+j+1)]<8;
          b21 = mid[__boundcheck_ptr_cast_to_array_reference(1033,21,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j ),(i+1)*x_size+j  )]<8;
          b10 = mid[__boundcheck_ptr_cast_to_array_reference(1034,21,"susan_thin",(void *)(mid),(void *)(mid+(i  )*x_size+j-1),(i  )*x_size+j-1)]<8;
          if((b01+b12+b21+b10)>1)
          {
            b00 = mid[__boundcheck_ptr_cast_to_array_reference(1037,23,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j-1),(i-1)*x_size+j-1)]<8;
            b02 = mid[__boundcheck_ptr_cast_to_array_reference(1038,23,"susan_thin",(void *)(mid),(void *)(mid+(i-1)*x_size+j+1),(i-1)*x_size+j+1)]<8;
	    b20 = mid[__boundcheck_ptr_cast_to_array_reference(1039,16,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j-1),(i+1)*x_size+j-1)]<8;
	    b22 = mid[__boundcheck_ptr_cast_to_array_reference(1040,16,"susan_thin",(void *)(mid),(void *)(mid+(i+1)*x_size+j+1),(i+1)*x_size+j+1)]<8;
            p1 = b00 | b01;
            p2 = b02 | b12;
            p3 = b22 | b21;
            p4 = b20 | b10;

            if( ((p1 + p2 + p3 + p4) - ((b01 & p2)+(b12 & p3)+(b21 & p4)+(b10 & p1))) < 2)
            {
              mid[__boundcheck_ptr_cast_to_array_reference(1048,19,"susan_thin",(void *)(mid),(void *)(mid+(i)*x_size+j),(i)*x_size+j)]=100;
              i--;
              j-=2;
              if (i<4) i=4;
              if (j<4) j=4;
            }
          }
        }

/* }}} */
      }
}

/* }}} */
/* {{{ susan_edges(in,r,sf,max_no,out) */

susan_edges(in,r,mid,bp,max_no,x_size,y_size)
  uchar *in, *bp, *mid;
  int   *r, max_no, x_size, y_size;
{
float z;
__boundcheck_metadata_store((void *)(&z),(void *)((size_t)(&z)+sizeof(z)*8-1));

int   do_symmetry;
__boundcheck_metadata_store((void *)(&do_symmetry),(void *)((size_t)(&do_symmetry)+sizeof(do_symmetry)*8-1));
int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int m;
__boundcheck_metadata_store((void *)(&m),(void *)((size_t)(&m)+sizeof(m)*8-1));
int n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));
int a;
__boundcheck_metadata_store((void *)(&a),(void *)((size_t)(&a)+sizeof(a)*8-1));
int b;
__boundcheck_metadata_store((void *)(&b),(void *)((size_t)(&b)+sizeof(b)*8-1));
int x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));
int y;
__boundcheck_metadata_store((void *)(&y),(void *)((size_t)(&y)+sizeof(y)*8-1));
int w;
__boundcheck_metadata_store((void *)(&w),(void *)((size_t)(&w)+sizeof(w)*8-1));

uchar c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));
uchar * p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
uchar * cp;
__boundcheck_metadata_store((void *)(&cp),(void *)((size_t)(&cp)+sizeof(cp)*8-1));


  memset (r,0,x_size * y_size * sizeof(int));

  for (i=3;i<y_size-3;i++)
    for (j=3;j<x_size-3;j++)
    {
      n=100;
      p=in + (i-3)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 3) * x_size));

      cp=bp + in[__boundcheck_ptr_cast_to_array_reference(1079,18,"susan_edges",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + in[i * x_size + j]));


      n+=*(uchar *)(__boundcheck_ptr_reference(1081,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1081,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1082,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1082,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1083,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1083,16,"susan_edges",(void *)(p),(void *)(p)))))));
      p+=x_size-3; 

      n+=*(uchar *)(__boundcheck_ptr_reference(1086,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1086,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1087,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1087,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1088,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1088,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1089,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1089,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1090,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1090,16,"susan_edges",(void *)(p),(void *)(p)))))));
      p+=x_size-5;

      n+=*(uchar *)(__boundcheck_ptr_reference(1093,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1093,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1094,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1094,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1095,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1095,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1096,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1096,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1097,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1097,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1098,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1098,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1099,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1099,16,"susan_edges",(void *)(p),(void *)(p)))))));
      p+=x_size-6;

      n+=*(uchar *)(__boundcheck_ptr_reference(1102,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1102,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1103,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1103,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1104,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1104,16,"susan_edges",(void *)(p),(void *)(p)))))));
      p+=2;
      n+=*(uchar *)(__boundcheck_ptr_reference(1106,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1106,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1107,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1107,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1108,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1108,16,"susan_edges",(void *)(p),(void *)(p)))))));
      p+=x_size-6;

      n+=*(uchar *)(__boundcheck_ptr_reference(1111,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1111,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1112,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1112,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1113,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1113,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1114,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1114,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1115,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1115,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1116,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1116,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1117,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1117,16,"susan_edges",(void *)(p),(void *)(p)))))));
      p+=x_size-5;

      n+=*(uchar *)(__boundcheck_ptr_reference(1120,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1120,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1121,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1121,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1122,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1122,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1123,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1123,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1124,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1124,16,"susan_edges",(void *)(p),(void *)(p)))))));
      p+=x_size-3;

      n+=*(uchar *)(__boundcheck_ptr_reference(1127,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1127,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1128,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1128,16,"susan_edges",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1129,11,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1129,16,"susan_edges",(void *)(p),(void *)(p)))))));

      if (n<=max_no)
        r[__boundcheck_ptr_cast_to_array_reference(1132,11,"susan_edges",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)] = max_no - n;
    }

  for (i=4;i<y_size-4;i++)
    for (j=4;j<x_size-4;j++)
    {
      if (r[__boundcheck_ptr_cast_to_array_reference(1138,13,"susan_edges",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)]>0)
      {
        m=r[__boundcheck_ptr_cast_to_array_reference(1140,13,"susan_edges",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)];
        n=max_no - m;
        cp=bp + in[__boundcheck_ptr_cast_to_array_reference(1142,20,"susan_edges",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + in[i * x_size + j]));


        if (n>600)
        {
          p=in + (i-3)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 3) * x_size));

          x=0;y=0;

          c=*(uchar *)(__boundcheck_ptr_reference(1149,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1149,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=c;y-=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1150,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1150,19,"susan_edges",(void *)(p),(void *)(p++)))))));y-=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1151,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1151,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=c;y-=3*c;
          p+=x_size-3; 
    
          c=*(uchar *)(__boundcheck_ptr_reference(1154,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1154,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=2*c;y-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1155,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1155,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=c;y-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1156,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1156,19,"susan_edges",(void *)(p),(void *)(p++)))))));y-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1157,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1157,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1158,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1158,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=2*c;y-=2*c;
          p+=x_size-5;
    
          c=*(uchar *)(__boundcheck_ptr_reference(1161,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1161,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=3*c;y-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1162,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1162,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=2*c;y-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1163,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1163,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=c;y-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1164,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1164,19,"susan_edges",(void *)(p),(void *)(p++)))))));y-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1165,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1165,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1166,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1166,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=2*c;y-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1167,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1167,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=3*c;y-=c;
          p+=x_size-6;

          c=*(uchar *)(__boundcheck_ptr_reference(1170,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1170,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1171,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1171,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1172,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1172,19,"susan_edges",(void *)(p),(void *)(p)))))));x-=c;
          p+=2;
          c=*(uchar *)(__boundcheck_ptr_reference(1174,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1174,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1175,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1175,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1176,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1176,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=3*c;
          p+=x_size-6;
    
          c=*(uchar *)(__boundcheck_ptr_reference(1179,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1179,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=3*c;y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1180,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1180,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=2*c;y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1181,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1181,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=c;y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1182,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1182,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1183,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1183,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1184,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1184,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=2*c;y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1185,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1185,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=3*c;y+=c;
          p+=x_size-5;

          c=*(uchar *)(__boundcheck_ptr_reference(1188,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1188,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=2*c;y+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1189,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1189,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=c;y+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1190,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1190,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1191,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1191,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1192,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1192,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=2*c;y+=2*c;
          p+=x_size-3;

          c=*(uchar *)(__boundcheck_ptr_reference(1195,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1195,19,"susan_edges",(void *)(p),(void *)(p++)))))));x-=c;y+=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1196,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1196,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1197,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1197,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=c;y+=3*c;

          z = sqrt((float)((x*x) + (y*y)));
          if (z > (0.9*(float)n)) /* 0.5 */
	  {
            do_symmetry=0;
            if (x==0)
              z=1000000.0;
            else
              z=((float)y) / ((float)x);
            if (z < 0) { z=-z; w=-1; }
            else w=1;
            if (z < 0.5) { /* vert_edge */ a=0; b=1; }
            else { if (z > 2.0) { /* hor_edge */ a=1; b=0; }
            else { /* diag_edge */ if (w>0) { a=1; b=1; }
                                   else { a=-1; b=1; }}}
            if ( (m > r[__boundcheck_ptr_cast_to_array_reference(1213,25,"susan_edges",(void *)(r),(void *)(r+(i+a)*x_size+j+b),(i+a)*x_size+j+b)]) && (m >= r[__boundcheck_ptr_cast_to_array_reference(1213,55,"susan_edges",(void *)(r),(void *)(r+(i-a)*x_size+j-b),(i-a)*x_size+j-b)]) &&
                 (m > r[__boundcheck_ptr_cast_to_array_reference(1214,25,"susan_edges",(void *)(r),(void *)(r+(i+(2*a))*x_size+j+(2*b)),(i+(2*a))*x_size+j+(2*b))]) && (m >= r[__boundcheck_ptr_cast_to_array_reference(1214,63,"susan_edges",(void *)(r),(void *)(r+(i-(2*a))*x_size+j-(2*b)),(i-(2*a))*x_size+j-(2*b))]) )
              mid[__boundcheck_ptr_cast_to_array_reference(1215,19,"susan_edges",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)] = 1;
          }
          else
            do_symmetry=1;
        }
        else 
          do_symmetry=1;

        if (do_symmetry==1)
	{ 
          p=in + (i-3)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 3) * x_size));

          x=0; y=0; w=0;

          /*   |      \
               y  -x-  w
               |        \   */

          c=*(uchar *)(__boundcheck_ptr_reference(1232,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1232,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=9*c;w+=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1233,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1233,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=9*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1234,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1234,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=c;y+=9*c;w-=3*c;
          p+=x_size-3; 
  
          c=*(uchar *)(__boundcheck_ptr_reference(1237,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1237,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=4*c;y+=4*c;w+=4*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1238,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1238,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=4*c;w+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1239,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1239,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=4*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1240,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1240,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=4*c;w-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1241,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1241,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=4*c;y+=4*c;w-=4*c;
          p+=x_size-5;
    
          c=*(uchar *)(__boundcheck_ptr_reference(1244,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1244,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=9*c;y+=c;w+=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1245,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1245,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=4*c;y+=c;w+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1246,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1246,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=c;w+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1247,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1247,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1248,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1248,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=c;w-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1249,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1249,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=4*c;y+=c;w-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1250,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1250,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=9*c;y+=c;w-=3*c;
          p+=x_size-6;

          c=*(uchar *)(__boundcheck_ptr_reference(1253,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1253,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=9*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1254,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1254,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=4*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1255,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1255,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=c;
          p+=2;
          c=*(uchar *)(__boundcheck_ptr_reference(1257,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1257,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1258,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1258,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=4*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1259,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1259,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=9*c;
          p+=x_size-6;
    
          c=*(uchar *)(__boundcheck_ptr_reference(1262,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1262,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=9*c;y+=c;w-=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1263,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1263,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=4*c;y+=c;w-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1264,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1264,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=c;w-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1265,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1265,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1266,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1266,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=c;w+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1267,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1267,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=4*c;y+=c;w+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1268,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1268,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=9*c;y+=c;w+=3*c;
          p+=x_size-5;
 
          c=*(uchar *)(__boundcheck_ptr_reference(1271,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1271,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=4*c;y+=4*c;w-=4*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1272,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1272,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=4*c;w-=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1273,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1273,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=4*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1274,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1274,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=4*c;w+=2*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1275,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1275,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=4*c;y+=4*c;w+=4*c;
          p+=x_size-3;

          c=*(uchar *)(__boundcheck_ptr_reference(1278,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1278,19,"susan_edges",(void *)(p),(void *)(p++)))))));x+=c;y+=9*c;w-=3*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1279,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1279,19,"susan_edges",(void *)(p),(void *)(p++)))))));y+=9*c;
          c=*(uchar *)(__boundcheck_ptr_reference(1280,14,"susan_edges",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1280,19,"susan_edges",(void *)(p),(void *)(p)))))));x+=c;y+=9*c;w+=3*c;

          if (y==0)
            z = 1000000.0;
          else
            z = ((float)x) / ((float)y);
          if (z < 0.5) { /* vertical */ a=0; b=1; }
          else { if (z > 2.0) { /* horizontal */ a=1; b=0; }
          else { /* diagonal */ if (w>0) { a=-1; b=1; }
                                else { a=1; b=1; }}}
          if ( (m > r[__boundcheck_ptr_cast_to_array_reference(1290,23,"susan_edges",(void *)(r),(void *)(r+(i+a)*x_size+j+b),(i+a)*x_size+j+b)]) && (m >= r[__boundcheck_ptr_cast_to_array_reference(1290,53,"susan_edges",(void *)(r),(void *)(r+(i-a)*x_size+j-b),(i-a)*x_size+j-b)]) &&
               (m > r[__boundcheck_ptr_cast_to_array_reference(1291,23,"susan_edges",(void *)(r),(void *)(r+(i+(2*a))*x_size+j+(2*b)),(i+(2*a))*x_size+j+(2*b))]) && (m >= r[__boundcheck_ptr_cast_to_array_reference(1291,61,"susan_edges",(void *)(r),(void *)(r+(i-(2*a))*x_size+j-(2*b)),(i-(2*a))*x_size+j-(2*b))]) )
            mid[__boundcheck_ptr_cast_to_array_reference(1292,17,"susan_edges",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)] = 2;	
        }
      }
    }
}

/* }}} */
/* {{{ susan_edges_small(in,r,sf,max_no,out) */

susan_edges_small(in,r,mid,bp,max_no,x_size,y_size)
  uchar *in, *bp, *mid;
  int   *r, max_no, x_size, y_size;
{
float z;
__boundcheck_metadata_store((void *)(&z),(void *)((size_t)(&z)+sizeof(z)*8-1));

int   do_symmetry;
__boundcheck_metadata_store((void *)(&do_symmetry),(void *)((size_t)(&do_symmetry)+sizeof(do_symmetry)*8-1));
int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int m;
__boundcheck_metadata_store((void *)(&m),(void *)((size_t)(&m)+sizeof(m)*8-1));
int n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));
int a;
__boundcheck_metadata_store((void *)(&a),(void *)((size_t)(&a)+sizeof(a)*8-1));
int b;
__boundcheck_metadata_store((void *)(&b),(void *)((size_t)(&b)+sizeof(b)*8-1));
int x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));
int y;
__boundcheck_metadata_store((void *)(&y),(void *)((size_t)(&y)+sizeof(y)*8-1));
int w;
__boundcheck_metadata_store((void *)(&w),(void *)((size_t)(&w)+sizeof(w)*8-1));

uchar c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));
uchar * p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
uchar * cp;
__boundcheck_metadata_store((void *)(&cp),(void *)((size_t)(&cp)+sizeof(cp)*8-1));


  memset (r,0,x_size * y_size * sizeof(int));

  max_no = 730; /* ho hum ;) */

  for (i=1;i<y_size-1;i++)
    for (j=1;j<x_size-1;j++)
    {
      n=100;
      p=in + (i-1)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 1) * x_size));

      cp=bp + in[__boundcheck_ptr_cast_to_array_reference(1318,18,"susan_edges_small",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + in[i * x_size + j]));


      n+=*(uchar *)(__boundcheck_ptr_reference(1320,11,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1320,16,"susan_edges_small",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1321,11,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1321,16,"susan_edges_small",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1322,11,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1322,16,"susan_edges_small",(void *)(p),(void *)(p)))))));
      p+=x_size-2; 

      n+=*(uchar *)(__boundcheck_ptr_reference(1325,11,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1325,16,"susan_edges_small",(void *)(p),(void *)(p)))))));
      p+=2;
      n+=*(uchar *)(__boundcheck_ptr_reference(1327,11,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1327,16,"susan_edges_small",(void *)(p),(void *)(p)))))));
      p+=x_size-2;

      n+=*(uchar *)(__boundcheck_ptr_reference(1330,11,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1330,16,"susan_edges_small",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1331,11,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1331,16,"susan_edges_small",(void *)(p),(void *)(p++)))))));
      n+=*(uchar *)(__boundcheck_ptr_reference(1332,11,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1332,16,"susan_edges_small",(void *)(p),(void *)(p)))))));

      if (n<=max_no)
        r[__boundcheck_ptr_cast_to_array_reference(1335,11,"susan_edges_small",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)] = max_no - n;
    }

  for (i=2;i<y_size-2;i++)
    for (j=2;j<x_size-2;j++)
    {
      if (r[__boundcheck_ptr_cast_to_array_reference(1341,13,"susan_edges_small",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)]>0)
      {
        m=r[__boundcheck_ptr_cast_to_array_reference(1343,13,"susan_edges_small",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)];
        n=max_no - m;
        cp=bp + in[__boundcheck_ptr_cast_to_array_reference(1345,20,"susan_edges_small",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + in[i * x_size + j]));


        if (n>250)
	{
          p=in + (i-1)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 1) * x_size));

          x=0;y=0;

          c=*(uchar *)(__boundcheck_ptr_reference(1352,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1352,19,"susan_edges_small",(void *)(p),(void *)(p++)))))));x-=c;y-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1353,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1353,19,"susan_edges_small",(void *)(p),(void *)(p++)))))));y-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1354,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1354,19,"susan_edges_small",(void *)(p),(void *)(p)))))));x+=c;y-=c;
          p+=x_size-2; 

          c=*(uchar *)(__boundcheck_ptr_reference(1357,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1357,19,"susan_edges_small",(void *)(p),(void *)(p)))))));x-=c;
          p+=2;
          c=*(uchar *)(__boundcheck_ptr_reference(1359,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1359,19,"susan_edges_small",(void *)(p),(void *)(p)))))));x+=c;
          p+=x_size-2;

          c=*(uchar *)(__boundcheck_ptr_reference(1362,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1362,19,"susan_edges_small",(void *)(p),(void *)(p++)))))));x-=c;y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1363,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1363,19,"susan_edges_small",(void *)(p),(void *)(p++)))))));y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1364,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1364,19,"susan_edges_small",(void *)(p),(void *)(p)))))));x+=c;y+=c;

          z = sqrt((float)((x*x) + (y*y)));
          if (z > (0.4*(float)n)) /* 0.6 */
          {
            do_symmetry=0;
            if (x==0)
	      z=1000000.0;
	    else
	      z=((float)y) / ((float)x);
	    if (z < 0) { z=-z; w=-1; }
            else w=1;
            if (z < 0.5) { /* vert_edge */ a=0; b=1; }
            else { if (z > 2.0) { /* hor_edge */ a=1; b=0; }
            else { /* diag_edge */ if (w>0) { a=1; b=1; }
                                   else { a=-1; b=1; }}}
            if ( (m > r[__boundcheck_ptr_cast_to_array_reference(1380,25,"susan_edges_small",(void *)(r),(void *)(r+(i+a)*x_size+j+b),(i+a)*x_size+j+b)]) && (m >= r[__boundcheck_ptr_cast_to_array_reference(1380,55,"susan_edges_small",(void *)(r),(void *)(r+(i-a)*x_size+j-b),(i-a)*x_size+j-b)]) )
              mid[__boundcheck_ptr_cast_to_array_reference(1381,19,"susan_edges_small",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)] = 1;
          }
          else
            do_symmetry=1;
        }
        else
          do_symmetry=1;

        if (do_symmetry==1)
	{ 
          p=in + (i-1)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 1) * x_size));

          x=0; y=0; w=0;

          /*   |      \
               y  -x-  w
               |        \   */

          c=*(uchar *)(__boundcheck_ptr_reference(1398,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1398,19,"susan_edges_small",(void *)(p),(void *)(p++)))))));x+=c;y+=c;w+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1399,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1399,19,"susan_edges_small",(void *)(p),(void *)(p++)))))));y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1400,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1400,19,"susan_edges_small",(void *)(p),(void *)(p)))))));x+=c;y+=c;w-=c;
          p+=x_size-2; 

          c=*(uchar *)(__boundcheck_ptr_reference(1403,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1403,19,"susan_edges_small",(void *)(p),(void *)(p)))))));x+=c;
          p+=2;
          c=*(uchar *)(__boundcheck_ptr_reference(1405,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1405,19,"susan_edges_small",(void *)(p),(void *)(p)))))));x+=c;
          p+=x_size-2;

          c=*(uchar *)(__boundcheck_ptr_reference(1408,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1408,19,"susan_edges_small",(void *)(p),(void *)(p++)))))));x+=c;y+=c;w-=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1409,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1409,19,"susan_edges_small",(void *)(p),(void *)(p++)))))));y+=c;
          c=*(uchar *)(__boundcheck_ptr_reference(1410,14,"susan_edges_small",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1410,19,"susan_edges_small",(void *)(p),(void *)(p)))))));x+=c;y+=c;w+=c;

          if (y==0)
            z = 1000000.0;
          else
            z = ((float)x) / ((float)y);
          if (z < 0.5) { /* vertical */ a=0; b=1; }
          else { if (z > 2.0) { /* horizontal */ a=1; b=0; }
          else { /* diagonal */ if (w>0) { a=-1; b=1; }
                                else { a=1; b=1; }}}
          if ( (m > r[__boundcheck_ptr_cast_to_array_reference(1420,23,"susan_edges_small",(void *)(r),(void *)(r+(i+a)*x_size+j+b),(i+a)*x_size+j+b)]) && (m >= r[__boundcheck_ptr_cast_to_array_reference(1420,53,"susan_edges_small",(void *)(r),(void *)(r+(i-a)*x_size+j-b),(i-a)*x_size+j-b)]) )
            mid[__boundcheck_ptr_cast_to_array_reference(1421,17,"susan_edges_small",(void *)(mid),(void *)(mid+i*x_size+j),i*x_size+j)] = 2;	
        }
      }
    }
}

/* }}} */

/* }}} */
/* {{{ corners */

/* {{{ corner_draw(in,corner_list,drawing_mode) */

corner_draw(in,corner_list,x_size,drawing_mode)
  uchar *in;
  CORNER_LIST corner_list;
  int x_size, drawing_mode;
{
uchar *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));

int   n=0;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));


  while(corner_list[__boundcheck_ptr_cast_to_array_reference(1442,21,"corner_draw",(void *)(corner_list),(void *)(corner_list+n),n)].info != 7)
  {
    if (drawing_mode==0)
    {
      p = in + (corner_list[__boundcheck_ptr_cast_to_array_reference(1446,29,"corner_draw",(void *)(corner_list),(void *)(corner_list+n),n)].y-1)*x_size + corner_list[__boundcheck_ptr_cast_to_array_reference(1446,58,"corner_draw",(void *)(corner_list),(void *)(corner_list+n),n)].x - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (corner_list[n].y - 1) * x_size));

      *(uchar *)(__boundcheck_ptr_reference(1447,8,"corner_draw",(void *)(p),(void *)(p++)))=255; *(uchar *)(__boundcheck_ptr_reference(1447,18,"corner_draw",(void *)(p),(void *)(p++)))=255; *(uchar *)(__boundcheck_ptr_reference(1447,28,"corner_draw",(void *)(p),(void *)(p)))=255; p+=x_size-2;
      *(uchar *)(__boundcheck_ptr_reference(1448,8,"corner_draw",(void *)(p),(void *)(p++)))=255; *(uchar *)(__boundcheck_ptr_reference(1448,18,"corner_draw",(void *)(p),(void *)(p++)))=0;   *(uchar *)(__boundcheck_ptr_reference(1448,28,"corner_draw",(void *)(p),(void *)(p)))=255; p+=x_size-2;
      *(uchar *)(__boundcheck_ptr_reference(1449,8,"corner_draw",(void *)(p),(void *)(p++)))=255; *(uchar *)(__boundcheck_ptr_reference(1449,18,"corner_draw",(void *)(p),(void *)(p++)))=255; *(uchar *)(__boundcheck_ptr_reference(1449,28,"corner_draw",(void *)(p),(void *)(p)))=255;
      n++;
    }
    else
    {
      p = in + corner_list[__boundcheck_ptr_cast_to_array_reference(1454,28,"corner_draw",(void *)(corner_list),(void *)(corner_list+n),n)].y*x_size + corner_list[__boundcheck_ptr_cast_to_array_reference(1454,54,"corner_draw",(void *)(corner_list),(void *)(corner_list+n),n)].x;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + corner_list[n].y * x_size));

      *(uchar *)(__boundcheck_ptr_reference(1455,8,"corner_draw",(void *)(p),(void *)(p)))=0;
      n++;
    }
  }
}

/* }}} */
/* {{{ susan(in,r,sf,max_no,corner_list) */

susan_corners(in,r,bp,max_no,corner_list,x_size,y_size)
  uchar       *in, *bp;
  int         *r, max_no, x_size, y_size;
  CORNER_LIST corner_list;
{
int   n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));
int x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));
int y;
__boundcheck_metadata_store((void *)(&y),(void *)((size_t)(&y)+sizeof(y)*8-1));
int sq;
__boundcheck_metadata_store((void *)(&sq),(void *)((size_t)(&sq)+sizeof(sq)*8-1));
int xx;
__boundcheck_metadata_store((void *)(&xx),(void *)((size_t)(&xx)+sizeof(xx)*8-1));
int yy;
__boundcheck_metadata_store((void *)(&yy),(void *)((size_t)(&yy)+sizeof(yy)*8-1));

     int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int * cgx;
__boundcheck_metadata_store((void *)(&cgx),(void *)((size_t)(&cgx)+sizeof(cgx)*8-1));
int * cgy;
__boundcheck_metadata_store((void *)(&cgy),(void *)((size_t)(&cgy)+sizeof(cgy)*8-1));

float divide;
__boundcheck_metadata_store((void *)(&divide),(void *)((size_t)(&divide)+sizeof(divide)*8-1));

uchar c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));
uchar * p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
uchar * cp;
__boundcheck_metadata_store((void *)(&cp),(void *)((size_t)(&cp)+sizeof(cp)*8-1));


  memset (r,0,x_size * y_size * sizeof(int));

  cgx=(int *)__boundcheck_malloc(x_size*y_size*sizeof(int));
  cgy=(int *)__boundcheck_malloc(x_size*y_size*sizeof(int));

  for (i=5;i<y_size-5;i++)
    for (j=5;j<x_size-5;j++) {
        n=100;
        p=in + (i-3)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 3) * x_size));

        cp=bp + in[__boundcheck_ptr_cast_to_array_reference(1483,20,"susan_corners",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + in[i * x_size + j]));


        n+=*(uchar *)(__boundcheck_ptr_reference(1485,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1485,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1486,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1486,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1487,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1487,18,"susan_corners",(void *)(p),(void *)(p)))))));
        p+=x_size-3; 

        n+=*(uchar *)(__boundcheck_ptr_reference(1490,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1490,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1491,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1491,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1492,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1492,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1493,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1493,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1494,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1494,18,"susan_corners",(void *)(p),(void *)(p)))))));
        p+=x_size-5;

        n+=*(uchar *)(__boundcheck_ptr_reference(1497,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1497,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1498,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1498,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1499,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1499,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1500,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1500,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1501,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1501,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1502,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1502,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1503,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1503,18,"susan_corners",(void *)(p),(void *)(p)))))));
        p+=x_size-6;

        n+=*(uchar *)(__boundcheck_ptr_reference(1506,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1506,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1507,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1507,18,"susan_corners",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1508,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1508,18,"susan_corners",(void *)(p),(void *)(p)))))));
      if (n<max_no){    /* do this test early and often ONLY to save wasted computation */
        p+=2;
        n+=*(uchar *)(__boundcheck_ptr_reference(1511,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1511,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1513,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1513,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1515,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1515,18,"susan_corners",(void *)(p),(void *)(p)))))));
      if (n<max_no){
        p+=x_size-6;

        n+=*(uchar *)(__boundcheck_ptr_reference(1519,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1519,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1521,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1521,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1523,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1523,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1525,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1525,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1527,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1527,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1529,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1529,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1531,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1531,18,"susan_corners",(void *)(p),(void *)(p)))))));
      if (n<max_no){
        p+=x_size-5;

        n+=*(uchar *)(__boundcheck_ptr_reference(1535,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1535,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1537,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1537,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1539,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1539,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1541,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1541,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1543,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1543,18,"susan_corners",(void *)(p),(void *)(p)))))));
      if (n<max_no){
        p+=x_size-3;

        n+=*(uchar *)(__boundcheck_ptr_reference(1547,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1547,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1549,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1549,18,"susan_corners",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1551,13,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1551,18,"susan_corners",(void *)(p),(void *)(p)))))));

        if (n<max_no)
        {
            x=0;y=0;
            p=in + (i-3)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 3) * x_size));


            c=*(uchar *)(__boundcheck_ptr_reference(1558,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1558,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=c;y-=3*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1559,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1559,21,"susan_corners",(void *)(p),(void *)(p++)))))));y-=3*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1560,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1560,21,"susan_corners",(void *)(p),(void *)(p)))))));x+=c;y-=3*c;
            p+=x_size-3; 
    
            c=*(uchar *)(__boundcheck_ptr_reference(1563,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1563,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=2*c;y-=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1564,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1564,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=c;y-=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1565,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1565,21,"susan_corners",(void *)(p),(void *)(p++)))))));y-=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1566,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1566,21,"susan_corners",(void *)(p),(void *)(p++)))))));x+=c;y-=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1567,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1567,21,"susan_corners",(void *)(p),(void *)(p)))))));x+=2*c;y-=2*c;
            p+=x_size-5;
    
            c=*(uchar *)(__boundcheck_ptr_reference(1570,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1570,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=3*c;y-=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1571,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1571,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=2*c;y-=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1572,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1572,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=c;y-=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1573,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1573,21,"susan_corners",(void *)(p),(void *)(p++)))))));y-=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1574,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1574,21,"susan_corners",(void *)(p),(void *)(p++)))))));x+=c;y-=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1575,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1575,21,"susan_corners",(void *)(p),(void *)(p++)))))));x+=2*c;y-=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1576,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1576,21,"susan_corners",(void *)(p),(void *)(p)))))));x+=3*c;y-=c;
            p+=x_size-6;

            c=*(uchar *)(__boundcheck_ptr_reference(1579,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1579,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=3*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1580,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1580,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1581,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1581,21,"susan_corners",(void *)(p),(void *)(p)))))));x-=c;
            p+=2;
            c=*(uchar *)(__boundcheck_ptr_reference(1583,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1583,21,"susan_corners",(void *)(p),(void *)(p++)))))));x+=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1584,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1584,21,"susan_corners",(void *)(p),(void *)(p++)))))));x+=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1585,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1585,21,"susan_corners",(void *)(p),(void *)(p)))))));x+=3*c;
            p+=x_size-6;
    
            c=*(uchar *)(__boundcheck_ptr_reference(1588,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1588,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=3*c;y+=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1589,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1589,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=2*c;y+=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1590,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1590,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=c;y+=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1591,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1591,21,"susan_corners",(void *)(p),(void *)(p++)))))));y+=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1592,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1592,21,"susan_corners",(void *)(p),(void *)(p++)))))));x+=c;y+=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1593,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1593,21,"susan_corners",(void *)(p),(void *)(p++)))))));x+=2*c;y+=c;
            c=*(uchar *)(__boundcheck_ptr_reference(1594,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1594,21,"susan_corners",(void *)(p),(void *)(p)))))));x+=3*c;y+=c;
            p+=x_size-5;

            c=*(uchar *)(__boundcheck_ptr_reference(1597,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1597,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=2*c;y+=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1598,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1598,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=c;y+=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1599,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1599,21,"susan_corners",(void *)(p),(void *)(p++)))))));y+=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1600,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1600,21,"susan_corners",(void *)(p),(void *)(p++)))))));x+=c;y+=2*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1601,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1601,21,"susan_corners",(void *)(p),(void *)(p)))))));x+=2*c;y+=2*c;
            p+=x_size-3;

            c=*(uchar *)(__boundcheck_ptr_reference(1604,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1604,21,"susan_corners",(void *)(p),(void *)(p++)))))));x-=c;y+=3*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1605,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1605,21,"susan_corners",(void *)(p),(void *)(p++)))))));y+=3*c;
            c=*(uchar *)(__boundcheck_ptr_reference(1606,16,"susan_corners",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1606,21,"susan_corners",(void *)(p),(void *)(p)))))));x+=c;y+=3*c;

            xx=x*x;
            yy=y*y;
            sq=xx+yy;
            if ( sq > ((n*n)/2) )
            {
              if(yy<xx) {
                divide=(float)y/(float)abs(x);
                sq=abs(x)/x;
                sq=*(uchar *)(__boundcheck_ptr_reference(1616,21,"susan_corners",(void *)(cp),(void *)((cp-in[__boundcheck_ptr_cast_to_array_reference(1616,28,"susan_corners",(void *)(in),(void *)(in+(i+FTOI(divide))*x_size+j+sq),(i+FTOI(divide))*x_size+j+sq)])))) +
                   *(uchar *)(__boundcheck_ptr_reference(1617,21,"susan_corners",(void *)(cp),(void *)((cp-in[__boundcheck_ptr_cast_to_array_reference(1617,28,"susan_corners",(void *)(in),(void *)(in+(i+FTOI(2*divide))*x_size+j+2*sq),(i+FTOI(2*divide))*x_size+j+2*sq)])))) +
                   *(uchar *)(__boundcheck_ptr_reference(1618,21,"susan_corners",(void *)(cp),(void *)((cp-in[__boundcheck_ptr_cast_to_array_reference(1618,28,"susan_corners",(void *)(in),(void *)(in+(i+FTOI(3*divide))*x_size+j+3*sq),(i+FTOI(3*divide))*x_size+j+3*sq)]))));}
              else {
                divide=(float)x/(float)abs(y);
                sq=abs(y)/y;
                sq=*(uchar *)(__boundcheck_ptr_reference(1622,21,"susan_corners",(void *)(cp),(void *)((cp-in[__boundcheck_ptr_cast_to_array_reference(1622,28,"susan_corners",(void *)(in),(void *)(in+(i+sq)*x_size+j+FTOI(divide)),(i+sq)*x_size+j+FTOI(divide))])))) +
                   *(uchar *)(__boundcheck_ptr_reference(1623,21,"susan_corners",(void *)(cp),(void *)((cp-in[__boundcheck_ptr_cast_to_array_reference(1623,28,"susan_corners",(void *)(in),(void *)(in+(i+2*sq)*x_size+j+FTOI(2*divide)),(i+2*sq)*x_size+j+FTOI(2*divide))])))) +
                   *(uchar *)(__boundcheck_ptr_reference(1624,21,"susan_corners",(void *)(cp),(void *)((cp-in[__boundcheck_ptr_cast_to_array_reference(1624,28,"susan_corners",(void *)(in),(void *)(in+(i+3*sq)*x_size+j+FTOI(3*divide)),(i+3*sq)*x_size+j+FTOI(3*divide))]))));}

              if(sq>290){
                r[__boundcheck_ptr_cast_to_array_reference(1627,19,"susan_corners",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)] = max_no-n;
                cgx[__boundcheck_ptr_cast_to_array_reference(1628,21,"susan_corners",(void *)(cgx),(void *)(cgx+i*x_size+j),i*x_size+j)] = (51*x)/n;
                cgy[__boundcheck_ptr_cast_to_array_reference(1629,21,"susan_corners",(void *)(cgy),(void *)(cgy+i*x_size+j),i*x_size+j)] = (51*y)/n;}
            }
	}
}}}}}}}}}}}}}}}}}}}

  /* to locate the local maxima */
  n=0;
  for (i=5;i<y_size-5;i++)
    for (j=5;j<x_size-5;j++) {
       x = r[__boundcheck_ptr_cast_to_array_reference(1638,14,"susan_corners",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)];
       if (x>0)  {
          /* 5x5 mask */
#ifdef FIVE_SUPP
          if (
              (x>r[(i-1)*x_size+j+2]) &&
              (x>r[(i  )*x_size+j+1]) &&
              (x>r[(i  )*x_size+j+2]) &&
              (x>r[(i+1)*x_size+j-1]) &&
              (x>r[(i+1)*x_size+j  ]) &&
              (x>r[(i+1)*x_size+j+1]) &&
              (x>r[(i+1)*x_size+j+2]) &&
              (x>r[(i+2)*x_size+j-2]) &&
              (x>r[(i+2)*x_size+j-1]) &&
              (x>r[(i+2)*x_size+j  ]) &&
              (x>r[(i+2)*x_size+j+1]) &&
              (x>r[(i+2)*x_size+j+2]) &&
              (x>=r[(i-2)*x_size+j-2]) &&
              (x>=r[(i-2)*x_size+j-1]) &&
              (x>=r[(i-2)*x_size+j  ]) &&
              (x>=r[(i-2)*x_size+j+1]) &&
              (x>=r[(i-2)*x_size+j+2]) &&
              (x>=r[(i-1)*x_size+j-2]) &&
              (x>=r[(i-1)*x_size+j-1]) &&
	      (x>=r[(i-1)*x_size+j  ]) &&
	      (x>=r[(i-1)*x_size+j+1]) &&
	      (x>=r[(i  )*x_size+j-2]) &&
	      (x>=r[(i  )*x_size+j-1]) &&
	      (x>=r[(i+1)*x_size+j-2]) )
#endif
#ifdef SEVEN_SUPP
          if ( 
                (x>r[__boundcheck_ptr_cast_to_array_reference(1670,22,"susan_corners",(void *)(r),(void *)(r+(i-3)*x_size+j-3),(i-3)*x_size+j-3)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1671,22,"susan_corners",(void *)(r),(void *)(r+(i-3)*x_size+j-2),(i-3)*x_size+j-2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1672,22,"susan_corners",(void *)(r),(void *)(r+(i-3)*x_size+j-1),(i-3)*x_size+j-1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1673,22,"susan_corners",(void *)(r),(void *)(r+(i-3)*x_size+j ),(i-3)*x_size+j  )]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1674,22,"susan_corners",(void *)(r),(void *)(r+(i-3)*x_size+j+1),(i-3)*x_size+j+1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1675,22,"susan_corners",(void *)(r),(void *)(r+(i-3)*x_size+j+2),(i-3)*x_size+j+2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1676,22,"susan_corners",(void *)(r),(void *)(r+(i-3)*x_size+j+3),(i-3)*x_size+j+3)]) &&

                (x>r[__boundcheck_ptr_cast_to_array_reference(1678,22,"susan_corners",(void *)(r),(void *)(r+(i-2)*x_size+j-3),(i-2)*x_size+j-3)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1679,22,"susan_corners",(void *)(r),(void *)(r+(i-2)*x_size+j-2),(i-2)*x_size+j-2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1680,22,"susan_corners",(void *)(r),(void *)(r+(i-2)*x_size+j-1),(i-2)*x_size+j-1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1681,22,"susan_corners",(void *)(r),(void *)(r+(i-2)*x_size+j ),(i-2)*x_size+j  )]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1682,22,"susan_corners",(void *)(r),(void *)(r+(i-2)*x_size+j+1),(i-2)*x_size+j+1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1683,22,"susan_corners",(void *)(r),(void *)(r+(i-2)*x_size+j+2),(i-2)*x_size+j+2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1684,22,"susan_corners",(void *)(r),(void *)(r+(i-2)*x_size+j+3),(i-2)*x_size+j+3)]) &&

                (x>r[__boundcheck_ptr_cast_to_array_reference(1686,22,"susan_corners",(void *)(r),(void *)(r+(i-1)*x_size+j-3),(i-1)*x_size+j-3)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1687,22,"susan_corners",(void *)(r),(void *)(r+(i-1)*x_size+j-2),(i-1)*x_size+j-2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1688,22,"susan_corners",(void *)(r),(void *)(r+(i-1)*x_size+j-1),(i-1)*x_size+j-1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1689,22,"susan_corners",(void *)(r),(void *)(r+(i-1)*x_size+j ),(i-1)*x_size+j  )]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1690,22,"susan_corners",(void *)(r),(void *)(r+(i-1)*x_size+j+1),(i-1)*x_size+j+1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1691,22,"susan_corners",(void *)(r),(void *)(r+(i-1)*x_size+j+2),(i-1)*x_size+j+2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1692,22,"susan_corners",(void *)(r),(void *)(r+(i-1)*x_size+j+3),(i-1)*x_size+j+3)]) &&

                (x>r[__boundcheck_ptr_cast_to_array_reference(1694,22,"susan_corners",(void *)(r),(void *)(r+(i)*x_size+j-3),(i)*x_size+j-3)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1695,22,"susan_corners",(void *)(r),(void *)(r+(i)*x_size+j-2),(i)*x_size+j-2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1696,22,"susan_corners",(void *)(r),(void *)(r+(i)*x_size+j-1),(i)*x_size+j-1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1697,23,"susan_corners",(void *)(r),(void *)(r+(i)*x_size+j+1),(i)*x_size+j+1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1698,23,"susan_corners",(void *)(r),(void *)(r+(i)*x_size+j+2),(i)*x_size+j+2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1699,23,"susan_corners",(void *)(r),(void *)(r+(i)*x_size+j+3),(i)*x_size+j+3)]) &&

                (x>=r[__boundcheck_ptr_cast_to_array_reference(1701,23,"susan_corners",(void *)(r),(void *)(r+(i+1)*x_size+j-3),(i+1)*x_size+j-3)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1702,23,"susan_corners",(void *)(r),(void *)(r+(i+1)*x_size+j-2),(i+1)*x_size+j-2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1703,23,"susan_corners",(void *)(r),(void *)(r+(i+1)*x_size+j-1),(i+1)*x_size+j-1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1704,23,"susan_corners",(void *)(r),(void *)(r+(i+1)*x_size+j ),(i+1)*x_size+j  )]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1705,23,"susan_corners",(void *)(r),(void *)(r+(i+1)*x_size+j+1),(i+1)*x_size+j+1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1706,23,"susan_corners",(void *)(r),(void *)(r+(i+1)*x_size+j+2),(i+1)*x_size+j+2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1707,23,"susan_corners",(void *)(r),(void *)(r+(i+1)*x_size+j+3),(i+1)*x_size+j+3)]) &&

                (x>=r[__boundcheck_ptr_cast_to_array_reference(1709,23,"susan_corners",(void *)(r),(void *)(r+(i+2)*x_size+j-3),(i+2)*x_size+j-3)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1710,23,"susan_corners",(void *)(r),(void *)(r+(i+2)*x_size+j-2),(i+2)*x_size+j-2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1711,23,"susan_corners",(void *)(r),(void *)(r+(i+2)*x_size+j-1),(i+2)*x_size+j-1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1712,23,"susan_corners",(void *)(r),(void *)(r+(i+2)*x_size+j ),(i+2)*x_size+j  )]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1713,23,"susan_corners",(void *)(r),(void *)(r+(i+2)*x_size+j+1),(i+2)*x_size+j+1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1714,23,"susan_corners",(void *)(r),(void *)(r+(i+2)*x_size+j+2),(i+2)*x_size+j+2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1715,23,"susan_corners",(void *)(r),(void *)(r+(i+2)*x_size+j+3),(i+2)*x_size+j+3)]) &&

                (x>=r[__boundcheck_ptr_cast_to_array_reference(1717,23,"susan_corners",(void *)(r),(void *)(r+(i+3)*x_size+j-3),(i+3)*x_size+j-3)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1718,23,"susan_corners",(void *)(r),(void *)(r+(i+3)*x_size+j-2),(i+3)*x_size+j-2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1719,23,"susan_corners",(void *)(r),(void *)(r+(i+3)*x_size+j-1),(i+3)*x_size+j-1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1720,23,"susan_corners",(void *)(r),(void *)(r+(i+3)*x_size+j ),(i+3)*x_size+j  )]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1721,23,"susan_corners",(void *)(r),(void *)(r+(i+3)*x_size+j+1),(i+3)*x_size+j+1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1722,23,"susan_corners",(void *)(r),(void *)(r+(i+3)*x_size+j+2),(i+3)*x_size+j+2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1723,23,"susan_corners",(void *)(r),(void *)(r+(i+3)*x_size+j+3),(i+3)*x_size+j+3)]) )
#endif
{
corner_list[__boundcheck_ptr_cast_to_array_reference(1726,13,"susan_corners",(void *)(corner_list),(void *)(corner_list+n),n)].info=0;
corner_list[__boundcheck_ptr_cast_to_array_reference(1727,13,"susan_corners",(void *)(corner_list),(void *)(corner_list+n),n)].x=j;
corner_list[__boundcheck_ptr_cast_to_array_reference(1728,13,"susan_corners",(void *)(corner_list),(void *)(corner_list+n),n)].y=i;
corner_list[__boundcheck_ptr_cast_to_array_reference(1729,13,"susan_corners",(void *)(corner_list),(void *)(corner_list+n),n)].dx=cgx[__boundcheck_ptr_cast_to_array_reference(1729,23,"susan_corners",(void *)(cgx),(void *)(cgx+i*x_size+j),i*x_size+j)];
corner_list[__boundcheck_ptr_cast_to_array_reference(1730,13,"susan_corners",(void *)(corner_list),(void *)(corner_list+n),n)].dy=cgy[__boundcheck_ptr_cast_to_array_reference(1730,23,"susan_corners",(void *)(cgy),(void *)(cgy+i*x_size+j),i*x_size+j)];
corner_list[__boundcheck_ptr_cast_to_array_reference(1731,13,"susan_corners",(void *)(corner_list),(void *)(corner_list+n),n)].I=in[__boundcheck_ptr_cast_to_array_reference(1731,21,"susan_corners",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
n++;
if(n==MAX_CORNERS){
      fprintf(stderr,"Too many corners.\n");
      exit(1);
         }}}}
corner_list[__boundcheck_ptr_cast_to_array_reference(1737,13,"susan_corners",(void *)(corner_list),(void *)(corner_list+n),n)].info=7;

__boundcheck_free(cgx);
__boundcheck_free(cgy);

}

/* }}} */
/* {{{ susan_quick(in,r,sf,max_no,corner_list) */

susan_corners_quick(in,r,bp,max_no,corner_list,x_size,y_size)
  uchar       *in, *bp;
  int         *r, max_no, x_size, y_size;
  CORNER_LIST corner_list;
{
int   n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));
int x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));
int y;
__boundcheck_metadata_store((void *)(&y),(void *)((size_t)(&y)+sizeof(y)*8-1));
int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));

uchar *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
uchar * cp;
__boundcheck_metadata_store((void *)(&cp),(void *)((size_t)(&cp)+sizeof(cp)*8-1));


  memset (r,0,x_size * y_size * sizeof(int));

  for (i=7;i<y_size-7;i++)
    for (j=7;j<x_size-7;j++) {
        n=100;
        p=in + (i-3)*x_size + j - 1;
__boundcheck_metadata_trans_check((void *)(p),(void *)(in),(void *)(in + (i - 3) * x_size));

        cp=bp + in[__boundcheck_ptr_cast_to_array_reference(1761,20,"susan_corners_quick",(void *)(in),(void *)(in+i*x_size+j),i*x_size+j)];
__boundcheck_metadata_trans_check((void *)(cp),(void *)(bp),(void *)(bp + in[i * x_size + j]));


        n+=*(uchar *)(__boundcheck_ptr_reference(1763,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1763,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1764,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1764,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1765,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1765,18,"susan_corners_quick",(void *)(p),(void *)(p)))))));
        p+=x_size-3;

        n+=*(uchar *)(__boundcheck_ptr_reference(1768,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1768,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1769,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1769,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1770,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1770,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1771,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1771,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1772,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1772,18,"susan_corners_quick",(void *)(p),(void *)(p)))))));
        p+=x_size-5;

        n+=*(uchar *)(__boundcheck_ptr_reference(1775,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1775,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1776,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1776,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1777,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1777,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1778,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1778,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1779,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1779,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1780,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1780,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1781,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1781,18,"susan_corners_quick",(void *)(p),(void *)(p)))))));
        p+=x_size-6;

        n+=*(uchar *)(__boundcheck_ptr_reference(1784,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1784,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1785,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1785,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
        n+=*(uchar *)(__boundcheck_ptr_reference(1786,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1786,18,"susan_corners_quick",(void *)(p),(void *)(p)))))));
      if (n<max_no){
        p+=2;
        n+=*(uchar *)(__boundcheck_ptr_reference(1789,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1789,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1791,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1791,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1793,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1793,18,"susan_corners_quick",(void *)(p),(void *)(p)))))));
      if (n<max_no){
        p+=x_size-6;

        n+=*(uchar *)(__boundcheck_ptr_reference(1797,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1797,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1799,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1799,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1801,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1801,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1803,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1803,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1805,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1805,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1807,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1807,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1809,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1809,18,"susan_corners_quick",(void *)(p),(void *)(p)))))));
      if (n<max_no){
        p+=x_size-5;

        n+=*(uchar *)(__boundcheck_ptr_reference(1813,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1813,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1815,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1815,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1817,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1817,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1819,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1819,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1821,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1821,18,"susan_corners_quick",(void *)(p),(void *)(p)))))));
      if (n<max_no){
        p+=x_size-3;

        n+=*(uchar *)(__boundcheck_ptr_reference(1825,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1825,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1827,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1827,18,"susan_corners_quick",(void *)(p),(void *)(p++)))))));
      if (n<max_no){
        n+=*(uchar *)(__boundcheck_ptr_reference(1829,13,"susan_corners_quick",(void *)(cp),(void *)((cp-*(uchar *)(__boundcheck_ptr_reference(1829,18,"susan_corners_quick",(void *)(p),(void *)(p)))))));

        if (n<max_no)
          r[__boundcheck_ptr_cast_to_array_reference(1832,13,"susan_corners_quick",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)] = max_no-n;
}}}}}}}}}}}}}}}}}}}

  /* to locate the local maxima */
  n=0;
  for (i=7;i<y_size-7;i++)
    for (j=7;j<x_size-7;j++) {
       x = r[__boundcheck_ptr_cast_to_array_reference(1839,14,"susan_corners_quick",(void *)(r),(void *)(r+i*x_size+j),i*x_size+j)];
       if (x>0)  {
          /* 5x5 mask */
#ifdef FIVE_SUPP
          if (
              (x>r[(i-1)*x_size+j+2]) &&
              (x>r[(i  )*x_size+j+1]) &&
              (x>r[(i  )*x_size+j+2]) &&
              (x>r[(i+1)*x_size+j-1]) &&
              (x>r[(i+1)*x_size+j  ]) &&
              (x>r[(i+1)*x_size+j+1]) &&
              (x>r[(i+1)*x_size+j+2]) &&
              (x>r[(i+2)*x_size+j-2]) &&
              (x>r[(i+2)*x_size+j-1]) &&
              (x>r[(i+2)*x_size+j  ]) &&
              (x>r[(i+2)*x_size+j+1]) &&
              (x>r[(i+2)*x_size+j+2]) &&
              (x>=r[(i-2)*x_size+j-2]) &&
              (x>=r[(i-2)*x_size+j-1]) &&
              (x>=r[(i-2)*x_size+j  ]) &&
              (x>=r[(i-2)*x_size+j+1]) &&
              (x>=r[(i-2)*x_size+j+2]) &&
              (x>=r[(i-1)*x_size+j-2]) &&
              (x>=r[(i-1)*x_size+j-1]) &&
	      (x>=r[(i-1)*x_size+j  ]) &&
	      (x>=r[(i-1)*x_size+j+1]) &&
	      (x>=r[(i  )*x_size+j-2]) &&
	      (x>=r[(i  )*x_size+j-1]) &&
	      (x>=r[(i+1)*x_size+j-2]) )
#endif
#ifdef SEVEN_SUPP
          if ( 
                (x>r[__boundcheck_ptr_cast_to_array_reference(1871,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-3)*x_size+j-3),(i-3)*x_size+j-3)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1872,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-3)*x_size+j-2),(i-3)*x_size+j-2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1873,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-3)*x_size+j-1),(i-3)*x_size+j-1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1874,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-3)*x_size+j ),(i-3)*x_size+j  )]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1875,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-3)*x_size+j+1),(i-3)*x_size+j+1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1876,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-3)*x_size+j+2),(i-3)*x_size+j+2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1877,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-3)*x_size+j+3),(i-3)*x_size+j+3)]) &&

                (x>r[__boundcheck_ptr_cast_to_array_reference(1879,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-2)*x_size+j-3),(i-2)*x_size+j-3)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1880,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-2)*x_size+j-2),(i-2)*x_size+j-2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1881,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-2)*x_size+j-1),(i-2)*x_size+j-1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1882,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-2)*x_size+j ),(i-2)*x_size+j  )]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1883,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-2)*x_size+j+1),(i-2)*x_size+j+1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1884,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-2)*x_size+j+2),(i-2)*x_size+j+2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1885,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-2)*x_size+j+3),(i-2)*x_size+j+3)]) &&

                (x>r[__boundcheck_ptr_cast_to_array_reference(1887,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-1)*x_size+j-3),(i-1)*x_size+j-3)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1888,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-1)*x_size+j-2),(i-1)*x_size+j-2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1889,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-1)*x_size+j-1),(i-1)*x_size+j-1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1890,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-1)*x_size+j ),(i-1)*x_size+j  )]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1891,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-1)*x_size+j+1),(i-1)*x_size+j+1)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1892,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-1)*x_size+j+2),(i-1)*x_size+j+2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1893,22,"susan_corners_quick",(void *)(r),(void *)(r+(i-1)*x_size+j+3),(i-1)*x_size+j+3)]) &&

                (x>r[__boundcheck_ptr_cast_to_array_reference(1895,22,"susan_corners_quick",(void *)(r),(void *)(r+(i)*x_size+j-3),(i)*x_size+j-3)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1896,22,"susan_corners_quick",(void *)(r),(void *)(r+(i)*x_size+j-2),(i)*x_size+j-2)]) &&
                (x>r[__boundcheck_ptr_cast_to_array_reference(1897,22,"susan_corners_quick",(void *)(r),(void *)(r+(i)*x_size+j-1),(i)*x_size+j-1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1898,23,"susan_corners_quick",(void *)(r),(void *)(r+(i)*x_size+j+1),(i)*x_size+j+1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1899,23,"susan_corners_quick",(void *)(r),(void *)(r+(i)*x_size+j+2),(i)*x_size+j+2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1900,23,"susan_corners_quick",(void *)(r),(void *)(r+(i)*x_size+j+3),(i)*x_size+j+3)]) &&

                (x>=r[__boundcheck_ptr_cast_to_array_reference(1902,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+1)*x_size+j-3),(i+1)*x_size+j-3)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1903,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+1)*x_size+j-2),(i+1)*x_size+j-2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1904,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+1)*x_size+j-1),(i+1)*x_size+j-1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1905,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+1)*x_size+j ),(i+1)*x_size+j  )]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1906,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+1)*x_size+j+1),(i+1)*x_size+j+1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1907,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+1)*x_size+j+2),(i+1)*x_size+j+2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1908,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+1)*x_size+j+3),(i+1)*x_size+j+3)]) &&

                (x>=r[__boundcheck_ptr_cast_to_array_reference(1910,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+2)*x_size+j-3),(i+2)*x_size+j-3)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1911,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+2)*x_size+j-2),(i+2)*x_size+j-2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1912,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+2)*x_size+j-1),(i+2)*x_size+j-1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1913,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+2)*x_size+j ),(i+2)*x_size+j  )]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1914,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+2)*x_size+j+1),(i+2)*x_size+j+1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1915,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+2)*x_size+j+2),(i+2)*x_size+j+2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1916,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+2)*x_size+j+3),(i+2)*x_size+j+3)]) &&

                (x>=r[__boundcheck_ptr_cast_to_array_reference(1918,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+3)*x_size+j-3),(i+3)*x_size+j-3)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1919,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+3)*x_size+j-2),(i+3)*x_size+j-2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1920,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+3)*x_size+j-1),(i+3)*x_size+j-1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1921,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+3)*x_size+j ),(i+3)*x_size+j  )]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1922,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+3)*x_size+j+1),(i+3)*x_size+j+1)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1923,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+3)*x_size+j+2),(i+3)*x_size+j+2)]) &&
                (x>=r[__boundcheck_ptr_cast_to_array_reference(1924,23,"susan_corners_quick",(void *)(r),(void *)(r+(i+3)*x_size+j+3),(i+3)*x_size+j+3)]) )
#endif
{
corner_list[__boundcheck_ptr_cast_to_array_reference(1927,13,"susan_corners_quick",(void *)(corner_list),(void *)(corner_list+n),n)].info=0;
corner_list[__boundcheck_ptr_cast_to_array_reference(1928,13,"susan_corners_quick",(void *)(corner_list),(void *)(corner_list+n),n)].x=j;
corner_list[__boundcheck_ptr_cast_to_array_reference(1929,13,"susan_corners_quick",(void *)(corner_list),(void *)(corner_list+n),n)].y=i;
x = in[__boundcheck_ptr_cast_to_array_reference(1930,8,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j-2),(i-2)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1930,31,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j-1),(i-2)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1930,54,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j),(i-2)*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1930,75,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j+1),(i-2)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1930,98,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j+2),(i-2)*x_size+j+2)] +
    in[__boundcheck_ptr_cast_to_array_reference(1931,8,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j-2),(i-1)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1931,31,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j-1),(i-1)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1931,54,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j),(i-1)*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1931,75,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j+1),(i-1)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1931,98,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j+2),(i-1)*x_size+j+2)] +
    in[__boundcheck_ptr_cast_to_array_reference(1932,8,"susan_corners_quick",(void *)(in),(void *)(in+(i  )*x_size+j-2),(i  )*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1932,31,"susan_corners_quick",(void *)(in),(void *)(in+(i  )*x_size+j-1),(i  )*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1932,54,"susan_corners_quick",(void *)(in),(void *)(in+(i  )*x_size+j),(i  )*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1932,75,"susan_corners_quick",(void *)(in),(void *)(in+(i  )*x_size+j+1),(i  )*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1932,98,"susan_corners_quick",(void *)(in),(void *)(in+(i  )*x_size+j+2),(i  )*x_size+j+2)] +
    in[__boundcheck_ptr_cast_to_array_reference(1933,8,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j-2),(i+1)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1933,31,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j-1),(i+1)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1933,54,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j),(i+1)*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1933,75,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j+1),(i+1)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1933,98,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j+2),(i+1)*x_size+j+2)] +
    in[__boundcheck_ptr_cast_to_array_reference(1934,8,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j-2),(i+2)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1934,31,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j-1),(i+2)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1934,54,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j),(i+2)*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1934,75,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j+1),(i+2)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1934,98,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j+2),(i+2)*x_size+j+2)];

corner_list[__boundcheck_ptr_cast_to_array_reference(1936,13,"susan_corners_quick",(void *)(corner_list),(void *)(corner_list+n),n)].I=x/25;
/*corner_list[n].I=in[i*x_size+j];*/
x = in[__boundcheck_ptr_cast_to_array_reference(1938,8,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j+2),(i-2)*x_size+j+2)] + in[__boundcheck_ptr_cast_to_array_reference(1938,31,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j+2),(i-1)*x_size+j+2)] + in[__boundcheck_ptr_cast_to_array_reference(1938,54,"susan_corners_quick",(void *)(in),(void *)(in+(i)*x_size+j+2),(i)*x_size+j+2)] + in[__boundcheck_ptr_cast_to_array_reference(1938,75,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j+2),(i+1)*x_size+j+2)] + in[__boundcheck_ptr_cast_to_array_reference(1938,98,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j+2),(i+2)*x_size+j+2)] -
   (in[__boundcheck_ptr_cast_to_array_reference(1939,8,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j-2),(i-2)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1939,31,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j-2),(i-1)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1939,54,"susan_corners_quick",(void *)(in),(void *)(in+(i)*x_size+j-2),(i)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1939,75,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j-2),(i+1)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1939,98,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j-2),(i+2)*x_size+j-2)]);
x += x + in[__boundcheck_ptr_cast_to_array_reference(1940,13,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j+1),(i-2)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1940,36,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j+1),(i-1)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1940,59,"susan_corners_quick",(void *)(in),(void *)(in+(i)*x_size+j+1),(i)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1940,80,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j+1),(i+1)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1940,103,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j+1),(i+2)*x_size+j+1)] -
        (in[__boundcheck_ptr_cast_to_array_reference(1941,13,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j-1),(i-2)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1941,36,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j-1),(i-1)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1941,59,"susan_corners_quick",(void *)(in),(void *)(in+(i)*x_size+j-1),(i)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1941,80,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j-1),(i+1)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1941,103,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j-1),(i+2)*x_size+j-1)]);

y = in[__boundcheck_ptr_cast_to_array_reference(1943,8,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j-2),(i+2)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1943,31,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j-1),(i+2)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1943,54,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j),(i+2)*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1943,75,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j+1),(i+2)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1943,98,"susan_corners_quick",(void *)(in),(void *)(in+(i+2)*x_size+j+2),(i+2)*x_size+j+2)] -
   (in[__boundcheck_ptr_cast_to_array_reference(1944,8,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j-2),(i-2)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1944,31,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j-1),(i-2)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1944,54,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j),(i-2)*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1944,75,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j+1),(i-2)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1944,98,"susan_corners_quick",(void *)(in),(void *)(in+(i-2)*x_size+j+2),(i-2)*x_size+j+2)]);
y += y + in[__boundcheck_ptr_cast_to_array_reference(1945,13,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j-2),(i+1)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1945,36,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j-1),(i+1)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1945,59,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j),(i+1)*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1945,80,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j+1),(i+1)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1945,103,"susan_corners_quick",(void *)(in),(void *)(in+(i+1)*x_size+j+2),(i+1)*x_size+j+2)] -
        (in[__boundcheck_ptr_cast_to_array_reference(1946,13,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j-2),(i-1)*x_size+j-2)] + in[__boundcheck_ptr_cast_to_array_reference(1946,36,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j-1),(i-1)*x_size+j-1)] + in[__boundcheck_ptr_cast_to_array_reference(1946,59,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j),(i-1)*x_size+j)] + in[__boundcheck_ptr_cast_to_array_reference(1946,80,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j+1),(i-1)*x_size+j+1)] + in[__boundcheck_ptr_cast_to_array_reference(1946,103,"susan_corners_quick",(void *)(in),(void *)(in+(i-1)*x_size+j+2),(i-1)*x_size+j+2)]);
corner_list[__boundcheck_ptr_cast_to_array_reference(1947,13,"susan_corners_quick",(void *)(corner_list),(void *)(corner_list+n),n)].dx=x/15;
corner_list[__boundcheck_ptr_cast_to_array_reference(1948,13,"susan_corners_quick",(void *)(corner_list),(void *)(corner_list+n),n)].dy=y/15;
n++;
if(n==MAX_CORNERS){
      fprintf(stderr,"Too many corners.\n");
      exit(1);
         }}}}
corner_list[__boundcheck_ptr_cast_to_array_reference(1954,13,"susan_corners_quick",(void *)(corner_list),(void *)(corner_list+n),n)].info=7;
}

/* }}} */

/* }}} */
/* {{{ main(argc, argv) */

main(argc, argv)
  int   argc;
  char  *argv [];
{
/* {{{ vars */

FILE   *ofp;
__boundcheck_metadata_store((void *)(&ofp),(void *)((size_t)(&ofp)+sizeof(ofp)*8-1));

char   filename [80];
__boundcheck_metadata_store((void *)(&filename),(void *)((size_t)(&filename)+sizeof(filename)*8-1));

       char * tcp;
__boundcheck_metadata_store((void *)(&tcp),(void *)((size_t)(&tcp)+sizeof(tcp)*8-1));

uchar  *in;
__boundcheck_metadata_store((void *)(&in),(void *)((size_t)(&in)+sizeof(in)*8-1));
 uchar * bp;
__boundcheck_metadata_store((void *)(&bp),(void *)((size_t)(&bp)+sizeof(bp)*8-1));
 uchar * mid;
__boundcheck_metadata_store((void *)(&mid),(void *)((size_t)(&mid)+sizeof(mid)*8-1));

float  dt=4.0;
__boundcheck_metadata_store((void *)(&dt),(void *)((size_t)(&dt)+sizeof(dt)*8-1));

int    *r;
__boundcheck_metadata_store((void *)(&r),(void *)((size_t)(&r)+sizeof(r)*8-1));

      int argindex=3;
__boundcheck_metadata_store((void *)(&argindex),(void *)((size_t)(&argindex)+sizeof(argindex)*8-1));

      int bt=20;
__boundcheck_metadata_store((void *)(&bt),(void *)((size_t)(&bt)+sizeof(bt)*8-1));

      int principle=0;
__boundcheck_metadata_store((void *)(&principle),(void *)((size_t)(&principle)+sizeof(principle)*8-1));

      int thin_post_proc=1;
__boundcheck_metadata_store((void *)(&thin_post_proc),(void *)((size_t)(&thin_post_proc)+sizeof(thin_post_proc)*8-1));

      int three_by_three=0;
__boundcheck_metadata_store((void *)(&three_by_three),(void *)((size_t)(&three_by_three)+sizeof(three_by_three)*8-1));

      int drawing_mode=0;
__boundcheck_metadata_store((void *)(&drawing_mode),(void *)((size_t)(&drawing_mode)+sizeof(drawing_mode)*8-1));

      int susan_quick=0;
__boundcheck_metadata_store((void *)(&susan_quick),(void *)((size_t)(&susan_quick)+sizeof(susan_quick)*8-1));

      int max_no_corners=1850;
__boundcheck_metadata_store((void *)(&max_no_corners),(void *)((size_t)(&max_no_corners)+sizeof(max_no_corners)*8-1));

      int max_no_edges=2650;
__boundcheck_metadata_store((void *)(&max_no_edges),(void *)((size_t)(&max_no_edges)+sizeof(max_no_edges)*8-1));

      int mode = 0;
__boundcheck_metadata_store((void *)(&mode),(void *)((size_t)(&mode)+sizeof(mode)*8-1));
int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

      int x_size;
__boundcheck_metadata_store((void *)(&x_size),(void *)((size_t)(&x_size)+sizeof(x_size)*8-1));
int y_size;
__boundcheck_metadata_store((void *)(&y_size),(void *)((size_t)(&y_size)+sizeof(y_size)*8-1));

CORNER_LIST corner_list;
__boundcheck_metadata_store((void *)(&corner_list),(void *)((size_t)(&corner_list)+sizeof(corner_list)*8-1));


/* }}} */

  if (argc<3)
    usage();

  get_image(argv[1],&in,&x_size,&y_size);

  /* {{{ look at options */

  while (argindex < argc)
  {
    tcp = argv[argindex];
    if (*tcp == '-')
      switch (*++tcp)
      {
        case 's': /* smoothing */
          mode=0;
	  break;
        case 'e': /* edges */
          mode=1;
	  break;
        case 'c': /* corners */
          mode=2;
	  break;
        case 'p': /* principle */
          principle=1;
	  break;
        case 'n': /* thinning post processing */
          thin_post_proc=0;
	  break;
        case 'b': /* simple drawing mode */
          drawing_mode=1;
	  break;
        case '3': /* 3x3 flat mask */
          three_by_three=1;
	  break;
        case 'q': /* quick susan mask */
          susan_quick=1;
	  break;
	case 'd': /* distance threshold */
          if (++argindex >= argc){
	    printf ("No argument following -d\n");
	    exit(0);}
	  dt=atof(argv[argindex]);
          if (dt<0) three_by_three=1;
	  break;
	case 't': /* brightness threshold */
          if (++argindex >= argc){
	    printf ("No argument following -t\n");
	    exit(0);}
	  bt=atoi(argv[argindex]);
	  break;
      }	    
      else
        usage();
    argindex++;
  }

  if ( (principle==1) && (mode==0) )
    mode=1;

/* }}} */
  /* {{{ main processing */

  switch (mode)
  {
    case 0:
      /* {{{ smoothing */

      setup_brightness_lut(&bp,bt,2);
      susan_smoothing(three_by_three,in,dt,x_size,y_size,bp);
      break;

/* }}} */
    case 1:
      /* {{{ edges */

      r   = (int *) __boundcheck_malloc(x_size * y_size * sizeof(int));
      setup_brightness_lut(&bp,bt,6);

      if (principle)
      {
        if (three_by_three)
          susan_principle_small(in,r,bp,max_no_edges,x_size,y_size);
        else
          susan_principle(in,r,bp,max_no_edges,x_size,y_size);
        int_to_uchar(r,in,x_size*y_size);
      }
      else
      {
        mid = (uchar *)__boundcheck_malloc(x_size*y_size);
        memset (mid,100,x_size * y_size); /* note not set to zero */

        if (three_by_three)
          susan_edges_small(in,r,mid,bp,max_no_edges,x_size,y_size);
        else
          susan_edges(in,r,mid,bp,max_no_edges,x_size,y_size);
        if(thin_post_proc)
          susan_thin(r,mid,x_size,y_size);
        edge_draw(in,mid,x_size,y_size,drawing_mode);
      }

      break;

/* }}} */
    case 2:
      /* {{{ corners */

      r   = (int *) __boundcheck_malloc(x_size * y_size * sizeof(int));
      setup_brightness_lut(&bp,bt,6);

      if (principle)
      {
        susan_principle(in,r,bp,max_no_corners,x_size,y_size);
        int_to_uchar(r,in,x_size*y_size);
      }
      else
      {
        if(susan_quick)
          susan_corners_quick(in,r,bp,max_no_corners,corner_list,x_size,y_size);
        else
          susan_corners(in,r,bp,max_no_corners,corner_list,x_size,y_size);
        corner_draw(in,corner_list,x_size,drawing_mode);
      }

      break;

/* }}} */
  }    

/* }}} */

  put_image(argv[2],in,x_size,y_size);
}

/* }}} */
