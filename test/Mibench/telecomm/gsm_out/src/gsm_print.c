/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/src/gsm_print.c,v 1.1.1.1 2000/11/06 19:54:26 mguthaus Exp $ */

#include	<stdio.h>

#include "private.h"

#include "gsm.h"
#include "proto.h"

int gsm_print P3((f, s, c), FILE * f, gsm s, gsm_byte * c)
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

	if (((*(gsm_byte *)(__boundcheck_ptr_reference(22,9,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x0F) != GSM_MAGIC) return -1;

	LARc[_RV_insert_check(0,8,24,7,"gsm_print",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(24,15,"gsm_print",(void *)(c),(void *)(c++))) & 0xF) << 2;		/* 1 */
	LARc[_RV_insert_check(0,8,25,7,"gsm_print",0)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(25,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[_RV_insert_check(0,8,26,7,"gsm_print",1)]  = *(gsm_byte *)(__boundcheck_ptr_reference(26,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x3F;
	LARc[_RV_insert_check(0,8,27,7,"gsm_print",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(27,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x1F;
	LARc[_RV_insert_check(0,8,28,7,"gsm_print",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(28,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x7) << 2;
	LARc[_RV_insert_check(0,8,29,7,"gsm_print",3)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(29,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[_RV_insert_check(0,8,30,7,"gsm_print",4)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(30,15,"gsm_print",(void *)(c),(void *)(c))) >> 2) & 0xF;
	LARc[_RV_insert_check(0,8,31,7,"gsm_print",5)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(31,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x3) << 2;
	LARc[_RV_insert_check(0,8,32,7,"gsm_print",5)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(32,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	LARc[_RV_insert_check(0,8,33,7,"gsm_print",6)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(33,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	LARc[_RV_insert_check(0,8,34,7,"gsm_print",7)]  = *(gsm_byte *)(__boundcheck_ptr_reference(34,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x7;


	Nc[_RV_insert_check(0,4,37,5,"gsm_print",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(37,13,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7F;
	bc[_RV_insert_check(0,4,38,5,"gsm_print",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(38,13,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[_RV_insert_check(0,4,39,5,"gsm_print",0)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(39,13,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	Mc[_RV_insert_check(0,4,40,5,"gsm_print",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(40,13,"gsm_print",(void *)(c),(void *)(c))) >> 5) & 0x3;
	xmaxc[_RV_insert_check(0,4,41,8,"gsm_print",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(41,16,"gsm_print",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[_RV_insert_check(0,4,42,8,"gsm_print",0)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(42,16,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,43,6,"gsm_print",0)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(43,14,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,44,6,"gsm_print",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(44,14,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,45,6,"gsm_print",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(45,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,46,6,"gsm_print",2)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(46,14,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,47,6,"gsm_print",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(47,14,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,48,6,"gsm_print",4)]  = *(gsm_byte *)(__boundcheck_ptr_reference(48,13,"gsm_print",(void *)(c),(void *)(c++))) & 0x7;
	xmc[_RV_insert_check(0,52,49,6,"gsm_print",5)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(49,14,"gsm_print",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[_RV_insert_check(0,52,50,6,"gsm_print",6)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(50,14,"gsm_print",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[_RV_insert_check(0,52,51,6,"gsm_print",7)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(51,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x3) << 1;		/* 10 */
	xmc[_RV_insert_check(0,52,52,6,"gsm_print",7)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(52,14,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,53,6,"gsm_print",8)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(53,14,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,54,6,"gsm_print",9)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(54,14,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,55,6,"gsm_print",10)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(55,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,56,6,"gsm_print",10)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(56,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,57,6,"gsm_print",11)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(57,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,58,6,"gsm_print",12)]  = *(gsm_byte *)(__boundcheck_ptr_reference(58,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x7;

	Nc[_RV_insert_check(0,4,60,5,"gsm_print",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(60,13,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7F;
	bc[_RV_insert_check(0,4,61,5,"gsm_print",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(61,13,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[_RV_insert_check(0,4,62,5,"gsm_print",1)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(62,13,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	Mc[_RV_insert_check(0,4,63,5,"gsm_print",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(63,13,"gsm_print",(void *)(c),(void *)(c))) >> 5) & 0x3;
	xmaxc[_RV_insert_check(0,4,64,8,"gsm_print",1)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(64,16,"gsm_print",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[_RV_insert_check(0,4,65,8,"gsm_print",1)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(65,16,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,66,6,"gsm_print",13)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(66,15,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,67,6,"gsm_print",14)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(67,15,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,68,6,"gsm_print",15)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(68,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,69,6,"gsm_print",15)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(69,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,70,6,"gsm_print",16)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(70,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,71,6,"gsm_print",17)]  = *(gsm_byte *)(__boundcheck_ptr_reference(71,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x7;
	xmc[_RV_insert_check(0,52,72,6,"gsm_print",18)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(72,15,"gsm_print",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[_RV_insert_check(0,52,73,6,"gsm_print",19)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(73,15,"gsm_print",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[_RV_insert_check(0,52,74,6,"gsm_print",20)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(74,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[_RV_insert_check(0,52,75,6,"gsm_print",20)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(75,15,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,76,6,"gsm_print",21)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(76,15,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,77,6,"gsm_print",22)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(77,15,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,78,6,"gsm_print",23)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(78,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,79,6,"gsm_print",23)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(79,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,80,6,"gsm_print",24)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(80,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,81,6,"gsm_print",25)]  = *(gsm_byte *)(__boundcheck_ptr_reference(81,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x7;


	Nc[_RV_insert_check(0,4,84,5,"gsm_print",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(84,13,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7F;
	bc[_RV_insert_check(0,4,85,5,"gsm_print",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(85,13,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 1;		/* 20 */
	bc[_RV_insert_check(0,4,86,5,"gsm_print",2)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(86,13,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	Mc[_RV_insert_check(0,4,87,5,"gsm_print",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(87,13,"gsm_print",(void *)(c),(void *)(c))) >> 5) & 0x3;
	xmaxc[_RV_insert_check(0,4,88,8,"gsm_print",2)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(88,16,"gsm_print",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[_RV_insert_check(0,4,89,8,"gsm_print",2)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(89,16,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,90,6,"gsm_print",26)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(90,15,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,91,6,"gsm_print",27)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(91,15,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,92,6,"gsm_print",28)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(92,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,93,6,"gsm_print",28)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(93,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,94,6,"gsm_print",29)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(94,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,95,6,"gsm_print",30)]  = *(gsm_byte *)(__boundcheck_ptr_reference(95,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x7;
	xmc[_RV_insert_check(0,52,96,6,"gsm_print",31)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(96,15,"gsm_print",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[_RV_insert_check(0,52,97,6,"gsm_print",32)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(97,15,"gsm_print",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[_RV_insert_check(0,52,98,6,"gsm_print",33)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(98,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[_RV_insert_check(0,52,99,6,"gsm_print",33)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(99,15,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,100,6,"gsm_print",34)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(100,15,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,101,6,"gsm_print",35)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(101,15,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,102,6,"gsm_print",36)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(102,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,103,6,"gsm_print",36)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(103,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,104,6,"gsm_print",37)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(104,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,105,6,"gsm_print",38)]  = *(gsm_byte *)(__boundcheck_ptr_reference(105,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x7;

	Nc[_RV_insert_check(0,4,107,5,"gsm_print",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(107,13,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7F;
	bc[_RV_insert_check(0,4,108,5,"gsm_print",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(108,13,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 1;
	bc[_RV_insert_check(0,4,109,5,"gsm_print",3)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(109,13,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	Mc[_RV_insert_check(0,4,110,5,"gsm_print",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(110,13,"gsm_print",(void *)(c),(void *)(c))) >> 5) & 0x3;
	xmaxc[_RV_insert_check(0,4,111,8,"gsm_print",3)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(111,16,"gsm_print",(void *)(c),(void *)(c++))) & 0x1F) << 1;
	xmaxc[_RV_insert_check(0,4,112,8,"gsm_print",3)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(112,16,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;

	xmc[_RV_insert_check(0,52,114,6,"gsm_print",39)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(114,15,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,115,6,"gsm_print",40)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(115,15,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,116,6,"gsm_print",41)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(116,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,117,6,"gsm_print",41)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(117,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,118,6,"gsm_print",42)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(118,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,119,6,"gsm_print",43)]  = *(gsm_byte *)(__boundcheck_ptr_reference(119,14,"gsm_print",(void *)(c),(void *)(c++))) & 0x7;			/* 30  */
	xmc[_RV_insert_check(0,52,120,6,"gsm_print",44)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(120,15,"gsm_print",(void *)(c),(void *)(c))) >> 5) & 0x7;
	xmc[_RV_insert_check(0,52,121,6,"gsm_print",45)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(121,15,"gsm_print",(void *)(c),(void *)(c))) >> 2) & 0x7;
	xmc[_RV_insert_check(0,52,122,6,"gsm_print",46)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(122,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x3) << 1;
	xmc[_RV_insert_check(0,52,123,6,"gsm_print",46)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(123,15,"gsm_print",(void *)(c),(void *)(c))) >> 7) & 0x1;
	xmc[_RV_insert_check(0,52,124,6,"gsm_print",47)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(124,15,"gsm_print",(void *)(c),(void *)(c))) >> 4) & 0x7;
	xmc[_RV_insert_check(0,52,125,6,"gsm_print",48)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(125,15,"gsm_print",(void *)(c),(void *)(c))) >> 1) & 0x7;
	xmc[_RV_insert_check(0,52,126,6,"gsm_print",49)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(126,15,"gsm_print",(void *)(c),(void *)(c++))) & 0x1) << 2;
	xmc[_RV_insert_check(0,52,127,6,"gsm_print",49)] |= (*(gsm_byte *)(__boundcheck_ptr_reference(127,15,"gsm_print",(void *)(c),(void *)(c))) >> 6) & 0x3;
	xmc[_RV_insert_check(0,52,128,6,"gsm_print",50)]  = (*(gsm_byte *)(__boundcheck_ptr_reference(128,15,"gsm_print",(void *)(c),(void *)(c))) >> 3) & 0x7;
	xmc[_RV_insert_check(0,52,129,6,"gsm_print",51)]  = *(gsm_byte *)(__boundcheck_ptr_reference(129,14,"gsm_print",(void *)(c),(void *)(c))) & 0x7;			/* 33 */

	fprintf(f,
	      "LARc:\t%2.2d  %2.2d  %2.2d  %2.2d  %2.2d  %2.2d  %2.2d  %2.2d\n",
	       LARc[_RV_insert_check(0,8,133,14,"gsm_print",0)],LARc[_RV_insert_check(0,8,133,22,"gsm_print",1)],LARc[_RV_insert_check(0,8,133,30,"gsm_print",2)],LARc[_RV_insert_check(0,8,133,38,"gsm_print",3)],LARc[_RV_insert_check(0,8,133,46,"gsm_print",4)],LARc[_RV_insert_check(0,8,133,54,"gsm_print",5)],LARc[_RV_insert_check(0,8,133,62,"gsm_print",6)],LARc[_RV_insert_check(0,8,133,70,"gsm_print",7)]);

	fprintf(f, "#1: 	Nc %4.4d    bc %d    Mc %d    xmaxc %d\n",
		Nc[_RV_insert_check(0,4,136,6,"gsm_print",0)], bc[_RV_insert_check(0,4,136,13,"gsm_print",0)], Mc[_RV_insert_check(0,4,136,20,"gsm_print",0)], xmaxc[_RV_insert_check(0,4,136,30,"gsm_print",0)]);
	fprintf(f,
"\t%.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d\n",
		xmc[_RV_insert_check(0,52,139,7,"gsm_print",0)],xmc[_RV_insert_check(0,52,139,14,"gsm_print",1)],xmc[_RV_insert_check(0,52,139,21,"gsm_print",2)],xmc[_RV_insert_check(0,52,139,28,"gsm_print",3)],xmc[_RV_insert_check(0,52,139,35,"gsm_print",4)],xmc[_RV_insert_check(0,52,139,42,"gsm_print",5)],xmc[_RV_insert_check(0,52,139,49,"gsm_print",6)],
		xmc[_RV_insert_check(0,52,140,7,"gsm_print",7)],xmc[_RV_insert_check(0,52,140,14,"gsm_print",8)],xmc[_RV_insert_check(0,52,140,21,"gsm_print",9)],xmc[_RV_insert_check(0,52,140,28,"gsm_print",10)],xmc[_RV_insert_check(0,52,140,36,"gsm_print",11)],xmc[_RV_insert_check(0,52,140,44,"gsm_print",12)] );

	fprintf(f, "#2: 	Nc %4.4d    bc %d    Mc %d    xmaxc %d\n",
		Nc[_RV_insert_check(0,4,143,6,"gsm_print",1)], bc[_RV_insert_check(0,4,143,13,"gsm_print",1)], Mc[_RV_insert_check(0,4,143,20,"gsm_print",1)], xmaxc[_RV_insert_check(0,4,143,30,"gsm_print",1)]);
	fprintf(f,
"\t%.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d\n",
		xmc[_RV_insert_check(0,52,146,7,"gsm_print",13+0)],xmc[_RV_insert_check(0,52,146,17,"gsm_print",13+1)],xmc[_RV_insert_check(0,52,146,27,"gsm_print",13+2)],xmc[_RV_insert_check(0,52,146,37,"gsm_print",13+3)],xmc[_RV_insert_check(0,52,146,47,"gsm_print",13+4)],xmc[_RV_insert_check(0,52,146,57,"gsm_print",13+5)],
		xmc[_RV_insert_check(0,52,147,7,"gsm_print",13+6)], xmc[_RV_insert_check(0,52,147,18,"gsm_print",13+7)],xmc[_RV_insert_check(0,52,147,28,"gsm_print",13+8)],xmc[_RV_insert_check(0,52,147,38,"gsm_print",13+9)],xmc[_RV_insert_check(0,52,147,48,"gsm_print",13+10)],xmc[_RV_insert_check(0,52,147,59,"gsm_print",13+11)],
		xmc[_RV_insert_check(0,52,148,7,"gsm_print",13+12)] );

	fprintf(f, "#3: 	Nc %4.4d    bc %d    Mc %d    xmaxc %d\n",
		Nc[_RV_insert_check(0,4,151,6,"gsm_print",2)], bc[_RV_insert_check(0,4,151,13,"gsm_print",2)], Mc[_RV_insert_check(0,4,151,20,"gsm_print",2)], xmaxc[_RV_insert_check(0,4,151,30,"gsm_print",2)]);
	fprintf(f,
"\t%.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d\n",
		xmc[_RV_insert_check(0,52,154,7,"gsm_print",26+0)],xmc[_RV_insert_check(0,52,154,17,"gsm_print",26+1)],xmc[_RV_insert_check(0,52,154,27,"gsm_print",26+2)],xmc[_RV_insert_check(0,52,154,37,"gsm_print",26+3)],xmc[_RV_insert_check(0,52,154,47,"gsm_print",26+4)],xmc[_RV_insert_check(0,52,154,57,"gsm_print",26+5)],
		xmc[_RV_insert_check(0,52,155,7,"gsm_print",26+6)], xmc[_RV_insert_check(0,52,155,18,"gsm_print",26+7)],xmc[_RV_insert_check(0,52,155,28,"gsm_print",26+8)],xmc[_RV_insert_check(0,52,155,38,"gsm_print",26+9)],xmc[_RV_insert_check(0,52,155,48,"gsm_print",26+10)],xmc[_RV_insert_check(0,52,155,59,"gsm_print",26+11)],
		xmc[_RV_insert_check(0,52,156,7,"gsm_print",26+12)] );

	fprintf(f, "#4: 	Nc %4.4d    bc %d    Mc %d    xmaxc %d\n",
		Nc[_RV_insert_check(0,4,159,6,"gsm_print",3)], bc[_RV_insert_check(0,4,159,13,"gsm_print",3)], Mc[_RV_insert_check(0,4,159,20,"gsm_print",3)], xmaxc[_RV_insert_check(0,4,159,30,"gsm_print",3)]);
	fprintf(f,
"\t%.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d %.2d\n",
		xmc[_RV_insert_check(0,52,162,7,"gsm_print",39+0)],xmc[_RV_insert_check(0,52,162,17,"gsm_print",39+1)],xmc[_RV_insert_check(0,52,162,27,"gsm_print",39+2)],xmc[_RV_insert_check(0,52,162,37,"gsm_print",39+3)],xmc[_RV_insert_check(0,52,162,47,"gsm_print",39+4)],xmc[_RV_insert_check(0,52,162,57,"gsm_print",39+5)],
		xmc[_RV_insert_check(0,52,163,7,"gsm_print",39+6)], xmc[_RV_insert_check(0,52,163,18,"gsm_print",39+7)],xmc[_RV_insert_check(0,52,163,28,"gsm_print",39+8)],xmc[_RV_insert_check(0,52,163,38,"gsm_print",39+9)],xmc[_RV_insert_check(0,52,163,48,"gsm_print",39+10)],xmc[_RV_insert_check(0,52,163,59,"gsm_print",39+11)],
		xmc[_RV_insert_check(0,52,164,7,"gsm_print",39+12)] );

	return 0;
}
