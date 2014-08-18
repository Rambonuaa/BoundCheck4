#include<stdio.h>
#define SIZE 5
int main( )
{ 
   char *pc[]={" Beijing", "Shanghai", "Tianjing", "Guangzhou", "Chongqing" };
   _pc_base=&pc[0];
   _pc_base=&pc[5-1];
   char **p;
   
   int i;
   for( i=0; i<SIZE;i++)
   { 
      p = pc+i;
      table_lookup(p)->base=_pc_base;
      table_lookup(p)->bound=_pc_bound;
      printf ("%s\n", *p);
      printf("%c\n",*(*p+2));
   }
   return 0;
}
