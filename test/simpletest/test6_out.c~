#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
int main()
{
   int int_arr[5]={0,1,2,3,4};__boundcheck_metadata_store(&int_arr[0],&int_arr[5-1]);

   for(int *pbegin=int_arr,*pend=int_arr+5;pbegin!=pend;++pbegin)
     printf("%d ",*(int *)(__boundcheck_ptr_reference(6,20,"main",(void *)(pbegin),(void *)(pbegin))));
   printf("\n");
   return 1;
}
