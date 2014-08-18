#include <stdio.h>
int main()
{
  int a=5;
  int *b=&a;
  int ** c = &b;
  printf("%d\n",**c);
  **c =10;
  printf("a=**c=%d\n",**c);
  return 1;
}
