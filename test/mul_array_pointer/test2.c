#include<stdio.h>
#define SIZE 5
int main( )
{ 
   char *pc[]={" Beijing", "Shanghai", "Tianjing", "Guangzhou", "Chongqing" };
   char **p;
   int i;
   for( i=0; i<SIZE;i++)
   { 
      p = pc+i;
      printf ("%s\n", *p);
      printf("%c\n",*(*p+2));
   }
   return 0;
}
