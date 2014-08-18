/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/src/gsm_decode.c,v 1.1.1.1 2000/11/06 19:54:25 mguthaus Exp $ */

#include "private.h"

#include "gsm.h"
#include "proto.h"

int gsm_decode P3((s, c, target), gsm s, gsm_byte * c, gsm_signal * target)
{
	word  	LARc[8];
__boundcheck_metadata_store((void *)(&LARc),(void *)((size_t)(&LARc)+sizeof(LARc)*8-1));
word [4] Nc[4];
__boundcheck_metadata_store((void *)(&Nc),(void *)((size_t)(&Nc)+sizeof(Nc)*8-1));
word [4] Mc[4];
__boundcheck_metadata_store((void *)(&Mc),(void *)((size_t)(&Mc)+sizeof(Mc)*8-1));
word [4] bc[4];
__boundcheck_metadata_store((void *)(&bc),(void *)((size_t)(&bc)+sizeof(bc)*8-1));
word [4] xmaxc[4];
__boundcheck_metadata_store((void *)(&xmaxc),(void *)((size_t)(&xmaxc)+sizeof(xmaxc)*8-1));
word [52] xmc[13*4];
__boundcheck_metadata_store((void *)(&xmc),(void *)((size_t)(&xmc)+sizeof(xmc)*8-1));


	/* GSM_MAGIC  = (*c >> 4) & 0xF; */

	if (((*(gsm_byte *)(__boundcheck_ptr_reference(20,9,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x0F) != GSM_MAGIC) return -1;

	LARc[_RV_insert_check(0,8,22,7,"gsm_decode",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(22,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0xF) << 2;		/* 1 */
	LARc[_RV_insert_check(0,8,23,7,"gsm_decode",0)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(23,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[_RV_insert_check(0,8,24,7,"gsm_decode",1)]  = *(gsm_byte *)(__boundcheck_ptr_reference(24,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x3F;
	LARc[_RV_insert_check(0,8,25,7,"gsm_decode",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(25,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x1F;
	LARc[_RV_insert_check(0,8,26,7,"gsm_decode",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(26,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7) << 2;
	LARc[_RV_insert_check(0,8,27,7,"gsm_decode",3)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(27,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[_RV_insert_check(0,8,28,7,"gsm_decode",4)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(28,15,"gsm_decode",(void *)(c),(void *)(c))) >> 2) & 0xF;
	LARc[_RV_insert_check(0,8,29,7,"gsm_decode",5)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(29,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x3) << 2;
	LARc[_RV_insert_check(0,8,30,7,"gsm_decode",5)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(30,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[_RV_insert_check(0,8,31,7,"gsm_decode",6)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(31,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	LARc[_RV_insert_check(0,8,32,7,"gsm_decode",7)]  = *(gsm_byte *)(__boundcheck_ptr_reference(32,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7;
	Nc[_RV_insert_check(0,4,33,5,"gsm_decode",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(33,13,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7F;
	bc[_RV_insert_check(0,4,34,5,"gsm_decode",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(34,13,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[_RV_insert_check(0,4,35,5,"gsm_decode",0)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(35,13,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	Mc[_RV_insert_check(0,4,36,5,"gsm_decode",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(36,13,"gsm_decode",(void *)(c),(void *)(c))) >> 5) & 0x3;
	xmaxc[_RV_insert_check(0,4,37,8,"gsm_decode",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(37,16,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[_RV_insert_check(0,4,38,8,"gsm_decode",0)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(38,16,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,39,6,"gsm_decode",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(39,14,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,40,6,"gsm_decode",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(40,14,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,41,6,"gsm_decode",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(41,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,42,6,"gsm_decode",2)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(42,14,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,43,6,"gsm_decode",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(43,14,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,44,6,"gsm_decode",4)]  = *(gsm_byte *)(__boundcheck_ptr_reference(44,13,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7;
	xmc[_RV_insert_check(0,52,45,6,"gsm_decode",5)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(45,14,"gsm_decode",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[_RV_insert_check(0,52,46,6,"gsm_decode",6)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(46,14,"gsm_decode",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[_RV_insert_check(0,52,47,6,"gsm_decode",7)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(47,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x3) << 1;		/* 10 */
	xmc[_RV_insert_check(0,52,48,6,"gsm_decode",7)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(48,14,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,49,6,"gsm_decode",8)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(49,14,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,50,6,"gsm_decode",9)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(50,14,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,51,6,"gsm_decode",10)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(51,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,52,6,"gsm_decode",10)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(52,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,53,6,"gsm_decode",11)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(53,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,54,6,"gsm_decode",12)]  = *(gsm_byte *)(__boundcheck_ptr_reference(54,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7;
	Nc[_RV_insert_check(0,4,55,5,"gsm_decode",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(55,13,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7F;
	bc[_RV_insert_check(0,4,56,5,"gsm_decode",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(56,13,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[_RV_insert_check(0,4,57,5,"gsm_decode",1)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(57,13,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	Mc[_RV_insert_check(0,4,58,5,"gsm_decode",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(58,13,"gsm_decode",(void *)(c),(void *)(c))) >> 5) & 0x3;
	xmaxc[_RV_insert_check(0,4,59,8,"gsm_decode",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(59,16,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[_RV_insert_check(0,4,60,8,"gsm_decode",1)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(60,16,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,61,6,"gsm_decode",13)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(61,15,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,62,6,"gsm_decode",14)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(62,15,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,63,6,"gsm_decode",15)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(63,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,64,6,"gsm_decode",15)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(64,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,65,6,"gsm_decode",16)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(65,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,66,6,"gsm_decode",17)]  = *(gsm_byte *)(__boundcheck_ptr_reference(66,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7;
	xmc[_RV_insert_check(0,52,67,6,"gsm_decode",18)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(67,15,"gsm_decode",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[_RV_insert_check(0,52,68,6,"gsm_decode",19)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(68,15,"gsm_decode",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[_RV_insert_check(0,52,69,6,"gsm_decode",20)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(69,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[_RV_insert_check(0,52,70,6,"gsm_decode",20)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(70,15,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,71,6,"gsm_decode",21)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(71,15,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,72,6,"gsm_decode",22)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(72,15,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,73,6,"gsm_decode",23)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(73,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,74,6,"gsm_decode",23)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(74,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,75,6,"gsm_decode",24)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(75,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,76,6,"gsm_decode",25)]  = *(gsm_byte *)(__boundcheck_ptr_reference(76,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7;
	Nc[_RV_insert_check(0,4,77,5,"gsm_decode",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(77,13,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7F;
	bc[_RV_insert_check(0,4,78,5,"gsm_decode",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(78,13,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 1;		/* 20 */
	bc[_RV_insert_check(0,4,79,5,"gsm_decode",2)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(79,13,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	Mc[_RV_insert_check(0,4,80,5,"gsm_decode",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(80,13,"gsm_decode",(void *)(c),(void *)(c))) >> 5) & 0x3;
	xmaxc[_RV_insert_check(0,4,81,8,"gsm_decode",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(81,16,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[_RV_insert_check(0,4,82,8,"gsm_decode",2)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(82,16,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,83,6,"gsm_decode",26)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(83,15,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,84,6,"gsm_decode",27)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(84,15,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,85,6,"gsm_decode",28)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(85,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,86,6,"gsm_decode",28)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(86,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,87,6,"gsm_decode",29)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(87,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,88,6,"gsm_decode",30)]  = *(gsm_byte *)(__boundcheck_ptr_reference(88,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7;
	xmc[_RV_insert_check(0,52,89,6,"gsm_decode",31)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(89,15,"gsm_decode",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[_RV_insert_check(0,52,90,6,"gsm_decode",32)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(90,15,"gsm_decode",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[_RV_insert_check(0,52,91,6,"gsm_decode",33)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(91,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[_RV_insert_check(0,52,92,6,"gsm_decode",33)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(92,15,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,93,6,"gsm_decode",34)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(93,15,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,94,6,"gsm_decode",35)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(94,15,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,95,6,"gsm_decode",36)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(95,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,96,6,"gsm_decode",36)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(96,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,97,6,"gsm_decode",37)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(97,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,98,6,"gsm_decode",38)]  = *(gsm_byte *)(__boundcheck_ptr_reference(98,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7;
	Nc[_RV_insert_check(0,4,99,5,"gsm_decode",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(99,13,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7F;
	bc[_RV_insert_check(0,4,100,5,"gsm_decode",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(100,13,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[_RV_insert_check(0,4,101,5,"gsm_decode",3)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(101,13,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	Mc[_RV_insert_check(0,4,102,5,"gsm_decode",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(102,13,"gsm_decode",(void *)(c),(void *)(c))) >> 5) & 0x3;
	xmaxc[_RV_insert_check(0,4,103,8,"gsm_decode",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(103,16,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[_RV_insert_check(0,4,104,8,"gsm_decode",3)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(104,16,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,105,6,"gsm_decode",39)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(105,15,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,106,6,"gsm_decode",40)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(106,15,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,107,6,"gsm_decode",41)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(107,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,108,6,"gsm_decode",41)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(108,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,109,6,"gsm_decode",42)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(109,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,110,6,"gsm_decode",43)]  = *(gsm_byte *)(__boundcheck_ptr_reference(110,14,"gsm_decode",(void *)(c),(void *)(c++))) & 0x7;			/* 30  */
	xmc[_RV_insert_check(0,52,111,6,"gsm_decode",44)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(111,15,"gsm_decode",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[_RV_insert_check(0,52,112,6,"gsm_decode",45)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(112,15,"gsm_decode",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[_RV_insert_check(0,52,113,6,"gsm_decode",46)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(113,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[_RV_insert_check(0,52,114,6,"gsm_decode",46)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(114,15,"gsm_decode",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,115,6,"gsm_decode",47)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(115,15,"gsm_decode",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,116,6,"gsm_decode",48)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(116,15,"gsm_decode",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,117,6,"gsm_decode",49)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(117,15,"gsm_decode",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,118,6,"gsm_decode",49)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(118,15,"gsm_decode",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,119,6,"gsm_decode",50)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(119,15,"gsm_decode",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,120,6,"gsm_decode",51)]  = *(gsm_byte *)(__boundcheck_ptr_reference(120,14,"gsm_decode",(void *)(c),(void *)(c))) & 0x7;			/* 33 */

	Gsm_Decoder(s, LARc, Nc, bc, Mc, xmaxc, xmc, target);

	return 0;
}
