/*******************************************
  实验九  第 7 题
  题目： 统计字符串中整数的个数
  例： 输入  abc345 rfgy678  kjfgj978
       输出  345  678  978
  要求： 
      1. 在主函数中输入输出 
	  2. 将字符串中的整数放入数组，由指针实现
********************************************/
#include <iostream.h>
int  selectnum(char *p, int *b)
{	
	int count=0; //整数串个数
	int num;

	while(*p!='\0')
	{
		while( !(*p>='0'&&*p<='9')) p++;
			
		num=0;
		while (*p>='0'&&*p<='9')
		{
			num = num*10 + *p - '0';
			p++;
		}
		b[count++]=num;	
	
	}
	return count;
}
/*
int  selectnum(char *p, int *b)
{	
	int count=0; //整数串个数
	int num;

	while(*p!='\0')
	{
		if (*p>='0'&&*p<='9')//如果碰到第一个数字字符
		{ 
			num=0;
			while (*p>='0'&&*p<='9')
			{
				num = num*10 + *p - '0';
				p++;
			}
			b[count++]=num;
		}
		else p++; //若非数字字符，指针加 1
	}
	return count;
}
*/
void main ()
{
	char s[100];
	int b[10],count;
	
	cout<<"please input a string: "<<endl;

	cin.getline(s,100); //输入带空格的字符串

	count = selectnum(s, b);
	
	cout<<"count=" << count << "\n";

	for (int i=0; i<count; i++)
		cout<<"b["<<i<<"]=" << *(b+i) << endl;
}

