#include"self2.h"
#include<string.h>
#include <stdio.h>
float Aver(void *_p_base,void *_p_bound,int p[][4],int n) 
{  
	int i,j, sum=0;
	for( i=0 ;i<n;i++)
        for( j=0 ;j<4;j++)
	    sum += *(*(int (*)[4])(_RV_insert_pointer_check(_p_base,_p_bound,7,16,"Aver",(void *)(p+i)))+j) ;
	return (float)(sum)/(n*4);
}

int Max(void *_p_base,void *_p_bound,int (*p)[4],int n) 
{  
	int i,j, max=p[_RV_insert_pointertoarray_check(_p_base,_p_bound,13,17,"Max",(void *)(p+0),0)][_RV_insert_pointertoarray_check(_p_base,_p_bound,13,20,"Max",(void *)(p+0),0)];
	for( i=0 ;i<n;i++)
	{
		for( j=0 ;j<4;j++)
			if(max<p[_RV_insert_pointertoarray_check(_p_base,_p_bound,17,13,"Max",(void *)(p+0),0)][_RV_insert_pointertoarray_check(_p_base,_p_bound,17,16,"Max",(void *)(p+j),j)]) max=p[_RV_insert_pointertoarray_check(_p_base,_p_bound,17,26,"Max",(void *)(p+0),0)][_RV_insert_pointertoarray_check(_p_base,_p_bound,17,29,"Max",(void *)(p+j),j)];
		p++;
	}
	return max;
}

int Min(void *_p_base,void *_p_bound,int p[4][4],int n) 
{ 
	int i,j, min=*(int (*)[4])(_RV_insert_pointer_check(_p_base,_p_bound,25,16,"Min",(void *)*(int (*)[4])(_RV_insert_pointer_check(_p_base,_p_bound,25,17,"Min",(void *)p))));
	for( i=0 ;i<n;i++)
	{
        for( j=0 ;j<3;j++)
			if(min> (*(int (*)[4])(_RV_insert_pointer_check(_p_base,_p_bound,29,14,"Min",(void *)p)))[j]) min=(*(int (*)[4])(_RV_insert_pointer_check(_p_base,_p_bound,29,27,"Min",(void *)p)))[j];
			p++;
	}
	return min;
}

int main(void)
{ 
	int a[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        printf("AVer=%f\n",Aver(&a[0],&a[4-1],a,4));
        printf("Max=%d\n",Max(&a[0],&a[4-1],a,4));
        printf("Min=%d\n",Min(&a[0],&a[4-1],a,4));
        return 1;
}