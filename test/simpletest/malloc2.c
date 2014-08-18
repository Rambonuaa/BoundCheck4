#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>
typedef struct data_type{
   int age;
   char name[20][30];
}data;

int main()
{
  data *bob1;
  bob1=(data *)malloc(sizeof(data));
  strcpy(bob1->name[1],"Robert");
  printf("%c\n",bob1->name[1][2]);
}
