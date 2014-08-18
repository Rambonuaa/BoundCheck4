/***********************************************************************
  实验九  第3 题
  题目：拷贝字符串
  要求：用指针作为函数的参数
  注意：
        1. 字符数组作为实参时，形参的写法 
		2. 指针的运算
***********************************************************************/
#include <iostream.h>

//以下用子函数实现

void my_strcpy(char *s1, char *s2)
{  
	while(*s2!='\0')
	{
		*s1=*s2;
		s1++;
		s2++;
	}
	*s1='\0';
}


void main( )
{
	char  a[80], b[80];
	cout << "Please input a source string: ";
	cin.getline(a, 80);
	my_strcpy(b, a);
	cout << "The target string is: " << b << endl;
}

/*

void my_strcpy(char *to, char *from) //另一种实现
{  
	while((*to++ = *from++)!='\0') ;  
}

void my_strcpy(char *to,char *from)
   {  while(*to++ = *from++) ;  }

*/

//在主函数中实现
/*

void main( )
{
	char  a[80], b[80];
	char *p1=a, *p2=b;
	cout << "Please input a source string: ";
	cin.getline(p1, 80);  //将读入的字符串放到以地址p1开始的内存中
	while(*p1 != '\0')
	{
		*p2=*p1;
		p1++;
		p2++;
	}
	*p2='\0';
	cout << "The target string is: ";
	cout << b << endl;
}

*/
