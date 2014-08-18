#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
  int i=0;
  char *a[3]={"qwee","rteyy","tytui"};
  int j;
  char *string[10];
  char **p;
  if(i)
   {
      for(j=0;j<10;j++)
      {
        string[j]=(char *)malloc(100*sizeof(char));
      }
      strcpy(string[5],"ASADAD");
      p=&string[5];
   }
   else
      p=a;
   printf("%s\n",*p);
   printf("%c\n",**p);
   printf("a=%p a[0]=%p\n",&a,&a[0]);
   printf("%p %p\n",&p,&(*p));
   return 1;
}
