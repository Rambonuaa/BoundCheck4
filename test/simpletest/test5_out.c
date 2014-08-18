#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<stdbool.h>

bool conver(int d[],int b[])
{
   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
int  *q;
__boundcheck_metadata_store((void *)(&q),(void *)((size_t)(&q)+sizeof(q)*8-1));

   p=b;
__boundcheck_metadata_trans_check((void *)(p),(void *)(b),(void *)(b));

   printf("%d\n",*(int *)(__boundcheck_ptr_reference(8,23,"conver",(void *)(p),(void *)(p+2))));
   for(i=0;i<5;i++)
      (*(int *)(__boundcheck_ptr_reference(10,7,"conver",(void *)(&d[0]),(void *)(&d[i]))))=(*(int *)(__boundcheck_ptr_reference(10,12,"conver",(void *)(&b[0]),(void *)(&b[i]))));    
   return 1;
}

int main()
{
   char *p2=NULL;
__boundcheck_metadata_store((void *)(&p2),(void *)((size_t)(&p2)+sizeof(p2)*8-1));

   char string[8]="ASDDSFF";__boundcheck_metadata_store(&string[0],&string[8-1]);

   char *__tmp_string_0="DDDFF";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+5));
char *p3=__tmp_string_0;
__boundcheck_metadata_store((void *)(&p3),(void *)((size_t)(&p3)+sizeof(p3)*8-1));

   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


   static int a[10];__boundcheck_metadata_store(&a[0],&a[10-1]);

   int num[5]={1,2,3,4,5};__boundcheck_metadata_store(&num[0],&num[5-1]);

   if(conver(a,num))
    for (i = 0; i <5; i++)
      printf("%d  ",a[_RV_insert_check(0,10,25,21,"main",i)]);
 
   return 0;
}
