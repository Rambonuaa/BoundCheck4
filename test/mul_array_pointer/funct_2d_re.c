#include <stdio.h>
#include"boundcheck.h"
void show_2d_array(int array[][10], int rows)
 {
   int i, j;

   for (i = 0; i < rows; i++)
     for (j = 0; j < 10; j++)
       printf("array[%d][%d] = %d\n", i, j, *(int *)(__boundcheck_ptr_reference(10,41,"main",(void *)(*(array + i)),(void *)(*(int **)(__boundcheck_ptr_reference(10,43,"main",(void *)(array),(void *)((array+i))))+j))));
 }

void show_3d_array(int ***values,int rows)
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
				printf("values[%d][%d][%d] = %d\n", row,column,table,values[__boundcheck_ptr_cast_to_array_reference(18,65,"show_3d_array",(void *)(values),(void *)(values+row),row)][__boundcheck_ptr_cast_to_array_reference(18,70,"show_3d_array",(void *)(values),(void *)(values+column),column)][__boundcheck_ptr_cast_to_array_reference(18,78,"show_3d_array",(void *)(values),(void *)(values+table),table)]);
}

int main(void)
 {
//   int a[1][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
/*
   int a_1[10]={1,2,3,4,5,6,7,8,9,10};
   __boundcheck_metadata_store((void *)(a_1),(void *)(a_1+sizeof(a_1)-1));
   int *a[]={a_1};
   __boundcheck_metadata_store((void *)(a),(void *)(a+sizeof(a)-1));
   printf("a_1=%d\n",sizeof(a));
   printf("a_2=%d\n",sizeof(a_1));
*/
   

//   int b[2][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
//                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20}};
/*
   int b_21[10]={1,2,3,4,5,6,7,8,9,10};
   int b_22[10]={11,12,13,14,15,16,17,18,19,20};
   int *b[2]={b_21,b_22};
   __boundcheck_metadata_store((void *)(b_21),(void *)(b_21+sizeof(b_21)-1));
   __boundcheck_metadata_store((void *)(b_22),(void *)(b_22+sizeof(b_22)-1));
   __boundcheck_metadata_store((void *)(b),(void *)(b+sizeof(b)-1));
*/
//   int c[3][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
//                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
//                   {21, 22, 23, 24, 25, 26, 27, 28, 29, 30}};
/*
	int c_21[10]={1,2,3,4,5,6,7,8,9,10};
	__boundcheck_metadata_store((void *)(c_21),(void *)(c_21+sizeof(c_21)-1));
	int c_22[10]={11,12,13,14,15,16,17,18,19,20};
	__boundcheck_metadata_store((void *)(c_22),(void *)(c_22+sizeof(c_22)-1));
	int c_23[10]={21,22,23,24,25,26,27,28,29,30};
	__boundcheck_metadata_store((void *)(c_23),(void *)(c_23+sizeof(c_23)-1));
	int *c[3]={c_21,c_22,c_23};	
	__boundcheck_metadata_store((void *)(c),(void *)(c+sizeof(c)-1));
*/

   int _array4_5[4]={21,22,23,24};
__boundcheck_metadata_store((void *)(&_array4_5),(void *)((size_t)(&_array4_5)+sizeof(_array4_5)*8-1));
int _array4_4[4]={17,18,19,20};
__boundcheck_metadata_store((void *)(&_array4_4),(void *)((size_t)(&_array4_4)+sizeof(_array4_4)*8-1));
int _array4_3[4]={13,14,15,16};
__boundcheck_metadata_store((void *)(&_array4_3),(void *)((size_t)(&_array4_3)+sizeof(_array4_3)*8-1));
int _array4_2[4]={9,10,11,12};
__boundcheck_metadata_store((void *)(&_array4_2),(void *)((size_t)(&_array4_2)+sizeof(_array4_2)*8-1));
int _array4_1[4]={5,6,7,8};
__boundcheck_metadata_store((void *)(&_array4_1),(void *)((size_t)(&_array4_1)+sizeof(_array4_1)*8-1));
int _array4_0[4]={1,2,3,4};
__boundcheck_metadata_store((void *)(&_array4_0),(void *)((size_t)(&_array4_0)+sizeof(_array4_0)*8-1));

int* _array4_6[3]={_array4_0,_array4_1,_array4_2};
int *_array4_7[3]={_array4_3,_array4_4,_array4_5};
int **array4[2]={_array4_6,_array4_7};

__boundcheck_metadata_store((void *)(&array4),(void *)((size_t)(&array4)+sizeof(array4)*8-1));

//   show_2d_array(a, 1);
//   show_2d_array(b, 2);
//	show_2d_array(c, 3);
	show_3d_array(array4,4);
   return 1;
 }
