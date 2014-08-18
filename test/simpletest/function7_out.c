#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
bool conver(int d[],int b[])
{
   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)-1));

   int c[5];__boundcheck_metadata_store(&c[0],&c[5-1]);

   for(i=0;i<5;i++)
    {
      *(int *)(__boundcheck_ptr_reference(9,12,"conver",(void *)(d),(void *)(d+i)))=*(int *)(__boundcheck_ptr_reference(9,19,"conver",(void *)(b),(void *)(b+i)));
      *(int *)(__boundcheck_ptr_reference(10,12,"conver",(void *)(c),(void *)(c+i)))=*(int *)(__boundcheck_ptr_reference(10,19,"conver",(void *)(b),(void *)(b+i)));
     }
   
   return 1;
}
int main()
{
  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)-1));

  static int a[10];__boundcheck_metadata_store(&a[0],&a[10-1]);

  int num[5]={1,2,3,4,5};__boundcheck_metadata_store(&num[0],&num[5-1]);

  static int c[10];__boundcheck_metadata_store(&c[0],&c[10-1]);

  int *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)-1));
  printf("%d\n",*(int *)(__boundcheck_ptr_reference(24,22,"main",(void *)(p),(void *)(p))));

  p=&i;
  p=&num[_RV_insert_check(0,5,23,6,"main",0)];
  printf("%d\n",*(int *)(__boundcheck_ptr_reference(24,22,"main",(void *)(p),(void *)(p+1))));
  if(conver(a,num))
    for (i = 0; i <5; i++)
      printf("%d  ",a[_RV_insert_check(0,10,27,21,"main",i)]);
  return 1;
}





















