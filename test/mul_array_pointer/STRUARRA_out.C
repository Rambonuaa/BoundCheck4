#include"self2.h"
#include<string.h>
#include <stdio.h>

void main(void)
 {
   struct Date {
     char month_name[64];
     int  month;
     int  day;
     int  year;
   } current_date = { "July", 7, 4, 1994 };

   int i;

   for (i = 0; current_date.month_name[_RV_insert_check(0,64,14,40,"main",i)]; i++)
     putchar(current_date.month_name[_RV_insert_check(0,64,15,38,"main",i)]);
 }

