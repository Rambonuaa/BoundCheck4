#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
  int i=0;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  char a[6]={'a','b','c','d','e','U'};__boundcheck_metadata_store(&a[0],&a[6-1]);

  char *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));

   if(i)
   {
      p=(char *)__boundcheck_malloc(7*sizeof(char));
      __boundcheck_strcpy(p,"ASDFFG");
   }
   else
      p=a;
__boundcheck_metadata_trans_check((void *)(p),(void *)(a),(void *)(a));

   printf("%c\n",*(char *)(__boundcheck_ptr_reference(16,23,"main",(void *)(p),(void *)(p+4))));
   return 1;
}
