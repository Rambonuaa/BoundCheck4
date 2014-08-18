#include<stdio.h>
void  swap( int *m, int *n)
{	
	int  t;  
	t=*m;	*m=*n;	*n=t;
}
int main(void)
{	
	int a, b, c;
	scanf("%d%d%d",&a,&b,&c);
	if(a>b) swap(&a,&b);
	if(a>c) swap(&a,&c);
	if(b>c) swap(&b,&c);
	printf("%d\t%d\t%d\n",a,b,c);
        return 1;
}
