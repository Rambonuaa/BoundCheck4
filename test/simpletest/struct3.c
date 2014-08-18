#include<stdio.h>
#include<string.h>
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
  char *string;
  char string1[10];
  int interp;
} ss[] = {
	{ 6, "define","asdfs",	 	1 },
	{ 4, "else","sdfsf",	 	0 },
	{ 5, "endif","sdfweq",	 	0 },
	{ 5, "ifdef","werew",	 	0 },
	{ 6, "ifndef","ewrewq",   	0 },
	{ 2, "if","qwewq",		0 },
	{ 7, "include","rtyrtu",  	1 },
	{ 8, "undefine","tyuytu", 	1 },
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
  struct struct2 *p=ss;
  char *q=&p[2].string[3];
//  printf("%d\n",strlen(p[2].string));
  char *n=p[2].string;
  char *m=p[3].string1;
  char **l=&q;
//  char *q=p[2].string;
  printf("%s\n",q);
  printf("%s\n",n);
  printf("%s\n",m);
  printf("%c\n",**l);
  return 0;
}

