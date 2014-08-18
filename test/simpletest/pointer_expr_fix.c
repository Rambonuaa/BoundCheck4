#include"self.h"
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include<malloc.h>

void func2(void *base,void *up,char *p)
{
   char *q;
   int i=1;
   q=p+i+1;
   printf("%c\n",*(char *)(_RV_insert_pointer_check(base,up,9,19,"func2",(void *)p)));
//   printf("%c\n",*(char *)(_RV_insert_pointer_check(base,up,10,19,"func2",(void *)(p+3))));
   p--;
   printf("%c\n",*(char *)(_RV_insert_pointer_check(base,up,14,19,"func2",(void *)p)));
   p++;
   printf("%c\n",*(char *)(_RV_insert_pointer_check(base,up,17,19,"func2",(void *)p)));

   printf("%c\n",*(char *)(_RV_insert_pointer_check(base,up,15,19,"func2",(void *)q)));
   q--;
   printf("%c\n",*(char *)(_RV_insert_pointer_check(base,up,16,19,"func2",(void *)q)));
   q--;
   printf("%c\n",*(char *)(_RV_insert_pointer_check(base,up,17,19,"func2",(void *)q)));
}
void func3(void *base,void *up,int *p)
{
   printf("func3-1=%d\n",*(int *)(_RV_insert_pointer_check(base,up,18,27,"func3",(void *)p)));
   p--;
   printf("func3-2=%d\n",*(int *)(_RV_insert_pointer_check(base,up,20,27,"func3",(void *)p)));
   p++;
   printf("func3-3=%d\n",*(int *)(_RV_insert_pointer_check(base,up,22,27,"func3",(void *)p)));
   p++;
   printf("func3-4=%d\n",*(int *)(_RV_insert_pointer_check(base,up,24,27,"func3",(void *)p)));
}

int main()
{
   char *p1,*p2=NULL;
   char *p3="DDDFF";
   char string[8]="ASDDSEF";
   int i,*ptr;
   static int a[10];
   int num[5]={1,2,3,4,5};
   i=1;

   p1=string+6;
   ptr=num+4;
/*
   printf("p3=%c\n",*(char *)(_RV_insert_pointer_check((void *)p3,(void *)(p3+6-1),40,22,"main",(void *)p3)));
   printf("p3+3=%c\n",*(char *)(_RV_insert_pointer_check((void *)p3,(void *)(p3+6-1),41,24,"main",(void *)(p3+3))));
   printf("p3+i+3=%c\n",*(char *)(_RV_insert_pointer_check((void *)p3,(void *)(p3+6-1),42,26,"main",(void *)(p3+i+3))));
   printf("i+p3+3=%c\n",*(char *)(_RV_insert_pointer_check((void *)p3,(void *)(p3+6-1),43,26,"main",(void *)(i+p3+3))));
   printf("p3+(i+3)=%c\n",*(char *)(_RV_insert_pointer_check((void *)p3,(void *)(p3+6-1),44,28,"main",(void *)(p3+(i+3)))));
   printf("i+(p3+3)=%c\n",*(char *)(_RV_insert_pointer_check((void *)p3,(void *)(p3+6-1),44,28,"main",(void *)(i+(p3+3)))));
   printf("ptr=%d\n",*(int *)(_RV_insert_pointer_check((void *)(num+4),(void *)(num+5-1),46,23,"main",(void *)ptr)));
   printf("p1+2=%c\n",*(char *)(_RV_insert_pointer_check((void *)string,(void *)(string+8-1),47,24,"main",(void *)(p1+2))));
*/ 
//   func3((void *)num,(void *)(num+5-1),num);
//   func3((void *)num,(void *)(num+5-1),ptr);
//   func2((void *)string,(void *)(string+8-1),string);
   func2((void *)string,(void *)(string+8-1),p1);
   return 0;
}
