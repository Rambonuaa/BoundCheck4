/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/*$Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/tst/lin2txt.c,v 1.1.1.1 2000/11/06 19:54:26 mguthaus Exp $*/

#include <stdio.h>

#include "gsm.h"
#include "proto.h"

char  * pname;

int	debug      = 0;
int	verbosity  = 0;
int	error      = 0;

usage P0()
{
	fprintf(stderr, "Usage: %s [-v] [files...]\n", pname);
	exit(1);
}

void process P2((f, filename), FILE * f, char * filename)
{
	short		source[160];
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));

	int		cc;
__boundcheck_metadata_store((void *)(&cc),(void *)((size_t)(&cc)+sizeof(cc)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int k;
__boundcheck_metadata_store((void *)(&k),(void *)((size_t)(&k)+sizeof(k)*8-1));

	gsm		r;
__boundcheck_metadata_store((void *)(&r),(void *)((size_t)(&r)+sizeof(r)*8-1));


	if (!(r = gsm_create())) {
		perror("gsm_create");
		error = 1;
		return ;
	}
	gsm_option(r, GSM_OPT_VERBOSE, &verbosity);
	for (;;) {

		if ((cc = fread((char *)source, 1, sizeof(source), f)) == 0) {
			gsm_destroy(r);
#ifdef	COUNT_OVERFLOW
			dump_overflow(stderr);
#endif
			return;
		}
		
		printf("{\t");
		for (j = 0; j < 4; j++) {
			printf("{\t");
			for (k = 0; k < 40; k++) {
				printf("%d", (int)source[ _RV_insert_check(0,160,52,31,"process",j * 40 + k )]);
				if (k < 39) {
					printf(", ");
					if (k % 4 == 3) printf("\n\t\t");
				} else {
					printf("\t}");
					if (j == 3) printf("\t},\n");
					else printf(",\n\t");
				}
			}
		}
	}
}

void __global_variables_init(){
__boundcheck_metadata_store((void *)(&pname),(void *)((size_t)(&pname)+sizeof(pname)*8-1));
__boundcheck_metadata_store((void *)(&debug),(void *)((size_t)(&debug)+sizeof(debug)*8-1));
__boundcheck_metadata_store((void *)(&verbosity),(void *)((size_t)(&verbosity)+sizeof(verbosity)*8-1));
__boundcheck_metadata_store((void *)(&error),(void *)((size_t)(&error)+sizeof(error)*8-1));
}
main P2((ac, av), int ac, char ** av)
{
	__global_variables_init();

	int 		opt;
__boundcheck_metadata_store((void *)(&opt),(void *)((size_t)(&opt)+sizeof(opt)*8-1));

	extern char   * optarg;
	extern int	optind;

	char *__tmp_string_0="inp2txt";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+7));
FILE		* f;
__boundcheck_metadata_store((void *)(&f),(void *)((size_t)(&f)+sizeof(f)*8-1));


	if (!(pname = av[__boundcheck_ptr_cast_to_array_reference(74,19,"main",(void *)(av),(void *)(av+0),0)])) pname = __tmp_string_0;

	while ((opt = getopt(ac, av, "v")) != EOF) switch (opt) {
	case 'v': verbosity++;    break;
	default:  usage();
	}

	ac -= optind;
	av += optind;

	if (!ac) process(stdin, "*stdin*");
	else for (; *(char **)(__boundcheck_ptr_reference(85,15,"main",(void *)(av),(void *)(av))); av++) {
		if (!(f = fopen(*(char **)(__boundcheck_ptr_reference(86,20,"main",(void *)(av),(void *)(av))), "r"))) perror(*(char **)(__boundcheck_ptr_reference(86,39,"main",(void *)(av),(void *)(av))));
		else {
			process(f, *(char **)(__boundcheck_ptr_reference(88,16,"main",(void *)(av),(void *)(av))));
			fclose(f);
		}
	}

	exit(error);
}
