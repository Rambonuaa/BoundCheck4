#include<stdio.h>
#include<string.h>
#define N 4

extern  void *__store_block(void *ptr,size_t size);

extern  void __delete_block(void *ptr);

extern  void __full_init(void *ptr);

extern  void __literal_string(void *ptr);

extern  int __valid_read(void *ptr,size_t size);

extern  int __initialized(void *ptr,size_t size);

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
};

struct struct3{
  int len;
  char array[10];
}sss[]={
        {1,"asdf"},
        {2,"dfds"},
}; 

void __e_acsl_memory_init(void)
{
  
  return;
}

int main(void)
{
  __e_acsl_memory_init();

  __store_block((void *)(s),48U);
  struct struct2 ss[] = {
	{ 6, "definesad","asdfs",	 	1 },
	{ 4, "else","sdfsf",	 	0 },
	{ 5, "endif","sdfweq",	 	0 },
	{ 5, "ifdef","werew",	 	0 },
	{ 6, "ifndef","ewrewq",   	0 },
	{ 2, "if","qwewq",		0 },
	{ 7, "include","rtyrtu",  	1 },
	{ 8, "undefine","tyuytu", 	1 },
  };
  __store_block((void *)(ss),192U);
  
  __store_block((void *)(sss),28U);

  struct struct2 *p=ss;
  printf("ss=%p  ",&ss);
  printf("ss+192=%p ",(&ss)+192);
  __store_block((void *)(& p),4U);
  int *q=&p[2].len;
  __store_block((void *)(& q),4U);
  printf("q=%p ",q); 
  printf("&p[1].len=%p ",&p[1].len);
  printf("lenght=%d\n",(int)&p[1].len-(int)q);
  char *n=p[2].string;
  __store_block((void *)(& n),4U);
  printf("n=%p ",n);
  
  char *m=p[3].string1;
  __store_block((void *)(& m),4U);
  int **l=&q;
  char **k=&n;
  __store_block((void *)(& l),4U);
  printf("*l=%p \n",*l);
  
  printf("%d\n",*q);
  printf("%s\n",n);
  printf("%s\n",m);
  printf("%d\n",**l);
 
  printf("%d\n",*((int *)__valid_read((void *)*((int **)__valid_read((void *)l,sizeof(int *))),sizeof(int))));
//  printf("%s\n",*((int *)__valid_read((void *)*((int **)__valid_read((void *)k,sizeof(int *))),sizeof(int))));
}


