#include <stdio.h>
float Aver(int p[][4],int n) 
{  
	int i,j, sum=0;
	for( i=0 ;i<n;i++)
        for( j=0 ;j<4;j++)
	    sum += *(*(p+i)+j) ;
	return (float)(sum)/(n*4);
}

int Max(int (*p)[4],int n) 
{  
	int i,j, max=p[0][0];
	for( i=0 ;i<n;i++)
	{
		for( j=0 ;j<4;j++)
			if(max<p[0][j]) max=p[0][j];
		p++;
	}
	return max;
}

int Min(int p[4][4],int n) 
{ 
	int i,j, min=**p;
	for( i=0 ;i<n;i++)
	{
        for( j=0 ;j<3;j++)
			if(min> (*p)[j]) min=(*p)[j];
			p++;
	}
	return min;
}

int main(void)
{ 
	int a[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        printf("AVer=%f\n",Aver(a,4));
        printf("Max=%d\n",Max(a,4));
        printf("Min=%d\n",Min(a,4));
        return 1;
}
