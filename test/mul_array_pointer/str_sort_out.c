#include"boundcheck.h"
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bubble_sort(char *array[], int size)
 {
   char *temp;
__boundcheck_metadata_store((void *)(&temp),(void *)((size_t)(&temp)+sizeof(temp)*8-1));
 
   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));


   for (i = 0; i < size; i++) 
    for (j = 0; j < size; j++)
      if (strcmp((*(char * *)(__boundcheck_ptr_reference(12,18,"bubble_sort",(void *)(&array[0]),(void *)(&array[i])))), (*(char * *)(__boundcheck_ptr_reference(12,28,"bubble_sort",(void *)(&array[0]),(void *)(&array[j]))))) < 0)
        {
          temp = (*(char * *)(__boundcheck_ptr_reference(14,18,"bubble_sort",(void *)(&array[0]),(void *)(&array[i]))));
          (*(char * *)(__boundcheck_ptr_reference(15,11,"bubble_sort",(void *)(&array[0]),(void *)(&array[i])))) = (*(char * *)(__boundcheck_ptr_reference(15,22,"bubble_sort",(void *)(&array[0]),(void *)(&array[j]))));
          (*(char * *)(__boundcheck_ptr_reference(16,11,"bubble_sort",(void *)(&array[0]),(void *)(&array[j])))) = temp;
        }
  printf("%c\n",(*(char *)(__boundcheck_ptr_reference(18,17,"bubble_sort",(void *)(&array[1][0]),(void *)(&array[1][2])))));
  }

int main(void)
 {
   char *__tmp_string_4="DDDDEFG";
 __boundcheck_metadata_store((void *)(__tmp_string_4),(void *)(__tmp_string_4+7));
char *__tmp_string_3="EEEDEF";
 __boundcheck_metadata_store((void *)(__tmp_string_3),(void *)(__tmp_string_3+6));
char *__tmp_string_2="BBBDE";
 __boundcheck_metadata_store((void *)(__tmp_string_2),(void *)(__tmp_string_2+5));
char *__tmp_string_1="CCCD";
 __boundcheck_metadata_store((void *)(__tmp_string_1),(void *)(__tmp_string_1+4));
char *__tmp_string_0="AAA";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+3));
char *values[] = {__tmp_string_0, __tmp_string_1, __tmp_string_2, __tmp_string_3, __tmp_string_4};__boundcheck_metadata_store(&values[0],&values[5-1]);

   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));


   bubble_sort(values, 5);

   for (i = 0; i < 5; i++)
     printf("%s ", values[_RV_insert_check(0,5,29,20,"main",i)]);
   return 1;
 }

