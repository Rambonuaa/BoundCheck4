#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<string.h>
#define N 4



struct struct2 {
  int len;
  char *string;
  char string1[10];
  int interp;
};

int main()
{
   char *__string1="define";
__boundcheck_metadata_store((void *)(__string1),(void *)(__string1+7-1));

   char *__string2="asdfs";
__boundcheck_metadata_store((void *)(__string2),(void *)(__string2+6-1));

   char *__string3="else";
__boundcheck_metadata_store((void *)(__string3),(void *)(__string3+5-1));

   char *__string4="sdfweq";
__boundcheck_metadata_store((void *)(__string4),(void *)(__string4+7-1));


   struct struct2 ss[2]= {
	{ 6, __string1,*(char *)(__boundcheck_ptr_reference(22,18,"main",(void *)(__string2),(void *)(__string2))),	 	1 },
	{ 4, __string3,*(char *)(__boundcheck_ptr_reference(23,18,"main",(void *)(__string4),(void *)(__string4))),	 	0 }
   };
__boundcheck_metadata_store((void *)(ss),(void *)(ss+2-1));

   struct struct2 *p=ss;
__boundcheck_metadata_store(&(p),&(p));

   char *q=&p[__boundcheck_ptr_cast_to_array_reference(26,15,"main",(void *)(p),(void *)(p+1),1)].string1[_RV_insert_check(0,10,26,26,"main",0)];
__boundcheck_metadata_store((void *)(&p[1].string1[0]),(void *)(p[1].string1[0]+strlen(p[1].string1)-1));

   char **l=&q;
__boundcheck_copy_metadata((void *)(l),(void *)(q));

   printf("%s\n",*(char **)(__boundcheck_ptr_reference(28,19,"main",(void *)(l),(void *)(l))));
//   printf("%c\n",*(char **)(__boundcheck_ptr_reference(29,19,"main",(void *)(l),(void *)(*(char **)(__boundcheck_ptr_reference(29,20,"main",(void *)(l),(void *)(l)))))));
   return 1;
}
