#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
   int i;
   char *a[10];
   
   for(i=0;i<10;i++)
   {
        a[i]=(char *)_boundcheck_malloc(100*sizeof(char));
   }
   strcpy(a[5],"ASADAD");
   printf("%c\n",*(int *)__boundcheck_ptr_reference((void *)(a[5]+2),"main",4,9));
   return 1;
}

