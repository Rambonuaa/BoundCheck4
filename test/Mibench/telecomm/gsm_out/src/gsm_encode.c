/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/src/gsm_encode.c,v 1.1.1.1 2000/11/06 19:54:26 mguthaus Exp $ */

#include "private.h"
#include "gsm.h"
#include "proto.h"

void gsm_encode P3((s, source, c), gsm s, gsm_signal * source, gsm_byte * c)
{
	word	 word [8] LARc[8];
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


	Gsm_Coder(s, source, LARc, Nc, bc, Mc, xmaxc, xmc);


	/*	variable	size

		GSM_MAGIC	4

		LARc[0]		6
		LARc[1]		6
		LARc[2]		5
		LARc[3]		5
		LARc[4]		4
		LARc[5]		4
		LARc[6]		3
		LARc[7]		3

		Nc[0]		7
		bc[0]		2
		Mc[0]		2
		xmaxc[0]	6
		xmc[0]		3
		xmc[1]		3
		xmc[2]		3
		xmc[3]		3
		xmc[4]		3
		xmc[5]		3
		xmc[6]		3
		xmc[7]		3
		xmc[8]		3
		xmc[9]		3
		xmc[10]		3
		xmc[11]		3
		xmc[12]		3

		Nc[1]		7
		bc[1]		2
		Mc[1]		2
		xmaxc[1]	6
		xmc[13]		3
		xmc[14]		3
		xmc[15]		3
		xmc[16]		3
		xmc[17]		3
		xmc[18]		3
		xmc[19]		3
		xmc[20]		3
		xmc[21]		3
		xmc[22]		3
		xmc[23]		3
		xmc[24]		3
		xmc[25]		3

		Nc[2]		7
		bc[2]		2
		Mc[2]		2
		xmaxc[2]	6
		xmc[26]		3
		xmc[27]		3
		xmc[28]		3
		xmc[29]		3
		xmc[30]		3
		xmc[31]		3
		xmc[32]		3
		xmc[33]		3
		xmc[34]		3
		xmc[35]		3
		xmc[36]		3
		xmc[37]		3
		xmc[38]		3

		Nc[3]		7
		bc[3]		2
		Mc[3]		2
		xmaxc[3]	6
		xmc[39]		3
		xmc[40]		3
		xmc[41]		3
		xmc[42]		3
		xmc[43]		3
		xmc[44]		3
		xmc[45]		3
		xmc[46]		3
		xmc[47]		3
		xmc[48]		3
		xmc[49]		3
		xmc[50]		3
		xmc[51]		3
	*/


	*(gsm_byte *)(__boundcheck_ptr_reference(107,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((GSM_MAGIC & 0xF) << 4)		/* 1 */
	       | ((LARc[_RV_insert_check(0,8,108,18,"gsm_encode",0)] >> 2) & 0xF);
	*(gsm_byte *)(__boundcheck_ptr_reference(109,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((LARc[_RV_insert_check(0,8,109,18,"gsm_encode",0)] & 0x3) << 6)
	       | (LARc[_RV_insert_check(0,8,110,17,"gsm_encode",1)] & 0x3F);
	*(gsm_byte *)(__boundcheck_ptr_reference(111,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((LARc[_RV_insert_check(0,8,111,18,"gsm_encode",2)] & 0x1F) << 3)
	       | ((LARc[_RV_insert_check(0,8,112,18,"gsm_encode",3)] >> 2) & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(113,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((LARc[_RV_insert_check(0,8,113,18,"gsm_encode",3)] & 0x3) << 6)
	       | ((LARc[_RV_insert_check(0,8,114,18,"gsm_encode",4)] & 0xF) << 2)
	       | ((LARc[_RV_insert_check(0,8,115,18,"gsm_encode",5)] >> 2) & 0x3);
	*(gsm_byte *)(__boundcheck_ptr_reference(116,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((LARc[_RV_insert_check(0,8,116,18,"gsm_encode",5)] & 0x3) << 6)
	       | ((LARc[_RV_insert_check(0,8,117,18,"gsm_encode",6)] & 0x7) << 3)
	       | (LARc[_RV_insert_check(0,8,118,17,"gsm_encode",7)] & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(119,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((Nc[_RV_insert_check(0,4,119,16,"gsm_encode",0)] & 0x7F) << 1)
	       | ((bc[_RV_insert_check(0,4,120,16,"gsm_encode",0)] >> 1) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(121,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((bc[_RV_insert_check(0,4,121,16,"gsm_encode",0)] & 0x1) << 7)
	       | ((Mc[_RV_insert_check(0,4,122,16,"gsm_encode",0)] & 0x3) << 5)
	       | ((xmaxc[_RV_insert_check(0,4,123,19,"gsm_encode",0)] >> 1) & 0x1F);
	*(gsm_byte *)(__boundcheck_ptr_reference(124,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmaxc[_RV_insert_check(0,4,124,19,"gsm_encode",0)] & 0x1) << 7)
	       | ((xmc[_RV_insert_check(0,52,125,17,"gsm_encode",0)] & 0x7) << 4)
	       | ((xmc[_RV_insert_check(0,52,126,17,"gsm_encode",1)] & 0x7) << 1)
	       | ((xmc[_RV_insert_check(0,52,127,17,"gsm_encode",2)] >> 2) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(128,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,128,17,"gsm_encode",2)] & 0x3) << 6)
	       | ((xmc[_RV_insert_check(0,52,129,17,"gsm_encode",3)] & 0x7) << 3)
	       | (xmc[_RV_insert_check(0,52,130,16,"gsm_encode",4)] & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(131,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,131,17,"gsm_encode",5)] & 0x7) << 5)			/* 10 */
	       | ((xmc[_RV_insert_check(0,52,132,17,"gsm_encode",6)] & 0x7) << 2)
	       | ((xmc[_RV_insert_check(0,52,133,17,"gsm_encode",7)] >> 1) & 0x3);
	*(gsm_byte *)(__boundcheck_ptr_reference(134,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,134,17,"gsm_encode",7)] & 0x1) << 7)
	       | ((xmc[_RV_insert_check(0,52,135,17,"gsm_encode",8)] & 0x7) << 4)
	       | ((xmc[_RV_insert_check(0,52,136,17,"gsm_encode",9)] & 0x7) << 1)
	       | ((xmc[_RV_insert_check(0,52,137,17,"gsm_encode",10)] >> 2) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(138,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,138,17,"gsm_encode",10)] & 0x3) << 6)
	       | ((xmc[_RV_insert_check(0,52,139,17,"gsm_encode",11)] & 0x7) << 3)
	       | (xmc[_RV_insert_check(0,52,140,16,"gsm_encode",12)] & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(141,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((Nc[_RV_insert_check(0,4,141,16,"gsm_encode",1)] & 0x7F) << 1)
	       | ((bc[_RV_insert_check(0,4,142,16,"gsm_encode",1)] >> 1) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(143,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((bc[_RV_insert_check(0,4,143,16,"gsm_encode",1)] & 0x1) << 7)
	       | ((Mc[_RV_insert_check(0,4,144,16,"gsm_encode",1)] & 0x3) << 5)
	       | ((xmaxc[_RV_insert_check(0,4,145,19,"gsm_encode",1)] >> 1) & 0x1F);
	*(gsm_byte *)(__boundcheck_ptr_reference(146,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmaxc[_RV_insert_check(0,4,146,19,"gsm_encode",1)] & 0x1) << 7)
	       | ((xmc[_RV_insert_check(0,52,147,17,"gsm_encode",13)] & 0x7) << 4)
	       | ((xmc[_RV_insert_check(0,52,148,17,"gsm_encode",14)] & 0x7) << 1)
	       | ((xmc[_RV_insert_check(0,52,149,17,"gsm_encode",15)] >> 2) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(150,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,150,17,"gsm_encode",15)] & 0x3) << 6)
	       | ((xmc[_RV_insert_check(0,52,151,17,"gsm_encode",16)] & 0x7) << 3)
	       | (xmc[_RV_insert_check(0,52,152,16,"gsm_encode",17)] & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(153,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,153,17,"gsm_encode",18)] & 0x7) << 5)
	       | ((xmc[_RV_insert_check(0,52,154,17,"gsm_encode",19)] & 0x7) << 2)
	       | ((xmc[_RV_insert_check(0,52,155,17,"gsm_encode",20)] >> 1) & 0x3);
	*(gsm_byte *)(__boundcheck_ptr_reference(156,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,156,17,"gsm_encode",20)] & 0x1) << 7)
	       | ((xmc[_RV_insert_check(0,52,157,17,"gsm_encode",21)] & 0x7) << 4)
	       | ((xmc[_RV_insert_check(0,52,158,17,"gsm_encode",22)] & 0x7) << 1)
	       | ((xmc[_RV_insert_check(0,52,159,17,"gsm_encode",23)] >> 2) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(160,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,160,17,"gsm_encode",23)] & 0x3) << 6)
	       | ((xmc[_RV_insert_check(0,52,161,17,"gsm_encode",24)] & 0x7) << 3)
	       | (xmc[_RV_insert_check(0,52,162,16,"gsm_encode",25)] & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(163,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((Nc[_RV_insert_check(0,4,163,16,"gsm_encode",2)] & 0x7F) << 1)			/* 20 */
	       | ((bc[_RV_insert_check(0,4,164,16,"gsm_encode",2)] >> 1) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(165,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((bc[_RV_insert_check(0,4,165,16,"gsm_encode",2)] & 0x1) << 7)
	       | ((Mc[_RV_insert_check(0,4,166,16,"gsm_encode",2)] & 0x3) << 5)
	       | ((xmaxc[_RV_insert_check(0,4,167,19,"gsm_encode",2)] >> 1) & 0x1F);
	*(gsm_byte *)(__boundcheck_ptr_reference(168,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmaxc[_RV_insert_check(0,4,168,19,"gsm_encode",2)] & 0x1) << 7)
	       | ((xmc[_RV_insert_check(0,52,169,17,"gsm_encode",26)] & 0x7) << 4)
	       | ((xmc[_RV_insert_check(0,52,170,17,"gsm_encode",27)] & 0x7) << 1)
	       | ((xmc[_RV_insert_check(0,52,171,17,"gsm_encode",28)] >> 2) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(172,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,172,17,"gsm_encode",28)] & 0x3) << 6)
	       | ((xmc[_RV_insert_check(0,52,173,17,"gsm_encode",29)] & 0x7) << 3)
	       | (xmc[_RV_insert_check(0,52,174,16,"gsm_encode",30)] & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(175,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,175,17,"gsm_encode",31)] & 0x7) << 5)
	       | ((xmc[_RV_insert_check(0,52,176,17,"gsm_encode",32)] & 0x7) << 2)
	       | ((xmc[_RV_insert_check(0,52,177,17,"gsm_encode",33)] >> 1) & 0x3);
	*(gsm_byte *)(__boundcheck_ptr_reference(178,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,178,17,"gsm_encode",33)] & 0x1) << 7)
	       | ((xmc[_RV_insert_check(0,52,179,17,"gsm_encode",34)] & 0x7) << 4)
	       | ((xmc[_RV_insert_check(0,52,180,17,"gsm_encode",35)] & 0x7) << 1)
	       | ((xmc[_RV_insert_check(0,52,181,17,"gsm_encode",36)] >> 2) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(182,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,182,17,"gsm_encode",36)] & 0x3) << 6)
	       | ((xmc[_RV_insert_check(0,52,183,17,"gsm_encode",37)] & 0x7) << 3)
	       | (xmc[_RV_insert_check(0,52,184,16,"gsm_encode",38)] & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(185,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((Nc[_RV_insert_check(0,4,185,16,"gsm_encode",3)] & 0x7F) << 1)
	       | ((bc[_RV_insert_check(0,4,186,16,"gsm_encode",3)] >> 1) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(187,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((bc[_RV_insert_check(0,4,187,16,"gsm_encode",3)] & 0x1) << 7)
	       | ((Mc[_RV_insert_check(0,4,188,16,"gsm_encode",3)] & 0x3) << 5)
	       | ((xmaxc[_RV_insert_check(0,4,189,19,"gsm_encode",3)] >> 1) & 0x1F);
	*(gsm_byte *)(__boundcheck_ptr_reference(190,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmaxc[_RV_insert_check(0,4,190,19,"gsm_encode",3)] & 0x1) << 7)
	       | ((xmc[_RV_insert_check(0,52,191,17,"gsm_encode",39)] & 0x7) << 4)
	       | ((xmc[_RV_insert_check(0,52,192,17,"gsm_encode",40)] & 0x7) << 1)
	       | ((xmc[_RV_insert_check(0,52,193,17,"gsm_encode",41)] >> 2) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(194,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,194,17,"gsm_encode",41)] & 0x3) << 6)			/* 30 */
	       | ((xmc[_RV_insert_check(0,52,195,17,"gsm_encode",42)] & 0x7) << 3)
	       | (xmc[_RV_insert_check(0,52,196,16,"gsm_encode",43)] & 0x7);
	*(gsm_byte *)(__boundcheck_ptr_reference(197,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,197,17,"gsm_encode",44)] & 0x7) << 5)
	       | ((xmc[_RV_insert_check(0,52,198,17,"gsm_encode",45)] & 0x7) << 2)
	       | ((xmc[_RV_insert_check(0,52,199,17,"gsm_encode",46)] >> 1) & 0x3);
	*(gsm_byte *)(__boundcheck_ptr_reference(200,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,200,17,"gsm_encode",46)] & 0x1) << 7)
	       | ((xmc[_RV_insert_check(0,52,201,17,"gsm_encode",47)] & 0x7) << 4)
	       | ((xmc[_RV_insert_check(0,52,202,17,"gsm_encode",48)] & 0x7) << 1)
	       | ((xmc[_RV_insert_check(0,52,203,17,"gsm_encode",49)] >> 2) & 0x1);
	*(gsm_byte *)(__boundcheck_ptr_reference(204,3,"gsm_encode",(void *)(c),(void *)(c++))) =   ((xmc[_RV_insert_check(0,52,204,17,"gsm_encode",49)] & 0x3) << 6)
	       | ((xmc[_RV_insert_check(0,52,205,17,"gsm_encode",50)] & 0x7) << 3)
	       | (xmc[_RV_insert_check(0,52,206,16,"gsm_encode",51)] & 0x7);
}
