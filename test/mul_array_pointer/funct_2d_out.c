#include"boundcheck.h"
#include<string.h>
#include <stdio.h>

void show_2d_array(int array[][10], int rows)
 {
   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));


   for (i = 0; i < rows; i++)
     for (j = 0; j < 10; j++)
       printf("array[%d][%d] = %d\n", i, j, *(int *)(__boundcheck_ptr_reference(9,59,"show_2d_array",(void *)(*(array + i)),(void *)(*(array+i)+j))));
 }

void show_3d_array(int values[][3][4],int rows)
{
	int row;
__boundcheck_metadata_store((void *)(&row),(void *)((size_t)(&row)+sizeof(row)*8-1));
int column;
__boundcheck_metadata_store((void *)(&column),(void *)((size_t)(&column)+sizeof(column)*8-1));
int table;
__boundcheck_metadata_store((void *)(&table),(void *)((size_t)(&table)+sizeof(table)*8-1));

	for (row = 0; row < 2; row++)
		for (column = 0; column < 3; column++)
			for (table = 0; table < rows; table++)
				printf("values[%d][%d][%d] = %d\n", row,column,table,(*(int *)(__boundcheck_ptr_reference(18,58,"show_3d_array",(void *)(&values[row][column][0]),(void *)(&values[row][column][table])))));
}

int main(void)
 {
   int sd[10][20];
int __sd_0;
	for(__sd_0=0;__sd_0<10;__sd_0++)
__boundcheck_metadata_store(&sd[__sd_0][0],&sd[__sd_0][20-1]);

   int array1[10];__boundcheck_metadata_store(&array1[0],&array1[10-1]);

   int array2[10]={1,2,3,4,5,6,7,8,9,10};__boundcheck_metadata_store(&array2[0],&array2[10-1]);

   int a[1][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
int __a_0;
	for(__a_0=0;__a_0<1;__a_0++)
__boundcheck_metadata_store(&a[__a_0][0],&a[__a_0][10-1]);

   int b[2][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20}};
int __b_0;
	for(__b_0=0;__b_0<2;__b_0++)
__boundcheck_metadata_store(&b[__b_0][0],&b[__b_0][10-1]);

   int array5[][10]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
int __array5_0;
	for(__array5_0=0;__array5_0<2;__array5_0++)
__boundcheck_metadata_store(&array5[__array5_0][0],&array5[__array5_0][10-1]);

   int c[3][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
                   {21, 22, 23, 24, 25, 26, 27, 28, 29, 30}};
int __c_0;
	for(__c_0=0;__c_0<3;__c_0++)
__boundcheck_metadata_store(&c[__c_0][0],&c[__c_0][10-1]);

   int array3[2][3][4];
int __array3_0;

int __array3_1;
	for(__array3_0=0;__array3_0<2;__array3_0++)
	for(__array3_1=0;__array3_1<3;__array3_1++)
__boundcheck_metadata_store(&array3[__array3_0][__array3_1][0],&array3[__array3_0][__array3_1][4-1]);

   int array4[2][3][4]={{{1,2,3,4},{5,6,7,8},{9,10,11,12}},{{13,14,15,16},{17,18,19,20},{21,22,23,24}}};
int __array4_0;

int __array4_1;
	for(__array4_0=0;__array4_0<2;__array4_0++)
	for(__array4_1=0;__array4_1<3;__array4_1++)
__boundcheck_metadata_store(&array4[__array4_0][__array4_1][0],&array4[__array4_0][__array4_1][4-1]);


   show_2d_array(a, 1);
   show_2d_array(b, 2);
   show_2d_array(c, 3);
   show_3d_array(array4,4);
   printf("%d\n",array1[_RV_insert_check(0,10,40,18,"main",1)]);
   printf("%d\n",a[_RV_insert_check(0,1,41,18,"main",0)][_RV_insert_check(0,10,41,18,"main",4)]);
   return 1;
 }
