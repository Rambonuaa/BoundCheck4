#include"boundcheck.h"
#include<string.h>
/* bss1.c */
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
static char input[20];
static char output[20];

void __global_variables_init(){
__boundcheck_metadata_store(&input[0],&input[20-1]);
__boundcheck_metadata_store(&output[0],&output[20-1]);
}
int main(int argc, char *argv[])
{
	__global_variables_init();

   __boundcheck_strcpy (input, argv[1]);
   __boundcheck_strcpy (output, "normal output");
   printf ("input at %p: %s\n", input, input);
   printf ("output at %p: %s\n", output, output);
   printf("\n\n%s\n", output);
}
