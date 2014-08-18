#include"self2.h"
#include<string.h>
#include <stdio.h>

void swap_values(void *_a_base,void *_a_bound,int *a, void *_b_base,void *_b_bound,int *b)
 { 
   int temp;

   temp = *(int *)(_RV_insert_pointer_check(_a_base,_a_bound,7,12,"swap_values",(void *)a));  // Temporarily hold the value pointed to by a
   *(int *)(_RV_insert_pointer_check(_a_base,_a_bound,8,5,"swap_values",(void *)a)) = *(int *)(_RV_insert_pointer_check(_b_base,_b_bound,8,10,"swap_values",(void *)b));    // Assign b's value to a
   *(int *)(_RV_insert_pointer_check(_b_base,_b_bound,9,5,"swap_values",(void *)b)) = temp;  // Assign a's value to b
 }

int main(void)
 {
   int one = 1, two = 2;

   swap_values(&one,&one,&one, &two,&two,&two);

   printf("one contains %d two contains %d\n", one, two);
   return 1; 
}


   
