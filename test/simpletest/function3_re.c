#include<string.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>

void func1(char *p)
{
   printf("the length of func1 is %d\n",strlen(p));
   printf("%c\n",*(int *)__boundcheck_ptr_reference((void *)(p+30),"main",4,9));
}

int main()
{
  char string[8]="ASDESFP";
__boundcheck_metadata_store((void *)string,(void *)(string+7));

   
   char *p1;
__boundcheck_metadata_store((void *)p1,(void *)p1);
printf("org p=%p ",&p1);

   p1=string+5;
printf("second p=%p ",&p1);
__boundcheck_copy_metadata((void *)p1,(void *)string);
   func1(p1);

   return 1;
}
