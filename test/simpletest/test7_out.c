#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
int main()
{
/*
   int a=3;
   printf("%p ",&a);
   int *p;
   printf("%p ",(void *)p);
   p=&a;
   printf("%p ",(void *)p);
*/

  char string[10]="asdfdsf";__boundcheck_metadata_store(&string[0],&string[10-1]);

  printf("%p ",&string[_RV_insert_check(0,10,14,17,"main",3)]);
  char *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));

  printf("%p ",(void *)p);
  p=string+3;
__boundcheck_metadata_trans_check((void *)(p),(void *)(string),(void *)(string + 3));

  printf("%p ",(void *)p);
   return 1;
}
   
