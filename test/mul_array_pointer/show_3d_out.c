#include"boundcheck.h"
#include<string.h>
#include <stdio.h>

void main(void)
 {
   int row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));
int  column;
__boundcheck_metadata_store((void *)(&column),(void *)((size_t)(&column)+sizeof(column)*8-1));
int  table;
__boundcheck_metadata_store((void *)(&table),(void *)((size_t)(&table)+sizeof(table)*8-1));


   float values[2][3][5] = {
                            {{1.0, 2.0, 3.0, 4.0, 5.0},
                             {6.0, 7.0, 8.0, 9.0, 10.0},
                             {11.0, 12.0, 13.0, 14.0, 15.0}},
                             
                            {{16.0, 17.0, 18.0, 19.0, 20.0},
                             {21.0, 22.0, 23.0, 24.0, 25.0},
                             {26.0, 27.0, 28.0, 29.0, 30.0}}
                           };
int __values_0;

int __values_1;
	for(__values_0=0;__values_0<2;__values_0++)
	for(__values_1=0;__values_1<3;__values_1++)
__boundcheck_metadata_store(&values[__values_0][__values_1][0],&values[__values_0][__values_1][5-1]);

   
   for (row = 0; row < 2; row++)
    for (column = 0; column < 3; column++)
     for (table = 0; table < 5; table++)
      printf("values[%d][%d][%d] = %f\n", row, column, table, 
        values[_RV_insert_check(0,2,21,9,"main",row)][_RV_insert_check(0,3,21,9,"main",column)][_RV_insert_check(0,5,21,9,"main",table)]);
 }

