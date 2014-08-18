#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<string.h>
#define N 4
struct model {
  int len;
  char *string;
  char string1[10];
  int interp;
}; 

int main()
{
   char *__tmp_string_7="undefine";
 __boundcheck_metadata_store((void *)(__tmp_string_7),(void *)(__tmp_string_7+8));
char *__tmp_string_6="include";
 __boundcheck_metadata_store((void *)(__tmp_string_6),(void *)(__tmp_string_6+7));
char *__tmp_string_5="if";
 __boundcheck_metadata_store((void *)(__tmp_string_5),(void *)(__tmp_string_5+2));
char *__tmp_string_4="ifndef";
 __boundcheck_metadata_store((void *)(__tmp_string_4),(void *)(__tmp_string_4+6));
char *__tmp_string_3="ifdef";
 __boundcheck_metadata_store((void *)(__tmp_string_3),(void *)(__tmp_string_3+5));
char *__tmp_string_2="endif";
 __boundcheck_metadata_store((void *)(__tmp_string_2),(void *)(__tmp_string_2+5));
char *__tmp_string_1="else";
 __boundcheck_metadata_store((void *)(__tmp_string_1),(void *)(__tmp_string_1+4));
char *__tmp_string_0="define";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+6));
struct model ss[]= {
{6,__tmp_string_0,"asdfs",	 	1 },
{4,__tmp_string_1,"sdfsf",	 	0 },
{5,__tmp_string_2,"sdfweq",	 	0 },
{5,__tmp_string_3,"werew",	 	0 },
{6,__tmp_string_4,"ewrewq",   	0 },
{2,__tmp_string_5,"qwewq",		0 },
{7,__tmp_string_6,"rtyrtu",  	1 },
{8,__tmp_string_7,"tyuytu", 	1 }
   };__boundcheck_metadata_store(&ss[0],&ss[8-1]);

   char *__tmp_string_10="sdfwee";
 __boundcheck_metadata_store((void *)(__tmp_string_10),(void *)(__tmp_string_10+6));
char *__tmp_string_9="sdfwee";
 __boundcheck_metadata_store((void *)(__tmp_string_9),(void *)(__tmp_string_9+6));
char *__tmp_string_8="asdfds";
 __boundcheck_metadata_store((void *)(__tmp_string_8),(void *)(__tmp_string_8+6));
char *ssl[]={__tmp_string_8,__tmp_string_9,__tmp_string_10};__boundcheck_metadata_store(&ssl[0],&ssl[3-1]);

   char *__tmp_string_11="qweqwe";
 __boundcheck_metadata_store((void *)(__tmp_string_11),(void *)(__tmp_string_11+6));
char *str=__tmp_string_11;
__boundcheck_metadata_store((void *)(&str),(void *)((size_t)(&str)+sizeof(str)*8-1));

   struct model *p=ss;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));

   char *q=&(*(struct model *)(__boundcheck_ptr_reference(26,13,"main",(void *)(&p[0]),(void *)(&p[2])))).string[0];
__boundcheck_metadata_store((void *)(&q),(void *)((size_t)(&q)+sizeof(q)*8-1));

   char **l=&q;
__boundcheck_metadata_store((void *)(&l),(void *)((size_t)(&l)+sizeof(l)*8-1));

   printf("%s\n",*(char **)(__boundcheck_ptr_reference(28,19,"main",(void *)(l),(void *)(l))));
   printf("%c\n",*(char *)(__boundcheck_ptr_reference(29,20,"main",(void *)(*l),(void *)*l)));
   return 1;
}
