#include<stdio.h>
#include<stdlib.h>


void func()
{
  int a[10],*m;
  int i=4;
  m = (int *)malloc(15);
/*
  a[i*2] = 1;
  *(a+i) = 1;
  m[i*2] = 3;
  *(m+i) = 1;
*/
  *(i+m)=2;
  printf("%d\n",*(i+m));
/*
  foo(a+8);
  foo(m+8);
  foo(8+m);
  foo(m+i+4);
  foo(i+4+m);
  foo(i+m+4);
*/
}
int main()
{
  func();
  return 0;
}
