#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

   char *a[10];
__boundcheck_metadata_store(&a[0],&a[10-1]);

   for(i=0;i<10;i++)
       a[_RV_insert_check(0,10,9,8,"main",i)]=(char *)__boundcheck_malloc(100*sizeof(char));
   __boundcheck_strcpy(a[_RV_insert_check(0,10,10,11,"main",5)],"ASADAD");
   printf("%c\n",*(char *)(__boundcheck_ptr_reference(11,26,"main",(void *)(a[5]),(void *)(a[_RV_insert_check(0,10,11,20,"main",5)]+1))));
   return 1;
}

