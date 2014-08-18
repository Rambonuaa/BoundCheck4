#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<string.h>
#define N 4

char *__string1="define";
char *__string2="else";
struct struct2 {
  int len;
  char *string;
  char string1[10];
  int interp;
} ss[] = {
	{ 6, __string1,"asdfs",	 	1 },
	{ 4, __string2,"sdfsf",	 	0 },
	{ 5, "endif","sdfweq",	 	0 },
	{ 5, "ifdef","werew",	 	0 },
	{ 6, "ifndef","ewrewq",   	0 },
	{ 2, "if","qwewq",		0 },
	{ 7, "include","rtyrtu",  	1 },
	{ 8, "undefine","tyuytu", 	1 },
};

int main()
{
   struct struct2 *p=ss;
__boundcheck_metadata_store(&(p),&(p));

   char *q=&p[__boundcheck_ptr_cast_to_array_reference(23,15,"main",(void *)(p),(void *)(p+2),2)].string[0];
__boundcheck_metadata_store((void *)(&p[2].string[0]),(void *)(p[2].string[0]+char-1));

   char **l=&q;
__boundcheck_copy_metadata((void *)(l),(void *)(q));

   printf("%s\n",*(char **)(__boundcheck_ptr_reference(25,19,"main",(void *)(l),(void *)(l))));
   printf("%c\n",*(char **)(__boundcheck_ptr_reference(26,19,"main",(void *)(l),(void *)(*(char **)(__boundcheck_ptr_reference(26,20,"main",(void *)(l),(void *)(l)))))));
   return 1;
}
