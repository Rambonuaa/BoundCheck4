#include"self2.h"
#include<string.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>
typedef struct data_type{
   int age;
   char name[20];
}data;

void func1(void *_p_base,void *_p_bound,char *p)
{
   printf("the length of func1 is %d\n",strlen(p));
   printf("%c\n",*(char *)(_RV_insert_pointer_check(_p_base,_p_bound,13,19,"func1",(void *)(p+4))));
}
void func2(void *_p_base,void *_p_bound,data *p[])
{
   printf("%s is %d years old\n",p[_RV_insert_pointertoarray_check(_p_base,_p_bound,17,36,"func2",p,0*sizeof(data *))]->name,p[_RV_insert_pointertoarray_check(_p_base,_p_bound,17,47,"func2",p,0*sizeof(data *))]->age);
}
void func3(void *_p1_base,void *_p1_bound,char *p1,void *_p2_base,void *_p2_bound,char *p2)
{
   printf("the length of func3 is %d\n",strlen(p1));
   if((_p1_bound-_p1_base)/sizeof(char)<strlen(p2)) printf("buffer overflow at 17:47 in func3\n");
   strcpy(p1,p2);
   printf("the length of func3 is %d\n",strlen(p2));
   printf("the string is %s\n",p1);
}

int main()
{
   int i;
   data *bob[100],*bob1;
void *_bob1_base=bob1;
void *_bob1_bound=NULL;

   char *a=(char *)malloc(100*sizeof(char));
void *_a_base=a;
void *_a_bound=a+100 * sizeof(char)/sizeof(char)-1;

   i=2;
   
   for(i=0;i<100;i++)
   {
      bob1=(data *)malloc(100*sizeof(data));
_bob1_base=bob1;
_bob1_bound=bob1+100 * sizeof(data)/sizeof(data)-1;

      if(bob1!=NULL)
      {
          bob1->age=i;
          if(20<strlen("Robert")) printf("buffer overflow at 17:47 in main\n");
          strcpy(bob1->name,"Robert");
          bob[_RV_insert_check(0,100,41,15,"main",i)]=bob1;
      }
   }
   for(i=0;i<10;i++)
   {
     printf("%s %d\n",bob[_RV_insert_check(0,100,46,27,"main",i)]->name,bob[_RV_insert_check(0,100,46,40,"main",i)]->age);
   }
   
   
   char *p1;
void *_p1_base=p1;
void *_p1_bound=NULL;
char *p2;
void *_p2_base=p2;
void *_p2_bound=NULL;

   char string[8]="ASDESFP";
   char *p3="DKLRTF";
void *_p3_base=p3;
void *_p3_bound=p3+7-1;

   p1=string+5;
_p1_base=&string[0];
_p1_bound=&string[8-1];

   p2=p3;
_p2_base=_p3_base;
_p2_bound=_p3_bound;

   func1(_p1_base,_p1_bound,p1);
   func1(&string[0],&string[8-1],string+5);
   func1(&string[0],&string[8-1],string+i+3);
   func1(&string[0],&string[8-1],5+string);
   func1(_p2_base,_p2_bound,p2);  //char [7]   "DKLRTF"  SSSSS
   func2(0,0,&bob1);
   func3(_a_base,_a_bound,a,_p3_base,_p3_bound,p3);
   return 1;
}
