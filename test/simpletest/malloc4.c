#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
   int i;
   char *a[10];
   for(i=0;i<10;i++)
       a[i]=(char *)malloc(100*sizeof(char));
   strcpy(a[5],"ASADAD");
   printf("%c\n",*(a[5]+1));
   return 1;
}

