#include<stdio.h>
#include<stdbool.h>
bool conver(int d[],int b[])
{
   int i;
   int c[5];
   for(i=0;i<5;i++)
    {
      *(d+i)=*(b+i);
      *(c+i)=*(b+i);
     }
   
   return 1;
}
int main()
{
  int i;
  static int a[10];
  int num[5]={1,2,3,4,5};
  static int c[10];
  int *p;
  p=&i;
  p=&num[0];
  printf("%d\n",*(p+1));
  if(conver(a,num))
    for (i = 0; i <5; i++)
      printf("%d  ",a[i]);
  return 1;
}





















