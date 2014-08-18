typedef unsigned int size_t;

extern  void *__store_block(void *ptr,size_t size);

extern  void __delete_block(void *ptr);

extern  void __initialize(void *ptr,size_t size);

extern  void __full_init(void *ptr);

extern  void __literal_string(void *ptr);

extern  int __valid_read(void *ptr,size_t size);

void show_2d_array(int array[][10], int rows)
{
  int i;
  int j;
  __store_block((void *)(& array),4U);
  for (i = 0; i < rows; i++)
     for (j = 0; j < 10; j++)
       printf("array[%d][%d] = %d\n", i, j, *(int *)__valid_read((void *)(& (*(array + i))[j]),sizeof(int)));
}

int main(void)
{
   int a[1   ][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
   __store_block((void *)(a),40U);
   int b[2][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20}};
   __store_block((void *)(b),80U);
   int c[3][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
                   {21, 22, 23, 24, 25, 26, 27, 28, 29, 30}};
  __store_block((void *)(c),120U);
  
//  show_2d_array(a,1);
  show_2d_array(b,2);
  show_2d_array(c,3);
  return 1;
}


