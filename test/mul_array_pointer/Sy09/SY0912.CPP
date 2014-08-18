/*****************************************************
 实验九  第 12 题
  题目： 删除字符串中多处出现的子串

******************************************************/
#include <iostream.h> 
void DelSubStr(char *s1, char *s2);
void main()
{  
	char s1[100],s2[20];
	cout<<"请输入两个子串";
	cin.getline(s1,100);
	cin.getline(s2,20);
	DelSubStr(s1,s2);
	cout<<s1<<endl;
}
void DelSubStr(char *s1, char *s2)
{
	char *p1,*p11,*tp,*p2;
	p1=p11=s1;
	while(*p11)
	{
		tp=p11;
		p2=s2;
		while(*tp==*p2 && *p2!='\0')
			tp++,p2++;
		if(*p2=='\0')//是子串
			p11=tp;
		else
		{
			*p1 = *p11;
			p1++;
			p11++;
		}
	}
	*p1='\0';
}


/*解 2 用一个临时字符串

#include <iostream.h> 
void DelSubStr(char *s1, char *s2)
{
	char tempstr[100],*tp=tempstr;
	char *p1,*p11,*p2;
	p11=s1;
	while(*p11)
	{
		p1=p11;
		p2=s2;
		while(*p1==*p2 && *p2!='\0')
			p1++,p2++;
		if(*p2=='\0')//是子串
			p11 = p1;
		else
			*tp++ = *p11++;
	}
	*tp='\0';
	tp=tempstr;
	while(*s1++ = *tp++); //拷回s1
}
void main()
{  
	char s1[100],s2[20];
	cin.getline(s1,100);
	cin.getline(s2,20);
	DelSubStr(s1,s2);
	cout<<s1<<endl;
}
*/
/*解3
void DelSubStr(char *s1, char *s2)
{
	char *p1=s1,*p2=s1,*p3;
	int n=strlen(s2);
	for( ; *p1!='\0'; p1++)
		for(p2=p1,p3=s2; *p2==*p3; p2++,p3++)
			if(p3-s2 == n-1)
			{
				strcpy(p1,p2+1);
				p1--;
				break;
			}
}
*/
/*解4
#include <iostream.h>
#include <string.h>
void delsubstr(char *s1, char *s2)
{
	char a[50],*p2,*pa,*pt;
	strcpy(a,s1);
	for(pa=a; *pa!='\0'; pa++)
	{
		for(p2=s2; *pa!=*p2&&*pa!='\0'; pa++,s1++)
			*s1=*pa;
		for(pt=pa;*p2!='\0'; p2++,pt++)
			if(*pt!=*p2) break;
		if(*p2=='\0'){ pa=pt; pa--;}
		else *s1++=*pa;
	}
	*s1='\0';
}
void main()
{
	char s1[100],s2[50];
	cin.getline(s1,100);
	cin.getline(s2,50);
	delsubstr(s1,s2);
	cout<<s1<<endl;
}
*/