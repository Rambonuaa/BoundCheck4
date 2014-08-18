#include"self2.h"
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bubble_sort(void *_array_base,void *_array_bound,char *array[], int size)
 {
   char *temp;
void *_temp_base=NULL;
void *_temp_bound=NULL;
 
   int i, j;

   for (i = 0; i < size; i++) 
    for (j = 0; j < size; j++)
      if (strcmp(array[_RV_insert_pointertoarray_check(_array_base,_array_bound,12,24,"bubble_sort",(void *)(array+i),i)], array[_RV_insert_pointertoarray_check(_array_base,_array_bound,12,34,"bubble_sort",(void *)(array+j),j)]) < 0)
        {
          temp = array[_RV_insert_pointertoarray_check(_array_base,_array_bound,14,24,"bubble_sort",(void *)(array+i),i)];
          array[_RV_insert_pointertoarray_check(_array_base,_array_bound,15,17,"bubble_sort",(void *)(array+i),i)] = array[_RV_insert_pointertoarray_check(_array_base,_array_bound,15,28,"bubble_sort",(void *)(array+j),j)];
          array[_RV_insert_pointertoarray_check(_array_base,_array_bound,16,17,"bubble_sort",(void *)(array+j),j)] = temp;
        }
  }

int main(void)
 {
   char *values[] = {"AAA", "CCC", "BBB", "EEE", "DDD"};
   int i;

   bubble_sort(&values[0],&values[5-1],values, 5);

   for (i = 0; i < 5; i++)
     printf("%s ", values[_RV_insert_check(0,5,28,27,"main",i)]);
   return 1;
 }

