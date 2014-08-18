/*heap.c*/
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
int main(int argc, char *argv[])
{
   char *input = malloc (20);
   char *output = malloc (20);
   printf("%d\n",strlen(output));

   if(20<strlen("normal output"))
      printf("error\n");

   strcpy(output,"normal output");

   if(20<strlen(argv[1]))
      printf("error\n");

   strcpy(input, argv[1]); 
   printf("input at %p: %s\n", input, input);
   printf("output at %p: %s\n", output, output);
   printf("\n\n%s\n", output);
   return 0;
}
