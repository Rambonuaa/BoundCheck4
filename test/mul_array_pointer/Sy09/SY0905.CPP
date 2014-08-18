/*****************************************************
 实验九  第 5 题
  题目：  判断是否实回文
******************************************************/
#include <iostream.h> 
#include <string.h>
#include <ctype.h>
void filter(char s1[]); //过滤: 去除非字母，并全部转换成大写。
int palin(char s[]);
void main()
{ 
	char s[80];
	cin.getline(s,80);
	if(palin(s)) 
		cout<<"Yes.\n";
	else 
		cout<<"No.\n";
}
void filter(char s1[]) //过滤: 去除非字母，并全部转换成大写。
{
	char *p1=s1,*p2=s1;
	while(*p1)
		if(isalpha(*p1))     // isalpha( )参数若是字母返回真
		{
			*p2=toupper(*p1);  // toupper( )函数将参数字母转换成大写字母
			p1++; p2++;
		}
		else p1++;
	*p2=0;
	
}
/*void filter(char s1[]) //过滤: 去除非字母，并全部转换成大写。
{
	char s2[80],*p1=s1,*p2=s2;
	while(*p1)
		if(isalpha(*p1))     // isalpha( )参数若是字母返回真
		{
			*p2=toupper(*p1);  // toupper( )函数将参数字母转换成大写字母
			p1++; p2++;
		}
		else p1++;
	*p2=0;
	strcpy(s1,s2);
}
*/
int palin(char s[])
{
	char *head,*tail;
	char s1[80];  //临时字符串
	strcpy(s1,s);
	filter(s1);
	head=s1; 
	tail=s1+strlen(s1)-1;
	while(head<tail)
		if(*head==*tail) head++,tail--;
		else return(0);
	return(1);
}



/* 
int palin(char s[])
{
	char *head,*tail;
	head=s; 
	tail=s+strlen(s)-1;
	while(head<tail)
	{
		while(!isalpha(*head)) head++;
		while(!isalpha(*tail)) tail--;
		if(toupper(*head)==toupper(*tail)) 
			head++,tail--;
		else return(0);
	}
	return(1);
}
*/
/*
int palin(char s[])
{
	char *p1=s,*p2=s+strlen(s)-1;
	strupr(s);     //将字符串全部转换成大写
	while(p1<p2)
		if('A'<=*p1 && *p1<='Z')
		{
			if('A'<=*p2 && *p2<='Z')
			{
				if(*p1!=*p2)
					return(0);
				else 
					p1++,p2--;
			}
			else
				p2--;
		}
		else
			p1++;
	return(1);
}
*/