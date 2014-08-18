#include<stdio.h>
typedef unsigned int size_t;

extern void *__store_block(void *ptr,size_t size);

extern  void __delete_block(void *ptr);

extern  void __full_init(void *ptr);

extern  void __literal_string(void *ptr);

extern  void* __valid_read(void *ptr,size_t size);

int a[3][3] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int *b[3] = {a[0], a[1], a[2]};
int **p = b;
void __e_acsl_memory_init(void)
{
  __store_block((void *)(& p),4U);
  __store_block((void *)(b),12U);
  __store_block((void *)(a),36U);
  return;
}

int main(void)
{
  __e_acsl_memory_init();
  int i;
  int j;
  for( i=0; i<3; i++ )
   for ( j=0; j<3; j++ )
       printf("%d,%d,%d\n", *((int *)__valid_read((void *)(b[i] + j),sizeof(int))), *((int *)__valid_read((void *)(*((int **)__valid_read((void *)(p + i+2),sizeof(int *))) + j),sizeof(int))), a[i][j] );
  return 1;
}


