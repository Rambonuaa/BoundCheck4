#include"boundcheck.h"
#include<string.h>
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

void __global_variables_init(){
__boundcheck_metadata_store(&s[0],&s[6-1]);
__boundcheck_metadata_store(&ss[0],&ss[8-1]);
__boundcheck_metadata_store(&sss[0],&sss[2-1]);
}
int main()
{
	__global_variables_init();

  int i=1;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  printf("%d\n",s[_RV_insert_check(0,6,39,17,"main",i)].len);
  printf("%d\n",ss[_RV_insert_check(0,8,40,17,"main",i+1)].len);
  printf("%c\n",sss[_RV_insert_check(0,2,41,17,"main",1)].array[_RV_insert_check(0,10,41,17,"main",2)]);
  return 0;
}

