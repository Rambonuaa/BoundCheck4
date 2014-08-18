#include"boundcheck.h"
#include<string.h>
/*heap.c*/
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
int main(int argc, char *argv[])
{
   char *input = __boundcheck_malloc (20);
__boundcheck_metadata_store((void *)(&input),(void *)((size_t)(&input)+sizeof(input)*8-1));

   char *output = __boundcheck_malloc (20);
__boundcheck_metadata_store((void *)(&output),(void *)((size_t)(&output)+sizeof(output)*8-1));

   printf("%d\n",strlen(output));
   
   __boundcheck_strcpy(output,"normal output");
   __boundcheck_strcpy(input, argv[1]); 
   printf("input at %p: %s\n", input, input);
   printf("output at %p: %s\n", output, output);
   printf("\n\n%s\n", output);
   return 0;
}

/*
   
*/
//strcpy(char *dest,char *src) 复制src到dest

//strncpy(char *dest,char *src,size_t n) 将src中最多n个字符复制到dest中
//strcat(char *dest,char *src) 将src复制在dest末尾

//memcpy(char *dest,char *src,size_t count) 从src复制count个字符到dest
//memset(char *dest,char c,size_t count) 将dest前面count个字符置换为字符c

//memmove(char *dest,char *src,size_t count) 从src复制count个字符串到dest中
