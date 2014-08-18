#include<stdio.h>
#include<string.h>
#define N 4

char *__string1="define";
char *__string2="asdfs";
char *__string3="else";
char *__string4="sdfweq";

struct struct2 {
  int len;
  char *string;
  char string1[10];
  int interp;
};

struct struct2 ss[2]= {
	{ 6, __string1,*__string2,	 	1 },
	{ 4, __string3,*__string4,	 	0 }
   };

int main()
{
   struct struct2 *p=ss;
   char *q=&p[1].string[0];
   char **l=&q;
   printf("%s\n",*l);
   printf("%c\n",**l);
   return 1;
}
