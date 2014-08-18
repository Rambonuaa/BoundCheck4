/***********************************************************************
  实验九  第2 题
  题目：求字符 串的长度
  要求：用指针作为函数的参数
  注意：
        1. 字符数组作为实参时，形参的写法 
		2. 指针的运算
***********************************************************************/
#include <iostream.h>
/*int my_strlen(char *s)
{
	for(int i=0; *(s+i)!='\0'; i++);  //注意循环体为空
	return(i);
}
*/
int my_strlen(char *s) //另一种实现方法
{
	char *begin=s;
	while(*s) s++;
	return(s-begin);
}

void main( )
{
	char  a[80];
	cout << "Please input a string: \n";
	cin.getline(a, 80);
	cout << "The string Length is: ";
	cout << my_strlen(a) << endl;
}



//在主函数中实现
/*
void main( )
{
	char  a[80],*p=a;
	int len=0;
	cout << "Please input a string: ";
	cin.getline(p, 80);
    while(*p!='\0')
	{
		len++;
		p++;
	}
	cout << "The string Length is: ";
	cout << len << endl;
}

void main( )
{
	char  a[80],*p=a;
	cout << "Please input a string: ";
	cin.getline(p, 80);  //将读入的字符串放到以地址p开始的内存中
    while(*p!='\0')
		p++;
	cout << "The string Length is: ";
	cout << (p-a) << endl;
}
*/
