#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>
typedef struct data_type{
   int age;
   char *string;
}data;

void func2(data *p)
{
   printf("%c is %d years old\n",*(char *)(__boundcheck_ptr_reference(12,47,"func2",(void *)(p->string),(void *)(p->string+3))),p->age);
}

int main()
{
  char len[8]="ASDESFP";__boundcheck_metadata_store(&len[0],&len[8-1]);

  data *bob=(data *)__boundcheck_malloc(sizeof(data));
__boundcheck_metadata_store((void *)(&bob),(void *)((size_t)(&bob)+sizeof(bob)*8-1));

  if(bob!=NULL)
  {
     bob->age=3;
     bob->string=len;
  }
  func2(bob);
  return 1;
}
