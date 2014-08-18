#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<string.h>
int main()
{
	static char tbuf[2][2048];

int __tbuf_0;
	for(__tbuf_0=0;__tbuf_0<2;__tbuf_0++)
__boundcheck_metadata_store(&tbuf[__tbuf_0][0],&tbuf[__tbuf_0][2048-1]);

        char buf[2][3][4];

int __buf_0;

int __buf_1;
	for(__buf_0=0;__buf_0<2;__buf_0++)
	for(__buf_1=0;__buf_1<3;__buf_1++)
__boundcheck_metadata_store(&buf[__buf_0][__buf_1][0],&buf[__buf_0][__buf_1][4-1]);

	char buf1[3][4][5];

int __buf1_0;

int __buf1_1;
	for(__buf1_0=0;__buf1_0<3;__buf1_0++)
	for(__buf1_1=0;__buf1_1<4;__buf1_1++)
__boundcheck_metadata_store(&buf1[__buf1_0][__buf1_1][0],&buf1[__buf1_0][__buf1_1][5-1]);


	buf[_RV_insert_check(0,2,9,2,"main",1)][_RV_insert_check(0,3,9,2,"main",1)][_RV_insert_check(0,4,9,2,"main",1)]='c';
	__boundcheck_strcpy(buf[_RV_insert_check(0,2,10,9,"main",1)][_RV_insert_check(0,3,10,9,"main",1)],"AAA");
	printf("%s",buf[_RV_insert_check(0,2,11,14,"main",1)][_RV_insert_check(0,3,11,14,"main",1)]);

        printf("%d\n",strlen(buf[_RV_insert_check(0,2,13,30,"main",1)][_RV_insert_check(0,3,13,30,"main",1)]));
	memset(tbuf[_RV_insert_check(0,2,14,9,"main",0)],0,2048);
	memset(tbuf[_RV_insert_check(0,2,15,9,"main",1)],0,2048);
	__boundcheck_strcpy(tbuf[_RV_insert_check(0,2,16,9,"main",0)],"AAAA");
	__boundcheck_strcpy(tbuf[_RV_insert_check(0,2,17,9,"main",1)],tbuf[_RV_insert_check(0,2,17,17,"main",0)]);
	__boundcheck_strcat(tbuf[_RV_insert_check(0,2,18,9,"main",1)],"a");
	tbuf[_RV_insert_check(0,2,19,2,"main",1)][_RV_insert_check(0,2048,19,2,"main",2047)]='\0';
	return 0;
}
