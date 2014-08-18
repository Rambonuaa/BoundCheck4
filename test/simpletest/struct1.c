#include<stdio.h>
#define N 4
struct struct1 {
  int len;
  int interp;
} s[] = {
	{ 6, 1 },
	{ 4, 0 },
	{ 5, 0 },
	{ 5, 0 },
	{ 6, 0 },
	{ 2, 0 },
};
struct struct2 {
  int len;
  char *struct1;
  int interp;
} ss[] = {
	{ 6, "define",	 	1 },
	{ 4, "else",	 	0 },
	{ 5, "endif",	 	0 },
	{ 5, "ifdef",	 	0 },
	{ 6, "ifndef",   	0 },
	{ 2, "if",		0 },
	{ 7, "include",  	1 },
	{ 8, "undefine", 	1 },
};
struct struct3{
  int len;
  char array[10];
}sss[]={
        {1,"asdf"},
        {2,"dfds"},
}; 

int main()
{
  int i=1;
  printf("%d\n",s[i].len);
  printf("%d\n",ss[i+1].len);
  printf("%c\n",sss[1].array[2]);
  return 0;
}

