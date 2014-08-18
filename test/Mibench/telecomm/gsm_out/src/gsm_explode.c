/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/src/gsm_explode.c,v 1.1.1.1 2000/11/06 19:54:26 mguthaus Exp $ */

#include "private.h"
#include "gsm.h"
#include "proto.h"

int gsm_explode P3((s, c, target), gsm s, gsm_byte * c, gsm_signal * target)
{
	/* GSM_MAGIC  = (*c >> 4) & 0xF; */

	if (((*(gsm_byte *)(__boundcheck_ptr_reference(17,9,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x0F) != GSM_MAGIC) return -1;

#define	LARc	target

	LARc[__boundcheck_ptr_cast_to_array_reference(21,7,"gsm_explode",(void *)(target),(void *)(target+0),0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(21,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0xF) << 2;		/* 1 */
	LARc[__boundcheck_ptr_cast_to_array_reference(22,7,"gsm_explode",(void *)(target),(void *)(target+0),0)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(22,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[__boundcheck_ptr_cast_to_array_reference(23,7,"gsm_explode",(void *)(target),(void *)(target+1),1)]  = *(gsm_byte *)(__boundcheck_ptr_reference(23,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x3F;
	LARc[__boundcheck_ptr_cast_to_array_reference(24,7,"gsm_explode",(void *)(target),(void *)(target+2),2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(24,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x1F;
	LARc[__boundcheck_ptr_cast_to_array_reference(25,7,"gsm_explode",(void *)(target),(void *)(target+3),3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(25,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7) << 2;
	LARc[__boundcheck_ptr_cast_to_array_reference(26,7,"gsm_explode",(void *)(target),(void *)(target+3),3)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(26,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[__boundcheck_ptr_cast_to_array_reference(27,7,"gsm_explode",(void *)(target),(void *)(target+4),4)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(27,15,"gsm_explode",(void *)(c),(void *)(c))) >> 2) & 0xF;
	LARc[__boundcheck_ptr_cast_to_array_reference(28,7,"gsm_explode",(void *)(target),(void *)(target+5),5)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(28,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x3) << 2;
	LARc[__boundcheck_ptr_cast_to_array_reference(29,7,"gsm_explode",(void *)(target),(void *)(target+5),5)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(29,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[__boundcheck_ptr_cast_to_array_reference(30,7,"gsm_explode",(void *)(target),(void *)(target+6),6)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(30,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	LARc[__boundcheck_ptr_cast_to_array_reference(31,7,"gsm_explode",(void *)(target),(void *)(target+7),7)]  = *(gsm_byte *)(__boundcheck_ptr_reference(31,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7;

#define	Nc	(target + 8)

	Nc[0]  = (*(gsm_byte *)(__boundcheck_ptr_reference(35,13,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7F;

#define	bc	(target + 9)

	bc[0]  = (*(gsm_byte *)(__boundcheck_ptr_reference(39,13,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[0] |= (*(gsm_byte *)(__boundcheck_ptr_reference(40,13,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;

#define	Mc	(target + 10)

	Mc[0]  = (*(gsm_byte *)(__boundcheck_ptr_reference(44,13,"gsm_explode",(void *)(c),(void *)(c))) >> 5) & 0x3;

#define	xmaxc	(target + 11)

	xmaxc[0]  = (*(gsm_byte *)(__boundcheck_ptr_reference(48,16,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[0] |= (*(gsm_byte *)(__boundcheck_ptr_reference(49,16,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;

#define	xmc	(target + 12)

	xmc[0]  = (*(gsm_byte *)(__boundcheck_ptr_reference(53,14,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[1]  = (*(gsm_byte *)(__boundcheck_ptr_reference(54,14,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[2]  = (*(gsm_byte *)(__boundcheck_ptr_reference(55,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[2] |= (*(gsm_byte *)(__boundcheck_ptr_reference(56,14,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[3]  = (*(gsm_byte *)(__boundcheck_ptr_reference(57,14,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[4]  = *(gsm_byte *)(__boundcheck_ptr_reference(58,13,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7;
	xmc[5]  = (*(gsm_byte *)(__boundcheck_ptr_reference(59,14,"gsm_explode",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[6]  = (*(gsm_byte *)(__boundcheck_ptr_reference(60,14,"gsm_explode",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[7]  = (*(gsm_byte *)(__boundcheck_ptr_reference(61,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x3) << 1;		/* 10 */
	xmc[7] |= (*(gsm_byte *)(__boundcheck_ptr_reference(62,14,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[8]  = (*(gsm_byte *)(__boundcheck_ptr_reference(63,14,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[9]  = (*(gsm_byte *)(__boundcheck_ptr_reference(64,14,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[10]  = (*(gsm_byte *)(__boundcheck_ptr_reference(65,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[10] |= (*(gsm_byte *)(__boundcheck_ptr_reference(66,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[11]  = (*(gsm_byte *)(__boundcheck_ptr_reference(67,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[12]  = *(gsm_byte *)(__boundcheck_ptr_reference(68,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7;

#undef	Nc
#define	Nc	(target + 25 - 1)

	Nc[1]  = (*(gsm_byte *)(__boundcheck_ptr_reference(73,13,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7F;

#undef	bc
#define	bc	(target + 26 - 1)

	bc[1]  = (*(gsm_byte *)(__boundcheck_ptr_reference(78,13,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[1] |= (*(gsm_byte *)(__boundcheck_ptr_reference(79,13,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;

#undef	Mc
#define	Mc	(target + 27 - 1)

	Mc[1]  = (*(gsm_byte *)(__boundcheck_ptr_reference(84,13,"gsm_explode",(void *)(c),(void *)(c))) >> 5) & 0x3;

#undef 	xmaxc
#define	xmaxc	(target + 28 - 1)

	xmaxc[1]  = (*(gsm_byte *)(__boundcheck_ptr_reference(89,16,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[1] |= (*(gsm_byte *)(__boundcheck_ptr_reference(90,16,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;

#undef	xmc
#define	xmc	(target + 29 - 13)

	xmc[13]  = (*(gsm_byte *)(__boundcheck_ptr_reference(95,15,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[14]  = (*(gsm_byte *)(__boundcheck_ptr_reference(96,15,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[15]  = (*(gsm_byte *)(__boundcheck_ptr_reference(97,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[15] |= (*(gsm_byte *)(__boundcheck_ptr_reference(98,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[16]  = (*(gsm_byte *)(__boundcheck_ptr_reference(99,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[17]  = *(gsm_byte *)(__boundcheck_ptr_reference(100,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7;
	xmc[18]  = (*(gsm_byte *)(__boundcheck_ptr_reference(101,15,"gsm_explode",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[19]  = (*(gsm_byte *)(__boundcheck_ptr_reference(102,15,"gsm_explode",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[20]  = (*(gsm_byte *)(__boundcheck_ptr_reference(103,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[20] |= (*(gsm_byte *)(__boundcheck_ptr_reference(104,15,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[21]  = (*(gsm_byte *)(__boundcheck_ptr_reference(105,15,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[22]  = (*(gsm_byte *)(__boundcheck_ptr_reference(106,15,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[23]  = (*(gsm_byte *)(__boundcheck_ptr_reference(107,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[23] |= (*(gsm_byte *)(__boundcheck_ptr_reference(108,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[24]  = (*(gsm_byte *)(__boundcheck_ptr_reference(109,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[25]  = *(gsm_byte *)(__boundcheck_ptr_reference(110,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7;

#undef	Nc
#define	Nc	(target + 42 - 2) 

	Nc[2]  = (*(gsm_byte *)(__boundcheck_ptr_reference(115,13,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7F;

#undef	bc
#define	bc	(target + 43 - 2)

	bc[2]  = (*(gsm_byte *)(__boundcheck_ptr_reference(120,13,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 1;		/* 20 */
	bc[2] |= (*(gsm_byte *)(__boundcheck_ptr_reference(121,13,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;

#undef	Mc
#define	Mc	(target + 44 - 2)

	Mc[2]  = (*(gsm_byte *)(__boundcheck_ptr_reference(126,13,"gsm_explode",(void *)(c),(void *)(c))) >> 5) & 0x3;

#undef	xmaxc
#define	xmaxc	(target + 45 - 2)

	xmaxc[2]  = (*(gsm_byte *)(__boundcheck_ptr_reference(131,16,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[2] |= (*(gsm_byte *)(__boundcheck_ptr_reference(132,16,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;

#undef	xmc
#define	xmc	(target + 46 - 26)

	xmc[26]  = (*(gsm_byte *)(__boundcheck_ptr_reference(137,15,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[27]  = (*(gsm_byte *)(__boundcheck_ptr_reference(138,15,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[28]  = (*(gsm_byte *)(__boundcheck_ptr_reference(139,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[28] |= (*(gsm_byte *)(__boundcheck_ptr_reference(140,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[29]  = (*(gsm_byte *)(__boundcheck_ptr_reference(141,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[30]  = *(gsm_byte *)(__boundcheck_ptr_reference(142,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7;
	xmc[31]  = (*(gsm_byte *)(__boundcheck_ptr_reference(143,15,"gsm_explode",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[32]  = (*(gsm_byte *)(__boundcheck_ptr_reference(144,15,"gsm_explode",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[33]  = (*(gsm_byte *)(__boundcheck_ptr_reference(145,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[33] |= (*(gsm_byte *)(__boundcheck_ptr_reference(146,15,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[34]  = (*(gsm_byte *)(__boundcheck_ptr_reference(147,15,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[35]  = (*(gsm_byte *)(__boundcheck_ptr_reference(148,15,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[36]  = (*(gsm_byte *)(__boundcheck_ptr_reference(149,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[36] |= (*(gsm_byte *)(__boundcheck_ptr_reference(150,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[37]  = (*(gsm_byte *)(__boundcheck_ptr_reference(151,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[38]  = *(gsm_byte *)(__boundcheck_ptr_reference(152,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7;

#undef	Nc
#define	Nc	(target + 59 - 3)

	Nc[3]  = (*(gsm_byte *)(__boundcheck_ptr_reference(157,13,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7F;

#undef	bc
#define	bc	(target + 60 - 3)

	bc[3]  = (*(gsm_byte *)(__boundcheck_ptr_reference(162,13,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[3] |= (*(gsm_byte *)(__boundcheck_ptr_reference(163,13,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;

#undef	Mc
#define	Mc	(target + 61 - 3)

	Mc[3]  = (*(gsm_byte *)(__boundcheck_ptr_reference(168,13,"gsm_explode",(void *)(c),(void *)(c))) >> 5) & 0x3;

#undef	xmaxc
#define	xmaxc	(target + 62 - 3)

	xmaxc[3]  = (*(gsm_byte *)(__boundcheck_ptr_reference(173,16,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[3] |= (*(gsm_byte *)(__boundcheck_ptr_reference(174,16,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;

#undef	xmc
#define	xmc	(target + 63 - 39)

	xmc[39]  = (*(gsm_byte *)(__boundcheck_ptr_reference(179,15,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[40]  = (*(gsm_byte *)(__boundcheck_ptr_reference(180,15,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[41]  = (*(gsm_byte *)(__boundcheck_ptr_reference(181,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[41] |= (*(gsm_byte *)(__boundcheck_ptr_reference(182,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[42]  = (*(gsm_byte *)(__boundcheck_ptr_reference(183,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[43]  = *(gsm_byte *)(__boundcheck_ptr_reference(184,14,"gsm_explode",(void *)(c),(void *)(c++))) & 0x7;			/* 30  */
	xmc[44]  = (*(gsm_byte *)(__boundcheck_ptr_reference(185,15,"gsm_explode",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[45]  = (*(gsm_byte *)(__boundcheck_ptr_reference(186,15,"gsm_explode",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[46]  = (*(gsm_byte *)(__boundcheck_ptr_reference(187,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[46] |= (*(gsm_byte *)(__boundcheck_ptr_reference(188,15,"gsm_explode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[47]  = (*(gsm_byte *)(__boundcheck_ptr_reference(189,15,"gsm_explode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[48]  = (*(gsm_byte *)(__boundcheck_ptr_reference(190,15,"gsm_explode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[49]  = (*(gsm_byte *)(__boundcheck_ptr_reference(191,15,"gsm_explode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[49] |= (*(gsm_byte *)(__boundcheck_ptr_reference(192,15,"gsm_explode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[50]  = (*(gsm_byte *)(__boundcheck_ptr_reference(193,15,"gsm_explode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[51]  = *(gsm_byte *)(__boundcheck_ptr_reference(194,14,"gsm_explode",(void *)(c),(void *)(c))) & 0x7;			/* 33 */

	return 0;
}
