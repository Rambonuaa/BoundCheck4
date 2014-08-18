/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/src/preprocess.c,v 1.1.1.1 2000/11/06 19:54:26 mguthaus Exp $ */

#include	<stdio.h>
#include	<assert.h>

#include "private.h"

#include	"gsm.h"
#include 	"proto.h"

/*	4.2.0 .. 4.2.3	PREPROCESSING SECTION
 *  
 *  	After A-law to linear conversion (or directly from the
 *   	Ato D converter) the following scaling is assumed for
 * 	input to the RPE-LTP algorithm:
 *
 *      in:  0.1.....................12
 *	     S.v.v.v.v.v.v.v.v.v.v.v.v.*.*.*
 *
 *	Where S is the sign bit, v a valid bit, and * a "don't care" bit.
 * 	The original signal is called sop[..]
 *
 *      out:   0.1................... 12 
 *	     S.S.v.v.v.v.v.v.v.v.v.v.v.v.0.0
 */


void Gsm_Preprocess P3((S, s, so),
	struct gsm_state * S,
	word		 * s,
	word 		 * so )		/* [0..159] 	IN/OUT	*/
{

	word       z1 = S->z1;
__boundcheck_metadata_store((void *)(&z1),(void *)((size_t)(&z1)+sizeof(z1)*8-1));

	longword L_z2 = S->L_z2;
__boundcheck_metadata_store((void *)(&L_z2),(void *)((size_t)(&L_z2)+sizeof(L_z2)*8-1));

	word 	   mp = S->mp;
__boundcheck_metadata_store((void *)(&mp),(void *)((size_t)(&mp)+sizeof(mp)*8-1));


	word 	   	s1;
__boundcheck_metadata_store((void *)(&s1),(void *)((size_t)(&s1)+sizeof(s1)*8-1));

	longword      L_s2;
__boundcheck_metadata_store((void *)(&L_s2),(void *)((size_t)(&L_s2)+sizeof(L_s2)*8-1));


	longword      L_temp;
__boundcheck_metadata_store((void *)(&L_temp),(void *)((size_t)(&L_temp)+sizeof(L_temp)*8-1));


	word		msp;
__boundcheck_metadata_store((void *)(&msp),(void *)((size_t)(&msp)+sizeof(msp)*8-1));
word lsp;
__boundcheck_metadata_store((void *)(&lsp),(void *)((size_t)(&lsp)+sizeof(lsp)*8-1));

	word		SO;
__boundcheck_metadata_store((void *)(&SO),(void *)((size_t)(&SO)+sizeof(SO)*8-1));


	longword	ltmp;
__boundcheck_metadata_store((void *)(&ltmp),(void *)((size_t)(&ltmp)+sizeof(ltmp)*8-1));
		/* for   ADD */
	ulongword	utmp;
__boundcheck_metadata_store((void *)(&utmp),(void *)((size_t)(&utmp)+sizeof(utmp)*8-1));
		/* for L_ADD */

	register int		k = 160;

	while (k--) {

	/*  4.2.1   Downscaling of the input signal
	 */
		SO = SASR( *(word *)(__boundcheck_ptr_reference(61,15,"Gsm_Preprocess",(void *)(s),(void *)(s))), 3 ) << 2;
		s++;

		assert (SO >= -0x4000);	/* downscaled by     */
		assert (SO <=  0x3FFC);	/* previous routine. */


	/*  4.2.2   Offset compensation
	 * 
	 *  This part implements a high-pass filter and requires extended
	 *  arithmetic precision for the recursive part of this filter.
	 *  The input of this procedure is the array so[0...159] and the
	 *  output the array sof[ 0...159 ].
	 */
		/*   Compute the non-recursive part
		 */

		s1 = SO - z1;			/* s1 = gsm_sub( *so, z1 ); */
		z1 = SO;

		assert(s1 != MIN_WORD);

		/*   Compute the recursive part
		 */
		L_s2 = s1;
		L_s2 <<= 15;

		/*   Execution of a 31 bv 16 bits multiplication
		 */

		msp = SASR( L_z2, 15 );
		lsp = L_z2-((longword)msp<<15); /* gsm_L_sub(L_z2,(msp<<15)); */

		L_s2  += GSM_MULT_R( lsp, 32735 );
		L_temp = (longword)msp * 32735; /* GSM_L_MULT(msp,32735) >> 1;*/
		L_z2   = GSM_L_ADD( L_temp, L_s2 );

		/*    Compute sof[k] with rounding
		 */
		L_temp = GSM_L_ADD( L_z2, 16384 );

	/*   4.2.3  Preemphasis
	 */

		msp   = GSM_MULT_R( mp, -28180 );
		mp    = SASR( L_temp, 15 );
		*(word *)(__boundcheck_ptr_reference(107,4,"Gsm_Preprocess",(void *)(so),(void *)(so++))) = GSM_ADD( mp, msp );
	}

	S->z1   = z1;
	S->L_z2 = L_z2;
	S->mp   = mp;
}
