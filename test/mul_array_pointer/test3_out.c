#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
int a[3][3]={1, 2, 3, 4, 5, 6, 7, 8, 9};
int *b[]={a[_RV_insert_check(0,3,3,11,"funlockfile",0)],a[_RV_insert_check(0,3,3,16,"funlockfile",1)],a[_RV_insert_check(0,3,3,21,"funlockfile",2)]};
int **p=b;
void __global_variables_init(){

int __a_0;
	for(__a_0=0;__a_0<3;__a_0++)
__boundcheck_metadata_store(&a[__a_0][0],&a[__a_0][3-1]);
__boundcheck_metadata_store(&b[0],&b[3-1]);

__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
}
int main( )
{
	__global_variables_init();
 
   int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));

   for( i=0; i<3; i++ )
   for ( j=0; j<3; j++ )
       printf("%d,%d,%d\n", *(int *)(__boundcheck_ptr_reference(10,37,"main",(void *)(b[i]),(void *)(b[_RV_insert_check(0,3,10,31,"main",i)]+j))), *(int *)(__boundcheck_ptr_reference(10,50,"main",(void *)(*(p + i)),(void *)(*(p+i)+j))), *(int *)(__boundcheck_ptr_reference(10,63,"main",(void *)(*(a + i)),(void *)(*(a+i)+j))) );
   return 1;
}
