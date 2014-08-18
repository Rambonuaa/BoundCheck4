/* run.config
   OPT: -rte -warn-signed-overflow -print -machdep x86_32
*/
#include<stdio.h>
int main() 
{
  int x=1;
  printf("x=%p \n",&x);
  int *p,*q;
  printf("p=%p \n",(void *)p);
  int tab[10]={0};

  p = &x;
  printf("p--=%p \n",(void *)p);
  q = p;
  *q = *p + 5;

  tab[3] = *q;
  tab[x] = *q;

  p = &tab[2];
  p = &tab[x];

  *(p+1) = tab[0];
  *(p+1) = tab[x];

  *q=p[2];

  return 0;
}
