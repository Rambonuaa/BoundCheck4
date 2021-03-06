/* NIST Secure Hash Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sha.h"

int main(int argc, char **argv)
{
    FILE *fin;
__boundcheck_metadata_store((void *)(&fin),(void *)((size_t)(&fin)+sizeof(fin)*8-1));

    SHA_INFO sha_info;
__boundcheck_metadata_store((void *)(&sha_info),(void *)((size_t)(&sha_info)+sizeof(sha_info)*8-1));


    if (argc < 2) {
	fin = stdin;

	sha_stream(&sha_info, fin);
	sha_print(&sha_info);
    } else {
	while (--argc) {
	    fin = fopen(*(++argv), "rb");
	    if (fin == NULL) {
		printf("error opening %s for reading\n", *argv);
	    } else {
		sha_stream(&sha_info, fin);
		sha_print(&sha_info);
		fclose(fin);
	    }
	}
    }
    return(0);
}
