/*============================================================================

    fourierf.c  -  Don Cross <dcross@intersrv.com>

    http://www.intersrv.com/~dcross/fft.html

    Contains definitions for doing Fourier transforms
    and inverse Fourier transforms.

    This module performs operations on arrays of 'float'.

    Revision history:

1998 September 19 [Don Cross]
    Updated coding standards.
    Improved efficiency of trig calculations.

============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "fourier.h"
#include "ddcmath.h"

#define CHECKPOINTER(p)  CheckPointer(p,#p)

static void CheckPointer ( void *p, char *name )
{
    if ( p == NULL )
    {
        fprintf ( stderr, "Error in fft_float():  %s == NULL\n", name );
        exit(1);
    }
}


void fft_float (
    unsigned  NumSamples,
    int       InverseTransform,
    float    *RealIn,
    float    *ImagIn,
    float    *RealOut,
    float    *ImagOut )
{
    unsigned NumBits;
__boundcheck_metadata_store(&(NumBits),&(NumBits));
    /* Number of bits needed to store indices */
    unsigned i;
__boundcheck_metadata_store(&(i),&(i));
unsigned int j;
__boundcheck_metadata_store(&(j),&(j));
unsigned int k;
__boundcheck_metadata_store(&(k),&(k));
unsigned int n;
__boundcheck_metadata_store(&(n),&(n));

    unsigned BlockSize;
__boundcheck_metadata_store(&(BlockSize),&(BlockSize));
unsigned int BlockEnd;
__boundcheck_metadata_store(&(BlockEnd),&(BlockEnd));


    double angle_numerator = 2.0 * DDC_PI;
__boundcheck_metadata_store(&(angle_numerator),&(angle_numerator));

    double tr;
