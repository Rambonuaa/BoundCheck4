#include"boundcheck.h"
#include<string.h>
#include <stdio.h>

void show_string(char *string)
 { 
   while (*(char *)(__boundcheck_ptr_reference(5,12,"show_string",(void *)(string),(void *)(string))))
     putchar(*(char *)(__boundcheck_ptr_reference(6,21,"show_string",(void *)(string),(void *)(string++))));
 }

int main(void)
 {
   char *__tmp_string_0="Jamsa's 1001 C/C++ Tips";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+23));
show_string(__tmp_string_0);
   return 1; 
}

