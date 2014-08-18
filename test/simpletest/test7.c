#include<stdio.h>
int main()
{
/*
   int a=3;
   printf("%p ",&a);
   int *p;
   printf("%p ",(void *)p);
   p=&a;
   printf("%p ",(void *)p);
*/

  char string[10]="asdfdsf";
  printf("%p ",&string[3]);
  char *p;
  printf("%p ",(void *)p);
  p=string+3;
  printf("%p ",(void *)p);
   return 1;
}
   
