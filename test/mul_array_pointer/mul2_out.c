#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
  int i=0;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  char *__tmp_string_2="tytui";
 __boundcheck_metadata_store((void *)(__tmp_string_2),(void *)(__tmp_string_2+5));
char *__tmp_string_1="rteyy";
 __boundcheck_metadata_store((void *)(__tmp_string_1),(void *)(__tmp_string_1+5));
char *__tmp_string_0="qwee";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+4));
char *a[3]={__tmp_string_0,__tmp_string_1,__tmp_string_2};__boundcheck_metadata_store(&a[0],&a[3-1]);

  int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));

  char *string[10];__boundcheck_metadata_store(&string[0],&string[10-1]);

  char **p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));

  if(i)
   {
      for(j=0;j<10;j++)
      {
        string[_RV_insert_check(0,10,15,9,"main",j)]=(char *)__boundcheck_malloc(100*sizeof(char));
      }
      __boundcheck_strcpy(string[_RV_insert_check(0,10,17,14,"main",5)],"ASADAD");
      p=&string[_RV_insert_check(0,10,18,10,"main",5)];
   }
   else
      p=a;
__boundcheck_metadata_trans_check((void *)(p),(void *)(a),(void *)(a));

   printf("%s\n",*(char **)(__boundcheck_ptr_reference(22,19,"main",(void *)(p),(void *)(p))));
   printf("%c\n",*(char *)(__boundcheck_ptr_reference(23,20,"main",(void *)(*p),(void *)*p)));
   printf("a=%p a[0]=%p\n",&a,&a[_RV_insert_check(0,3,24,32,"main",0)]);
   printf("%p %p\n",&p,&(*(char **)(__boundcheck_ptr_reference(25,27,"main",(void *)(p),(void *)(p)))));
   return 1;
}
