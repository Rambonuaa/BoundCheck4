#include"boundcheck.h"
#include<string.h>
#include <stdio.h>
int main()
{
  int a=5;
__boundcheck_metadata_store((void *)(&a),(void *)((size_t)(&a)+sizeof(a)*8-1));

  int *b=&a;
__boundcheck_metadata_store((void *)(&b),(void *)((size_t)(&b)+sizeof(b)*8-1));

  int ** c = &b;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));

  printf("%d\n",*(int *)(__boundcheck_ptr_reference(7,19,"main",(void *)(*c),(void *)*c)));
  *(int *)(__boundcheck_ptr_reference(8,5,"main",(void *)(*c),(void *)*c)) =10;
  printf("a=**c=%d\n",*(int *)(__boundcheck_ptr_reference(9,25,"main",(void *)(*c),(void *)*c)));
  return 1;
}
