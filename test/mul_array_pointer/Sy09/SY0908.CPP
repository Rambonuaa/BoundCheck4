/***********************************************************************
  实验九  第 8 题
  题目：三个数由小到大排序
  要求：变量的引用作为函数的参数　
  
  注意：实参和形参的写法 
***********************************************************************/
#include<iostream.h>
void  swap( int &m, int &n)
{	
	int  t;  
	t=m;	m=n;	n=t;
}
void main(void)
{	
	int a, b, c;
	cin>>a>>b>>c;
	if(a>b) swap(a,b);
	if(a>c) swap(a,c);
	if(b>c) swap(b,c);
	cout<<a<<'\t'<<b<<'\t'<<c<<'\n';
}
