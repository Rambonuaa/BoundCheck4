#include<stdio.h>
#include<stdbool.h>
bool conver(int d[],int b[])
{
   int i;
   int c[5];
   printf("d=%p \n",(void *)d);
   for(i=0;i<5;i++)
    {
      *(int *)__boundcheck_ptr_reference((void *)d,(void *)(d+i),"main",4,9)=*(b+i);
      *(c+i)=*(b+i);
     }
   
   return 1;
}
int main()
{
  int i;
  char string[10];
  __boundcheck_metadata_store((void *)string,(void *)(string+9));
  static int a[5];
  __boundcheck_metadata_store((void *)a,(void *)(a+4));
  int num[5]={1,2,3,4,5};
  __boundcheck_metadata_store((void *)num,(void *)(num+7));
  static int c[10];
  __boundcheck_metadata_store((void *)c,(void *)(c+9));
  printf("a=%p ",(void *)a);
  printf("a+4=%p \n",(void *)(a+4));
  printf("C=%p ",(void *)c);
  printf("c+9=%p \n",(void *)(c+9));
  __boundcheck_strcpy(string,"ASADASDASDASDASD");
  printf("%s\n",string);
  if(conver(a,num))
    for (i = 0; i <5; i++)
      printf("%d  ",a[i]);
  return 1;
}





















