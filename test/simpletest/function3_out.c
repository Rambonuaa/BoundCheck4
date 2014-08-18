#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>
typedef struct data_type{
   int age;
   char name[20];
}data;

void func1(char *p)
{
   printf("the length of func1 is %d\n",strlen(p));
   printf("%c\n",*(char *)(__boundcheck_ptr_reference(13,23,"func1",(void *)(p),(void *)(p+4))));
}
void func2(data *p[])
{
   printf("%s is %d years old\n",(*(data **)(__boundcheck_ptr_reference(17,40,"func2",(void *)(p),(void *)(p+0))))->name,(*(data * *)(__boundcheck_ptr_reference(17,49,"func2",(void *)(&p[0]),(void *)(&p[0]))))->age);
}

void func4(data *p)
{
   printf("%s is %d years old\n",p->name,p->age);
}

void func3(char *p1,char *p2)
{
   printf("the length of func3 is %d\n",strlen(p1));
   __boundcheck_strcpy(p1,p2);
   printf("the length of func3 is %d\n",strlen(p2));
   printf("the string is %s\n",p1);
}

int main()
{
   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

   data *bob[100];__boundcheck_metadata_store(&bob[0],&bob[100-1]);
data  *bob1;
__boundcheck_metadata_store((void *)(&bob1),(void *)((size_t)(&bob1)+sizeof(bob1)*8-1));

   printf("%d    LLLLLLLL\n",sizeof(bob));
   char string[8]="ASDESFP";__boundcheck_metadata_store(&string[0],&string[8-1]);

   char *a=(char *)__boundcheck_malloc(100*sizeof(char));
__boundcheck_metadata_store((void *)(&a),(void *)((size_t)(&a)+sizeof(a)*8-1));

   i=2;
   
   for(i=0;i<100;i++)
   {
      bob1=(data *)__boundcheck_malloc(100*sizeof(data));
      if(bob1!=NULL)
      {
          bob1->age=i;
          __boundcheck_strcpy(bob1->name,"Robert");
          *(data **)(__boundcheck_ptr_reference(49,18,"main",(void *)(bob),(void *)(bob+i)))=bob1;
      }
   }
   for(i=0;i<10;i++)
   {
     printf("%s %d\n",(*(data **)(__boundcheck_ptr_reference(54,42,"main",(void *)((data **)(&bob[i])),(void *)(data **)(&bob[_RV_insert_check(0,100,54,36,"main",i)]))))->name,bob[_RV_insert_check(0,100,54,51,"main",i)]->age);
   }
   
   char *p1;
__boundcheck_metadata_store((void *)(&p1),(void *)((size_t)(&p1)+sizeof(p1)*8-1));
char  *p2;
__boundcheck_metadata_store((void *)(&p2),(void *)((size_t)(&p2)+sizeof(p2)*8-1));

   
   char *__tmp_string_0="DKLRTF";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+6));
char *p3=__tmp_string_0;
__boundcheck_metadata_store((void *)(&p3),(void *)((size_t)(&p3)+sizeof(p3)*8-1));

   p1=string+5;
__boundcheck_metadata_trans_check((void *)(p1),(void *)(string),(void *)(string + 5));

   p2=p3;
__boundcheck_metadata_trans_check((void *)(p2),(void *)(p3),(void *)(p3));

   func1(p1);
   func1(string+5);
   func1(string+i+3);
   func1(5+string);
   func1(p2);  //char [7]   "DKLRTF"  SSSSS
   func2(&bob1);
   func4(bob1);
   func3(a,p3);
   return 1;
}
