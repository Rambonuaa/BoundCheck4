#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>
typedef struct data_type{
   int age;
   char name[20][30];
}data;

int main()
{
  data *bob1;
__boundcheck_metadata_store((void *)(&bob1),(void *)((size_t)(&bob1)+sizeof(bob1)*8-1));

  bob1=(data *)__boundcheck_malloc(sizeof(data));
  __boundcheck_strcpy(bob1->name[_RV_insert_check(0,20,14,10,"main",1)],"Robert");
  printf("%c\n",bob1->name[_RV_insert_check(0,20,15,17,"main",1)][_RV_insert_check(0,30,15,17,"main",2)]);
}
