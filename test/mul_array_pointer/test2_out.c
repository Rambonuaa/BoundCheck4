#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#define SIZE 5
int main( )
{ 
   char *__tmp_string_4="Chongqing";
 __boundcheck_metadata_store((void *)(__tmp_string_4),(void *)(__tmp_string_4+9));
char *__tmp_string_3="Guangzhou";
 __boundcheck_metadata_store((void *)(__tmp_string_3),(void *)(__tmp_string_3+9));
char *__tmp_string_2="Tianjing";
 __boundcheck_metadata_store((void *)(__tmp_string_2),(void *)(__tmp_string_2+8));
char *__tmp_string_1="Shanghai";
 __boundcheck_metadata_store((void *)(__tmp_string_1),(void *)(__tmp_string_1+8));
char *__tmp_string_0=" Beijing";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+8));
char *pc[]={__tmp_string_0, __tmp_string_1, __tmp_string_2, __tmp_string_3, __tmp_string_4 };__boundcheck_metadata_store(&pc[0],&pc[5-1]);

   char **p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));

   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

   for( i=0; i<SIZE;i++)
   { 
      p = pc+i;
__boundcheck_metadata_trans_check((void *)(p),(void *)(pc),(void *)(pc + i));

      printf ("%s\n", *(char **)(__boundcheck_ptr_reference(11,24,"main",(void *)(p),(void *)(p))));
      printf("%c\n",*(char *)(__boundcheck_ptr_reference(12,27,"main",(void *)(*p),(void *)(*p+2))));
   }
   return 0;
}
