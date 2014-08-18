#include"self2.h"
#include<string.h>
#include <stdio.h>

void show_2d_array(void *_array_base,void *_array_bound,int array[][10], int rows)
 {
   int i, j;
   printf("%p\n",_array_bound);

   for (i = 0; i < rows; i++)
     for (j = 0; j < 10; j++)
       printf("array[%d][%d] = %d\n", i, j, *(int *)__valid_read(_array_base,_array_bound,&array[i][j]));
 }


int main(void)
 {
   int a[1][10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
   int b[2][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20}};
   int c[3][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
                   {21, 22, 23, 24, 25, 26, 27, 28, 29, 30}};

//   printf("&a[0][9]=%p ",&a[0][9]);
//   show_2d_array(&a[0],&a[0][9]+9*sizeof(int),a, 1);
   printf("&b[1][9]=%p ",&b[1][9]);
   show_2d_array(&b[0][0],&b[0][0]+19,b, 2);
   printf("&c[2][9]=%p ",&c[2][9]);
   show_2d_array(&c[0][0],&c[0][0]+29,c, 3);
   return 1;
 }
