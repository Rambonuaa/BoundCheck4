#include"self2.h"
#include<string.h>
#include <stdio.h>

void show_string(void *_string_base,void *_string_bound,char *string)
 { 
   while (*(char *)(_RV_insert_pointer_check(_string_base,_string_bound,5,12,"show_string",(void *)string)))
     putchar(*(char *)(_RV_insert_pointer_check(_string_base,_string_bound,6,15,"show_string",(void *)string++)));
 }

int main(void)
 {
   show_string((void *)"Jamsa's 1001 C/C++ Tips",(void *)("Jamsa's 1001 C/C++ Tips"+23-1),"Jamsa's 1001 C/C++ Tips");
   return 1; 
}

