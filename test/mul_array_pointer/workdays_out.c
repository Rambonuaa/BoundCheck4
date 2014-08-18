#include"boundcheck.h"
#include<string.h>
#include <stdio.h>

int main(void)
 {
   char *__tmp_string_5="";
 __boundcheck_metadata_store((void *)(__tmp_string_5),(void *)(__tmp_string_5+0));
char *__tmp_string_4="Friday";
 __boundcheck_metadata_store((void *)(__tmp_string_4),(void *)(__tmp_string_4+6));
char *__tmp_string_3="Thursday";
 __boundcheck_metadata_store((void *)(__tmp_string_3),(void *)(__tmp_string_3+8));
char *__tmp_string_2="Wednesday";
 __boundcheck_metadata_store((void *)(__tmp_string_2),(void *)(__tmp_string_2+9));
char *__tmp_string_1="Tuesday";
 __boundcheck_metadata_store((void *)(__tmp_string_1),(void *)(__tmp_string_1+7));
char *__tmp_string_0="Monday";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+6));
char *workdays[] = {__tmp_string_0, __tmp_string_1, __tmp_string_2,
                        __tmp_string_3, __tmp_string_4, __tmp_string_5 };__boundcheck_metadata_store(&workdays[0],&workdays[6-1]);

   char **work_day;
__boundcheck_metadata_store((void *)(&work_day),(void *)((size_t)(&work_day)+sizeof(work_day)*8-1));


   work_day = workdays;
__boundcheck_metadata_trans_check((void *)(work_day),(void *)(workdays),(void *)(workdays));
 

   while (*(char **)(__boundcheck_ptr_reference(11,12,"main",(void *)(work_day),(void *)(work_day))))
     printf("%s\n", *(char **)(__boundcheck_ptr_reference(12,30,"main",(void *)(work_day),(void *)(work_day++))));

   return 1;
 }

