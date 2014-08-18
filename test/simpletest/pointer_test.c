#include<stdio.h>
#include<stdlib.h>
int main()
{
   int size=10;
   int *ptr=malloc(size);
   printf("*ptr=%d\n",sizeof(*ptr));
   printf("ptr=%d\n",sizeof(ptr));
   return 1;
}
