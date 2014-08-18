/*******************************************
  实验九  第 6 题
  题目：求二维数组珠最大值、最小值与平均值
  要求： 在主函数中定义数组并初始化
         三个函数的形参写法不同

       二维数组指针作参数的有三种形式:	       
         int p[4][4]
         int p[][4]
		 int (*p)[4]
********************************************/
#include <iostream.h>

// 解1
// 二维数组指针作参数的三种形式
float Aver(int p[][4],int n) //n 行数，求平均值
{  
	int i,j, sum=0;
	for( i=0 ;i<n;i++)
        for( j=0 ;j<4;j++)
			sum += *(*(p+i)+j) ;
	return float(sum)/(n*4);
}

int Max(int (*p)[4],int n) //n 行数，求最大值
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

// 不妨读读此函数
/* 
int Max(int (*p)[4],int n) //n 行数，求最大值
{  
	 int i,j, *t;
	 t=*p;
	 for( i=0 ;i<n;i++)
	 {
		 for( j=0 ;j<4;j++)
             if(*t<p[i][j]) 
				 t= p[i]+j;
	 }
	 return *t;
}
*/

int Min(int p[4][4],int n) //n 行数，求最小值
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

void main(void)
{ 
	int a[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	cout << "Aver="<< Aver( a, 4 )<< endl;
	cout << "Max="<< Max( a, 4 )<< endl;
	cout << "Min="<< Min( a, 4 )<< endl;
}


// 解2: 将二维数组看成一维数组
/*
void fun(int *a, int n, float *averp, int *maxp, int *minp)
// a: 数组起始地址,元素指针
// n: 元素个数
// averp: 平均值指针
// maxp: 最大值指针     
// minp: 最小值指针
{
	int *p;
	float aver=0; 
	int max=*a, min=*a;
	for(p=a; p<a+n; p++)
	{	
		aver += *p;
		if(max<*p) max=*p;
		if(min>*p) min=*p;
	}
	*averp = float(aver)/n;
	*maxp=max;
	*minp=min;
}

void main ()
{
	int a[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	int max, min;
	float aver;

	fun(a[0], 16, &aver, &max, &min); //将结果通过指针带回
	
	cout<<"average="<<aver<<'\n'; //在主函数中输出结果
	cout<<"max="<<max<<'\n';
	cout<<"min="<<min<<'\n'; 
	
}

*/


