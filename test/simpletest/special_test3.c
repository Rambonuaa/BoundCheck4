#include<stdio.h>
#include<string.h>
#define N 4



struct struct2 {
  int len;
  char *string;
  char string1[10];
  int interp;
};

int main()
{
   __global_variables_init();
   char *__tmp_string1="define";
   __boundcheck_metadata_store((void *)(__tmp_string1),(void *)(__tmp_string1+7-1));
   char *__tmp_string2="else";
   __boundcheck_metadata_store((void *)(__tmp_string2),(void *)(__tmp_string2+7-1));
   struct model ss[]= {
	{ 6, __tmp_string1,__tmp_string2,	 	1 };

   struct struct2 *p=ss;
   char *q=p[1].string1;
//   char *m=p[1].string1;
   char **l=&q;
   printf("%s\n",*l);
   printf("%c\n",**l);
   return 1;
}
