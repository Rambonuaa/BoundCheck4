/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/src/add.c,v 1.1.1.1 2000/11/06 19:54:25 mguthaus Exp $ */

/*
 *  See private.h for the more commonly used macro versions.
 */

#include	<stdio.h>
#include	<assert.h>

#include	"private.h"
#include	"gsm.h"
#include	"proto.h"

#define	saturate(x) 	\
	((x) < MIN_WORD ? MIN_WORD : (x) > MAX_WORD ? MAX_WORD: (x))

word gsm_add P2((a,b), word a, word b)
{
	longword sum = (longword)a + (longword)b;
__boundcheck_metadata_store((void *)(&sum),(void *)((size_t)(&sum)+sizeof(sum)*8-1));

__boundcheck_metadata_store((void *)(&sum),(void *)((size_t)(&sum)+sizeof(sum)*8-1));

	return saturate(sum);
}

word gsm_sub P2((a,b), word a, word b)
{
	longword diff = (longword)a - (longword)b;
__boundcheck_metadata_store((void *)(&diff),(void *)((size_t)(&diff)+sizeof(diff)*8-1));

__boundcheck_metadata_store((void *)(&diff),(void *)((size_t)(&diff)+sizeof(diff)*8-1));

	return saturate(diff);
}

word gsm_mult P2((a,b), word a, word b)
{
	if (a == MIN_WORD && b == MIN_WORD) return MAX_WORD;
	else return SASR( (longword)a * (longword)b, 15 );
}

word gsm_mult_r P2((a,b), word a, word b)
{
	if (b == MIN_WORD && a == MIN_WORD) return MAX_WORD;
	else {
		longword prod = (longword)a * (longword)b + 16384;
__boundcheck_metadata_store((void *)(&prod),(void *)((size_t)(&prod)+sizeof(prod)*8-1));

__boundcheck_metadata_store((void *)(&prod),(void *)((size_t)(&prod)+sizeof(prod)*8-1));

		prod >>= 15;
		return prod & 0xFFFF;
	}
}

word gsm_abs P1((a), word a)
{
	return a < 0 ? (a == MIN_WORD ? MAX_WORD : -a) : a;
}

longword gsm_L_mult P2((a,b),word a, word b)
{
	assert( a != MIN_WORD || b != MIN_WORD );
	return ((longword)a * (longword)b) << 1;
}

longword gsm_L_add P2((a,b), longword a, longword b)
{
	if (a < 0) {
		if (b >= 0) return a + b;
		else {
			ulongword A = (ulongword)-(a + 1) + (ulongword)-(b + 1);
__boundcheck_metadata_store((void *)(&A),(void *)((size_t)(&A)+sizeof(A)*8-1));

__boundcheck_metadata_store((void *)(&A),(void *)((size_t)(&A)+sizeof(A)*8-1));

			return A >= MAX_LONGWORD ? MIN_LONGWORD :-(longword)A-2;
		}
	}
	else if (b <= 0) return a + b;
	else {
		ulongword A = (ulongword)a + (ulongword)b;
__boundcheck_metadata_store((void *)(&A),(void *)((size_t)(&A)+sizeof(A)*8-1));

__boundcheck_metadata_store((void *)(&A),(void *)((size_t)(&A)+sizeof(A)*8-1));

		return A > MAX_LONGWORD ? MAX_LONGWORD : A;
	}
}

longword gsm_L_sub P2((a,b), longword a, longword b)
{
	if (a >= 0) {
		if (b >= 0) return a - b;
		else {
			/* a>=0, b<0 */

			ulongword A = (ulongword)a + -(b + 1);
__boundcheck_metadata_store((void *)(&A),(void *)((size_t)(&A)+sizeof(A)*8-1));

__boundcheck_metadata_store((void *)(&A),(void *)((size_t)(&A)+sizeof(A)*8-1));

			return A >= MAX_LONGWORD ? MAX_LONGWORD : (A + 1);
		}
	}
	else if (b <= 0) return a - b;
	else {
		/* a<0, b>0 */  

		ulongword A = (ulongword)-(a + 1) + b;
__boundcheck_metadata_store((void *)(&A),(void *)((size_t)(&A)+sizeof(A)*8-1));

__boundcheck_metadata_store((void *)(&A),(void *)((size_t)(&A)+sizeof(A)*8-1));

		return A >= MAX_LONGWORD ? MIN_LONGWORD : -(longword)A - 1;
	}
}

