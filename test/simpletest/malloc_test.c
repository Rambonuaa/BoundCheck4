#include<stdio.h>
#include<stdlib.h>
int main()
{
   int *Se8=(int *)malloc(100);
   printf("%d\n",100/sizeof(int *));
   return 0;
}