__boundcheck_metadata_store(&(tr),&(tr));
double ti;
__boundcheck_metadata_store(&(ti),&(ti));
     /* temp real, temp imaginary */

    if ( !IsPowerOfTwo(NumSamples) )
    {
        fprintf (
            stderr,
            "Error in fft():  NumSamples=%u is not power of two\n",
            NumSamples );

        exit(1);
    }

    if ( InverseTransform )
        angle_numerator = -angle_numerator;

    CHECKPOINTER ( RealIn );
    CHECKPOINTER ( RealOut );
    CHECKPOINTER ( ImagOut );

    NumBits = NumberOfBitsNeeded ( NumSamples );

    /*
    **   Do simultaneous data copy and bit-reversal ordering into outputs...
    */

    for ( i=0; i < NumSamples; i++ )
    {
        j = ReverseBits ( i, NumBits );
        RealOut[__boundcheck_ptr_cast_to_array_reference(80,17,"fft_float",(void *)(RealOut),(void *)(RealOut+j),j)] = RealIn[__boundcheck_ptr_cast_to_array_reference(80,29,"fft_float",(void *)(RealIn),(void *)(RealIn+i),i)];
        ImagOut[__boundcheck_ptr_cast_to_array_reference(81,17,"fft_float",(void *)(ImagOut),(void *)(ImagOut+j),j)] = (ImagIn == NULL) ? 0.0 : ImagIn[__boundcheck_ptr_cast_to_array_reference(81,54,"fft_float",(void *)(ImagIn),(void *)(ImagIn+i),i)];
    }

    /*
    **   Do the FFT itself...
    */

    BlockEnd = 1;
    for ( BlockSize = 2; BlockSize <= NumSamples; BlockSize <<= 1 )
    {
        double delta_angle = angle_numerator / (double)BlockSize;
__boundcheck_metadata_store(&(delta_angle),&(delta_angle));

        double sm2 = sin ( -2 * delta_angle );
__boundcheck_metadata_store(&(sm2),&(sm2));

        double sm1 = sin ( -delta_angle );
__boundcheck_metadata_store(&(sm1),&(sm1));

        double cm2 = cos ( -2 * delta_angle );
__boundcheck_metadata_store(&(cm2),&(cm2));

        double cm1 = cos ( -delta_angle );
__boundcheck_metadata_store(&(cm1),&(cm1));

        double w = 2 * cm1;
__boundcheck_metadata_store(&(w),&(w));

        double ar[3];
__boundcheck_metadata_store((void *)(ar),(void *)(ar+sizeof(ar)-1));
double ai[3];
__boundcheck_metadata_store((void *)(ai),(void *)(ai+sizeof(ai)-1));

        double temp;
__boundcheck_metadata_store(&(temp),&(temp));


        for ( i=0; i < NumSamples; i += BlockSize )
        {
            ar[_RV_insert_check(0,3,102,16,"fft_float",2)] = cm2;
            ar[_RV_insert_check(0,3,103,16,"fft_float",1)] = cm1;

            ai[_RV_insert_check(0,3,105,16,"fft_float",2)] = sm2;
            ai[_RV_insert_check(0,3,106,16,"fft_float",1)] = sm1;

            for ( j=i, n=0; n < BlockEnd; j++, n++ )
            {
                ar[_RV_insert_check(0,3,110,20,"fft_float",0)] = w*ar[_RV_insert_check(0,3,110,30,"fft_float",1)] - ar[_RV_insert_check(0,3,110,38,"fft_float",2)];
                ar[_RV_insert_check(0,3,111,20,"fft_float",2)] = ar[_RV_insert_check(0,3,111,28,"fft_float",1)];
                ar[_RV_insert_check(0,3,112,20,"fft_float",1)] = ar[_RV_insert_check(0,3,112,28,"fft_float",0)];

                ai[_RV_insert_check(0,3,114,20,"fft_float",0)] = w*ai[_RV_insert_check(0,3,114,30,"fft_float",1)] - ai[_RV_insert_check(0,3,114,38,"fft_float",2)];
                ai[_RV_insert_check(0,3,115,20,"fft_float",2)] = ai[_RV_insert_check(0,3,115,28,"fft_float",1)];
                ai[_RV_insert_check(0,3,116,20,"fft_float",1)] = ai[_RV_insert_check(0,3,116,28,"fft_float",0)];

                k = j + BlockEnd;
                tr = ar[_RV_insert_check(0,3,119,25,"fft_float",0)]*RealOut[__boundcheck_ptr_cast_to_array_reference(119,36,"fft_float",(void *)(RealOut),(void *)(RealOut+k),k)] - ai[_RV_insert_check(0,3,119,44,"fft_float",0)]*ImagOut[__boundcheck_ptr_cast_to_array_reference(119,55,"fft_float",(void *)(ImagOut),(void *)(ImagOut+k),k)];
                ti = ar[_RV_insert_check(0,3,120,25,"fft_float",0)]*ImagOut[__boundcheck_ptr_cast_to_array_reference(120,36,"fft_float",(void *)(ImagOut),(void *)(ImagOut+k),k)] + ai[_RV_insert_check(0,3,120,44,"fft_float",0)]*RealOut[__boundcheck_ptr_cast_to_array_reference(120,55,"fft_float",(void *)(RealOut),(void *)(RealOut+k),k)];

                RealOut[__boundcheck_ptr_cast_to_array_reference(122,25,"fft_float",(void *)(RealOut),(void *)(RealOut+k),k)] = RealOut[__boundcheck_ptr_cast_to_array_reference(122,38,"fft_float",(void *)(RealOut),(void *)(RealOut+j),j)] - tr;
                ImagOut[__boundcheck_ptr_cast_to_array_reference(123,25,"fft_float",(void *)(ImagOut),(void *)(ImagOut+k),k)] = ImagOut[__boundcheck_ptr_cast_to_array_reference(123,38,"fft_float",(void *)(ImagOut),(void *)(ImagOut+j),j)] - ti;

                RealOut[__boundcheck_ptr_cast_to_array_reference(125,25,"fft_float",(void *)(RealOut),(void *)(RealOut+j),j)] += tr;
                ImagOut[__boundcheck_ptr_cast_to_array_reference(126,25,"fft_float",(void *)(ImagOut),(void *)(ImagOut+j),j)] += ti;
            }
        }

        BlockEnd = BlockSize;
    }

    /*
    **   Need to normalize if inverse transform...
    */

    if ( InverseTransform )
    {
        double denom = (double)NumSamples;
__boundcheck_metadata_store(&(denom),&(denom));


        for ( i=0; i < NumSamples; i++ )
        {
            RealOut[__boundcheck_ptr_cast_to_array_reference(143,21,"fft_float",(void *)(RealOut),(void *)(RealOut+i),i)] /= denom;
            ImagOut[__boundcheck_ptr_cast_to_array_reference(144,21,"fft_float",(void *)(ImagOut),(void *)(ImagOut+i),i)] /= denom;
        }
    }
}


/*--- end of file fourierf.c ---*/
