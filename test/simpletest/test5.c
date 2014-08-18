#include<stdio.h>
#include<stdbool.h>

bool conver(int d[],int b[])
{
   int i,*p,*q;
   p=b;
   printf("%d\n",*(p+2));
   for(i=0;i<5;i++)
      d[i]=b[i];    
   return 1;
}

int main()
{
   char *p2=NULL;
   char string[8]="ASDDSFF";
   char *p3="DDDFF";
   int i;

   static int a[10];
   int num[5]={1,2,3,4,5};
   if(conver(a,num))
    for (i = 0; i <5; i++)
      printf("%d  ",a[i]);
 
   return 0;
}
