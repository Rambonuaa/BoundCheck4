#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	//dangling stack pointer dereference	


	//dangling heap pointer dereference
	int *m;
__boundcheck_metadata_store((void *)(&m),(void *)((size_t)(&m)+sizeof(m)*8-1));
int  *n;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));

	m=(int *)__boundcheck_malloc(10*sizeof(int));
	(*(int *)(__boundcheck_ptr_reference(12,2,"main",(void *)(&m[0]),(void *)(&m[1]))))=5;
	n=m;
__boundcheck_metadata_trans_check((void *)(n),(void *)(m),(void *)(m));

        printf("the 2st is %d\n",(*(int *)(__boundcheck_ptr_reference(14,34,"main",(void *)(&n[0]),(void *)(&n[1])))));
	__boundcheck_free(m);
	printf("the 2st is %d\n",(*(int *)(__boundcheck_ptr_reference(16,27,"main",(void *)(&n[0]),(void *)(&n[1])))));
	
    //multiple deallocations
	int  *c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));
int  *d;
__boundcheck_metadata_store((void *)(&d),(void *)((size_t)(&d)+sizeof(d)*8-1));

	c=(int *)__boundcheck_malloc(10*sizeof(int));
	d=c;
__boundcheck_metadata_trans_check((void *)(d),(void *)(c),(void *)(c));

        printf("the 2st is %d\n",(*(int *)(__boundcheck_ptr_reference(22,34,"main",(void *)(&c[0]),(void *)(&c[1])))));
        printf("the 2st is %d\n",(*(int *)(__boundcheck_ptr_reference(23,34,"main",(void *)(&d[0]),(void *)(&d[1])))));
	__boundcheck_free(c);
        printf("the 2st is %d\n",(*(int *)(__boundcheck_ptr_reference(25,34,"main",(void *)(&d[0]),(void *)(&d[1])))));
	__boundcheck_free(d);
        printf("the 2st is %d\n",(*(int *)(__boundcheck_ptr_reference(27,34,"main",(void *)(&d[0]),(void *)(&d[1])))));
}
	
	
	
