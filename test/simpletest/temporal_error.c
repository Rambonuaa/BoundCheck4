#include<stdio.h>
#include<stdlib.h>

int main()
{
	//dangling stack pointer dereference	


	//dangling heap pointer dereference
	int *m,*n;
	m=(int *)malloc(10*sizeof(int));
	m[1]=5;
	n=m;
        printf("the 2st is %d\n",n[1]);
	free(m);
	printf("the 2st is %d\n",n[1]);
	
    //multiple deallocations
	int  *c,*d;
	c=(int *)malloc(10*sizeof(int));
	d=c;
        printf("the 2st is %d\n",c[1]);
        printf("the 2st is %d\n",d[1]);
	free(c);
        printf("the 2st is %d\n",d[1]);
	free(d);
        printf("the 2st is %d\n",d[1]);
}
	
	
	
