#include<stdio.h>
#include<math.h>
int main()
{
  printf("Min_signed_int=%lf\n",0-pow(2,(sizeof(signed int)*8-1)));
  printf("Max_signed_int=%lf\n",pow(2,(sizeof(signed int)*8-1))-1);
  printf("Min_unsigned_int=0\n");
  printf("Max_unsigned_int=%lf\n",pow(2,(sizeof(unsigned int)*8))-1);
  return 0;
}
