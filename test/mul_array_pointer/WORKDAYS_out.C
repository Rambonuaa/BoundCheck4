#include"self2.h"
#include<string.h>
#include <stdio.h>

int main(void)
 {
   char *workdays[] = {"Monday", "Tuesday", "Wednesday",
                        "Thursday", "Friday", "" };
   char **work_day;
void *_work_day_base=NULL;
void *_work_day_bound=NULL;


   work_day = workdays;
_work_day_base=&workdays[0];
_work_day_bound=&workdays[6-1];
 

   while (*(char **)(_RV_insert_pointer_check(_work_day_base,_work_day_bound,11,12,"main",(void *)work_day)))
     printf("%s\n", *(char **)(_RV_insert_pointer_check(_work_day_base,_work_day_bound,12,22,"main",(void *)work_day++)));

   return 1;
 }

