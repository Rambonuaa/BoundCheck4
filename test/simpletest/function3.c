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
   printf("%c\n",*(p+4));
}
void func2(data *p[])
{
   printf("%s is %d years old\n",(*(p+0))->name,p[0]->age);
}

void func4(data *p)
{
   printf("%s is %d years old\n",p->name,p->age);
}

void func3(char *p1,char *p2)
{
   printf("the length of func3 is %d\n",strlen(p1));
   strcpy(p1,p2);
   printf("the length of func3 is %d\n",strlen(p2));
   printf("the string is %s\n",p1);
}

int main()
{
   int i;
   data *bob[100],*bob1;
   printf("%d    LLLLLLLL\n",sizeof(bob));
   char string[8]="ASDESFP";
   char *a=(char *)malloc(100*sizeof(char));
   i=2;
   
   for(i=0;i<100;i++)
   {
      bob1=(data *)malloc(100*sizeof(data));
      if(bob1!=NULL)
      {
          bob1->age=i;
          strcpy(bob1->name,"Robert");
          *(bob+i)=bob1;
      }
   }
   for(i=0;i<10;i++)
   {
     printf("%s %d\n",(*(data **)(&bob[i]))->name,bob[i]->age);
   }
   
   char *p1,*p2;
   
   char *p3="DKLRTF";
   p1=string+5;
   p2=p3;
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
