#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
//#include <tcclib.h>
#include<stdbool.h>
bool conver(int d[],int b[])
{
   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

   int c[5];
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));

   for(i=0;i<5;i++)
    {
      d[__boundcheck_ptr_cast_to_array_reference(10,9,"conver",(void *)(d),(void *)(d+i),i)]=b[__boundcheck_ptr_cast_to_array_reference(10,14,"conver",(void *)(b),(void *)(b+i),i)];
      c[_RV_insert_check(0,5,11,9,"conver",i)]=b[__boundcheck_ptr_cast_to_array_reference(11,14,"conver",(void *)(b),(void *)(b+i),i)];
     }
   
   return 1;
}
int main()
{
  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  static int a[10];
__boundcheck_metadata_store((void *)(&a),(void *)((size_t)(&a)+sizeof(a)*8-1));

  int num[5]={1,2,3,4,5};
__boundcheck_metadata_store((void *)(&num),(void *)((size_t)(&num)+sizeof(num)*8-1));

  static int c[10];
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));

  if(conver(a,num))
    for (i = 0; i <5; i++)
      printf("%d  ",a[_RV_insert_check(0,10,24,23,"main",i)]);
  return 1;
}





















