/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/*$Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/tst/cod2lin.c,v 1.1.1.1 2000/11/06 19:54:26 mguthaus Exp $*/

#include <stdio.h>
#include <assert.h>

#include	"gsm.h"
#include	"proto.h"

char  * pname;

int	debug      = 0;
int	verbosity  = 0;
int	fast       = 0;
int	error      = 0;

usage P0()
{
	fprintf(stderr, "Usage: %s [-vF] [files...]\n", pname);
	exit(1);
}

void process P2((f, filename), FILE * f, char * filename)
{
	gsm_frame	buf;
__boundcheck_metadata_store((void *)(&buf),(void *)((size_t)(&buf)+sizeof(buf)*8-1));

	gsm_signal	source[160];
__boundcheck_metadata_store((void *)(&source),(void *)((size_t)(&source)+sizeof(source)*8-1));


	int		cc;
__boundcheck_metadata_store((void *)(&cc),(void *)((size_t)(&cc)+sizeof(cc)*8-1));

	gsm		r;
__boundcheck_metadata_store((void *)(&r),(void *)((size_t)(&r)+sizeof(r)*8-1));


	(void)memset(source, 0x00, sizeof(source));

	if (!(r = gsm_create())) {
		perror("gsm_create");
		error = 1;
		return ;
	}
	gsm_option(r, GSM_OPT_VERBOSE, &verbosity);
	gsm_option(r, GSM_OPT_FAST,   &fast);
	for (;;) {
		cc = fread((char *)source, sizeof(*(gsm_signal [160])(__boundcheck_ptr_reference(46,38,"process",(void *)(source),(void *)(source)))), 76, f);
		if (cc == 0) {
			gsm_destroy(r);
			return;
		}
		if (cc != 76) {
			error = 1;
			fprintf(stderr,
				"%s: %s -- %d trailing bytes ignored\n",
				pname, filename, cc);
			gsm_destroy(r);
			return;
		}

		gsm_implode(r, source, buf);
		gsm_decode(r, buf, source);

		if (write(1, source, sizeof(source)) != sizeof(source)) {
			perror("write");
			error = 1;
			gsm_destroy(r);
			return;
		}
	}
}

void __global_variables_init(){
__boundcheck_metadata_store((void *)(&pname),(void *)((size_t)(&pname)+sizeof(pname)*8-1));
__boundcheck_metadata_store((void *)(&debug),(void *)((size_t)(&debug)+sizeof(debug)*8-1));
__boundcheck_metadata_store((void *)(&verbosity),(void *)((size_t)(&verbosity)+sizeof(verbosity)*8-1));
__boundcheck_metadata_store((void *)(&fast),(void *)((size_t)(&fast)+sizeof(fast)*8-1));
__boundcheck_metadata_store((void *)(&error),(void *)((size_t)(&error)+sizeof(error)*8-1));
}
main P2((ac, av), int ac, char ** av)
{
	__global_variables_init();

	int 		opt;
__boundcheck_metadata_store((void *)(&opt),(void *)((size_t)(&opt)+sizeof(opt)*8-1));

	extern char   * optarg;
	extern int	optind;

	char *__tmp_string_0="cod2out";
 __boundcheck_metadata_store((void *)(__tmp_string_0),(void *)(__tmp_string_0+7));
FILE		* f;
__boundcheck_metadata_store((void *)(&f),(void *)((size_t)(&f)+sizeof(f)*8-1));


	if (!(pname = av[__boundcheck_ptr_cast_to_array_reference(80,19,"main",(void *)(av),(void *)(av+0),0)])) pname = __tmp_string_0;

	while ((opt = getopt(ac, av, "vF")) != EOF) switch (opt) {
	case 'v': verbosity++; 	  break;
	case 'F': fast++;	  break;
	default:  usage();
	}

	ac -= optind;
	av += optind;

	if (!ac) process(stdin, "*stdin*");
	else for (; *(char **)(__boundcheck_ptr_reference(92,15,"main",(void *)(av),(void *)(av))); av++) {
		if (!(f = fopen(*(char **)(__boundcheck_ptr_reference(93,20,"main",(void *)(av),(void *)(av))), "r"))) perror(*(char **)(__boundcheck_ptr_reference(93,39,"main",(void *)(av),(void *)(av))));
		else {
			process(f, *(char **)(__boundcheck_ptr_reference(95,16,"main",(void *)(av),(void *)(av))));
			fclose(f);
		}
	}

	exit(error);
}
