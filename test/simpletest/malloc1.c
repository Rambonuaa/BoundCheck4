#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>
typedef struct data_type{
   int age;
   char name[20];
}data;

typedef struct Sequence{
   int a;
   data *first;
   data *last;
}Sequence;

void func1(char *p)
{
   printf("the func1 is %c\n",*(p+6));
}

void func2(data *p)
{
   printf("%s is %d years old\n",p->name,p->age);
}

void func3(char *p1,char *p2)
{
   strcpy(p1,p2);
   printf("the string is %s\n",p1);
}

void func4(Sequence *p)
{

}

void func5(int *p)
{
}

int main()
{
   int i=10;
   int x_size=10,y_size=10;
   data *bob=(data *)malloc(sizeof(data));
   Sequence *Se=(Sequence *)malloc(sizeof(Sequence));
   Sequence *Se1;
   Sequence *Se2=(Sequence *)malloc(100*sizeof(Sequence));
   Sequence *Se3=(Sequence *)malloc(i*sizeof(Sequence));
   Sequence *Se4=(Sequence *)malloc(x_size*y_size*sizeof(Sequence));
   Sequence *Se5=(Sequence *)malloc((x_size)*100*sizeof(Sequence));
   Sequence *Se6=(Sequence *)malloc(sizeof(Sequence)*x_size*y_size);
   int *Se7=(int *)malloc(x_size*y_size);
   int *Se8=(int *)malloc(100);
   char *a=(char *)malloc(100*sizeof(char));
   char *input = malloc (20);
   char *output = malloc (20);

   Se1=(Sequence *)malloc(sizeof(Sequence)*100);

   if(bob!=NULL)
   {
       bob->age=22;
       strcpy(bob->name,"Robert");    
    }
   Se->a=3;
   Se->first=bob;
   printf("the struct is %s\n",Se->first->name);

   char *p1,*p2;
   char string[8]="ASDESFP";
   char *p3="DKLRTF";
   char aa[10];
   p1=string;
   p2=p3;
   printf("the length of func3 is %d\n",sizeof(bob));
   func1(p1);
   func1(p2);
   func1(Se->first->name);
   func2(bob);
   func4(Se);
   func4(Se1);
   func4(Se2);
   func4(Se3);
   func4(Se4);
   func4(Se5);
   func4(Se6);
   func5(Se7);
   func5(Se8);
   func3(a,p3);
   func3(input,output);
   printf("the length of func3 is %d\n",strlen(a));
   return 1;
}
