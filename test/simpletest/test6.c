#include<stdio.h>
int main()
{
   int int_arr[5]={0,1,2,3,4};
   for(int *pbegin=int_arr,*pend=int_arr+5;pbegin!=pend;++pbegin)
     printf("%d ",*pbegin);
   printf("\n");
   return 1;
}
