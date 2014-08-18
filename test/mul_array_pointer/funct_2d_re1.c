#include <stdio.h>
#include"boundcheck.h"

void show_2d_array(int array[][10], int rows)
{
   int i, j;

	for (i = 0; i < rows; i++)
		for (j = 0; j < 10; j++)
	{
//		printf("array[%d][%d] = %d\n", i, j, *(*(array+i+1)+j));
		printf("array[%d][%d] = %d\n", i, j, *(int *)(__boundcheck_ptr_reference(9,46,"show_2d_array",(void *)(&array[i]),(void *)(&array[i][j]))));
		printf("array[%d][%d] = %d\n", i, j, *(int *)(__boundcheck_ptr_reference(9,46,"show_2d_array",(void *)(*(array + i)),(void *)(*(array+i)+j))));
	}
//printf("array[%d][%d] = %d\n", i, j, *(*(int **)(__boundcheck_ptr_reference(10,41,"main",(void *)(array),(void *)(array+1)))));
}


void show_3d_array(int values[][3][4],int rows)
{
	int row;
//__boundcheck_metadata_store(&(row),&(row));
int column;
//__boundcheck_metadata_store(&(column),&(column));
int table;
//__boundcheck_metadata_store(&(table),&(table));

	for (row = 0; row < 2; row++)
		for (column = 0; column < 3; column++)
			for (table = 0; table < rows; table++)
				printf("values[%d][%d][%d] = %d\n", row,column,table,*(int *)(__boundcheck_ptr_reference(9,46,"show_2d_array",(void *)(&values[row][column]),(void *)(&values[row][column][table]))));
}

int main(void)
 {
/*
   int sd[10][20];
//   __boundcheck_metadata_store(&sd[0][0],&sd[0][19]);

   int array1[10];
   int array2[10]={1,2,3,4,5,6,7,8,9,10};
   int a[1][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
*/

   int b[2][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20}};
	int __b_i;
	for(__b_i=0;__b_i<2;__b_i++)	
	{
		__boundcheck_metadata_store(&b[__RV_i][0],&b[__RV_i][9]);
	}

//	__boundcheck_metadata_store(&b[0][0],&b[1][9]);

/*

   int array5[][10]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
   int c[3][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
                   {21, 22, 23, 24, 25, 26, 27, 28, 29, 30}};
   int array3[2][3][4];
*/
	
   int array4[2][3][4]={{{1,2,3,4},{5,6,7,8},{9,10,11,12}},{{13,14,15,16},{17,18,19,20},{21,22,23,24}}};
	int __array4_i,__array4_j;
	for(__array4_i=0;__array4_i<2;__array4_i++)
		for(__array4_j=0;__array4_j<3;__array4_j++)
			__boundcheck_metadata_store(&array4[__array4_i][__array4_j][0],&array4[__array4_i][__array4_j][3]);

//   show_2d_array(a, 1);
	show_2d_array(b, 2);
//   show_2d_array(c, 3);
//	show_3d_array(array4,4);
   return 1;
 }
