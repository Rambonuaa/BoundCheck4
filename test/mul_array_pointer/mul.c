#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
  int i=0;
  char a[6]={'a','b','c','d','e','U'};
  char *p;
   if(i)
   {
      p=(char *)malloc(7*sizeof(char));
      strcpy(p,"ASDFFG");
   }
   else
      p=a;
   printf("%c\n",*(p+4));
   return 1;
}
