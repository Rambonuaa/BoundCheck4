#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
struct struct1{
  int len;
  int interp;
};
int main()
{
	struct struct1 ss;
__boundcheck_metadata_store((void *)(&(ss)),(void *)((size_t)(&(ss))+sizeof(ss)*8-1));

	int var=2;
__boundcheck_metadata_store((void *)(&(var)),(void *)((size_t)(&(var))+sizeof(var)*8-1));

	char *p;
__boundcheck_metadata_store((void *)(&(p)),(void *)((size_t)(&(p))+sizeof(p)*8-1));

	int array1[10];
__boundcheck_metadata_store((void *)(&(array1)),(void *)((size_t)(&(array1))+sizeof(array1)*8-1));

	char array2[10];
__boundcheck_metadata_store((void *)(&(array2)),(void *)((size_t)(&(array2))+sizeof(array2)*8-1));

	printf("%d\n",var);
}
	