static unsigned char bitoff[ 256 ] = {
	 8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
	 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

word gsm_norm P1((a), longword a )
/*
 * the number of left shifts needed to normalize the 32 bit
 * variable L_var1 for positive values on the interval
 *
 * with minimum of
 * minimum of 1073741824  (01000000000000000000000000000000) and 
 * maximum of 2147483647  (01111111111111111111111111111111)
 *
 *
 * and for negative values on the interval with
 * minimum of -2147483648 (-10000000000000000000000000000000) and
 * maximum of -1073741824 ( -1000000000000000000000000000000).
 *
 * in order to normalize the result, the following
 * operation must be done: L_norm_var1 = L_var1 << norm( L_var1 );
 *
 * (That's 'ffs', only from the left, not the right..)
 */
{
	assert(a != 0);

	if (a < 0) {
		if (a <= -1073741824) return 0;
		a = ~a;
	}

	return    a & 0xffff0000 
		? ( a & 0xff000000
		  ?  -1 + bitoff[ _RV_insert_check(0,256,160,21,"gsm_norm",_RV_insert_check(0,256,146,21,"gsm_norm",0xFF & (a >> 24) ))]
		  :   7 + bitoff[ _RV_insert_check(0,256,161,21,"gsm_norm",_RV_insert_check(0,256,147,21,"gsm_norm",0xFF & (a >> 16) ))] )
		: ( a & 0xff00
		  ?  15 + bitoff[ _RV_insert_check(0,256,163,21,"gsm_norm",_RV_insert_check(0,256,149,21,"gsm_norm",0xFF & (a >> 8) ))]
		  :  23 + bitoff[ _RV_insert_check(0,256,164,21,"gsm_norm",_RV_insert_check(0,256,150,21,"gsm_norm",0xFF & a ))] );
}

longword gsm_L_asl P2((a,n), longword a, int n)
{
	if (n >= 32) return 0;
	if (n <= -32) return -(a < 0);
	if (n < 0) return gsm_L_asr(a, -n);
	return a << n;
}

word gsm_asl P2((a,n), word a, int n)
{
	if (n >= 16) return 0;
	if (n <= -16) return -(a < 0);
	if (n < 0) return gsm_asr(a, -n);
	return a << n;
}

longword gsm_L_asr P2((a,n), longword a, int n)
{
	if (n >= 32) return -(a < 0);
	if (n <= -32) return 0;
	if (n < 0) return a << -n;

#	ifdef	SASR
		return a >> n;
#	else
		if (a >= 0) return a >> n;
		else return -(longword)( -(ulongword)a >> n );
#	endif
}

word gsm_asr P2((a,n), word a, int n)
{
	if (n >= 16) return -(a < 0);
	if (n <= -16) return 0;
	if (n < 0) return a << -n;

#	ifdef	SASR
		return a >> n;
#	else
		if (a >= 0) return a >> n;
		else return -(word)( -(uword)a >> n );
#	endif
}

/* 
 *  (From p. 46, end of section 4.2.5)
 *
 *  NOTE: The following lines gives [sic] one correct implementation
 *  	  of the div(num, denum) arithmetic operation.  Compute div
 *        which is the integer division of num by denum: with denum
 *	  >= num > 0
 */

word gsm_div P2((num,denum), word num, word denum)
{
	longword	L_num   = num;
__boundcheck_metadata_store((void *)(&L_num),(void *)((size_t)(&L_num)+sizeof(L_num)*8-1));

__boundcheck_metadata_store((void *)(&L_num),(void *)((size_t)(&L_num)+sizeof(L_num)*8-1));

	longword	L_denum = denum;
__boundcheck_metadata_store((void *)(&L_denum),(void *)((size_t)(&L_denum)+sizeof(L_denum)*8-1));

__boundcheck_metadata_store((void *)(&L_denum),(void *)((size_t)(&L_denum)+sizeof(L_denum)*8-1));

	word		div 	= 0;
__boundcheck_metadata_store((void *)(&div),(void *)((size_t)(&div)+sizeof(div)*8-1));

__boundcheck_metadata_store((void *)(&div),(void *)((size_t)(&div)+sizeof(div)*8-1));

	int		k 	= 15;
__boundcheck_metadata_store((void *)(&k),(void *)((size_t)(&k)+sizeof(k)*8-1));

__boundcheck_metadata_store((void *)(&k),(void *)((size_t)(&k)+sizeof(k)*8-1));


	/* The parameter num sometimes becomes zero.
	 * Although this is explicitly guarded against in 4.2.5,
	 * we assume that the result should then be zero as well.
	 */

	/* assert(num != 0); */

	assert(num >= 0 && denum >= num);
	if (num == 0)
	    return 0;

	while (k--) {
		div   <<= 1;
		L_num <<= 1;

		if (L_num >= L_denum) {
			L_num -= L_denum;
			div++;
		}
	}

	return div;
}
