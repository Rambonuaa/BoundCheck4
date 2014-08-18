/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/add-test/add_test.c,v 1.1.1.1 2000/11/06 19:54:22 mguthaus Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gsm.h"

#include "../src/add.c"

int		interactive = 1;

char		* opname;
longword	L_op1, L_op2, L_expect;
word		op1, op2, expect;
int		do_expect;

word M_gsm_add P((word op1, word op2));
word M_gsm_sub P((word op1, word op2));
word M_gsm_mult P((word op1, word op2));
word M_gsm_mult_r P((word op1, word op2));
word M_gsm_abs P((word op1));
longword M_gsm_L_mult P((word op1, word op2));
longword M_gsm_L_add P((longword op1, longword op2));

help()
{
puts( "  add a b      sub a b     mult a b   div    a b" );
puts( "L_add A B    L_sub A B   L_mult A B   mult_r a b" );
puts( "" );
puts( "abs   a      norm  a        >> a b      << a b" );
puts( "                          L_>> A B    L_<< A B" );

}

char * strtek P2((str, sep), char * str, char * sep) {

	static char     * S = (char *)0;
__boundcheck_metadata_store((void *)(&S),(void *)((size_t)(&S)+sizeof(S)*8-1));

	char		* c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));
 *char * base;
__boundcheck_metadata_store((void *)(&base),(void *)((size_t)(&base)+sizeof(base)*8-1));


	if (str) S = str;
__boundcheck_metadata_trans_check((void *)(S),(void *)(str),(void *)(str));


	if (!S || !*(char *)(__boundcheck_ptr_reference(49,14,"strtek",(void *)(S),(void *)(S)))) return (char *)0;

	/*  Skip delimiters.
	 */
	while (*(char *)(__boundcheck_ptr_reference(53,10,"strtek",(void *)(S),(void *)(S)))) {
		for (c = sep; *(char *)(__boundcheck_ptr_reference(54,18,"strtek",(void *)(c),(void *)(c))) && *(char *)(__boundcheck_ptr_reference(54,24,"strtek",(void *)(c),(void *)(c))) != *(char *)(__boundcheck_ptr_reference(54,30,"strtek",(void *)(S),(void *)(S))); c++) ;
		if (*(char *)(__boundcheck_ptr_reference(55,8,"strtek",(void *)(c),(void *)(c)))) *(char *)(__boundcheck_ptr_reference(55,12,"strtek",(void *)(S),(void *)(S++))) = 0;
		else break;
	}

	base = S;
__boundcheck_metadata_trans_check((void *)(base),(void *)(S),(void *)(S));


	/*   Skip non-delimiters.
	 */
	for (base = S; *(char *)(__boundcheck_ptr_reference(63,18,"strtek",(void *)(S),(void *)(S))); S++) {

		for (c = sep; *(char *)(__boundcheck_ptr_reference(65,18,"strtek",(void *)(c),(void *)(c))); c++)
			if (*(char *)(__boundcheck_ptr_reference(66,9,"strtek",(void *)(c),(void *)(c))) == *(char *)(__boundcheck_ptr_reference(66,15,"strtek",(void *)(S),(void *)(S)))) {
				*(char *)(__boundcheck_ptr_reference(67,6,"strtek",(void *)(S),(void *)(S++))) = 0;
				return base;
			}
	}

	return base == S ? (char *)0 : base;
}

long value P1((s), char * s)
{
	switch (*(char *)(__boundcheck_ptr_reference(77,11,"value",(void *)(s),(void *)(s)))) {
	case '-': switch (s[__boundcheck_ptr_cast_to_array_reference(78,22,"value",(void *)(s),(void *)(s+1),1)]) {
		  case '\0': return MIN_WORD;
		  case '-':  return MIN_LONGWORD;
		  default:   break;
		  }
		  break;

	case '+': switch (s[__boundcheck_ptr_cast_to_array_reference(85,22,"value",(void *)(s),(void *)(s+1),1)]) {
		  case '\0': return MAX_WORD;
		  case '+':  return MAX_LONGWORD;
		  default:   break;
		  }
	default:  break;
	}

	return strtol(s, (char **)0, 0);
}

char * parse P1((buf), char * buf)
{
	char  * s;
__boundcheck_metadata_store((void *)(&s),(void *)((size_t)(&s)+sizeof(s)*8-1));
 *char * a;
__boundcheck_metadata_store((void *)(&a),(void *)((size_t)(&a)+sizeof(a)*8-1));

	long	l;
__boundcheck_metadata_store((void *)(&l),(void *)((size_t)(&l)+sizeof(l)*8-1));


	if (a = strchr(buf, '=')) *(char *)(__boundcheck_ptr_reference(101,29,"parse",(void *)(a),(void *)(a++))) = 0;

	opname = s = strtek(buf, " \t(");
__boundcheck_metadata_trans_check((void *)(opname),(void *)(s),(void *)(s = strtek(buf, " \t(")));
 
	if (!s) return (char *)0;

	op1 = op2 = L_op1 = L_op2 = 0;

	if (s = strtek( (char *)0, "( \t,")) {
		op1 = L_op1 = value(s);
		if (s = strtek( (char *)0, ", \t)")) op2 = L_op2 = value(s);
	}

	if (a) {
		do_expect = 1;
		while (*(char *)(__boundcheck_ptr_reference(115,11,"parse",(void *)(a),(void *)(a))) == ' ' || *(char *)(__boundcheck_ptr_reference(115,24,"parse",(void *)(a),(void *)(a))) == '\t') a++;
		expect = L_expect = value(a);
	}

	return opname;
}

void fprint_word P2((f, w), FILE * f,  word w)
{
	if (!w) putc('0', f);
	else fprintf(f, "0x%4.4x (%d%s)",
		(unsigned int)w,
		(int)w,
		w == MIN_WORD? "/-" : (w == MAX_WORD ? "/+" : ""));
}

void print_word P1((w), word w)
{
	fprint_word( stdout, w );
}

void fprint_longword P2((f, w), FILE * f, longword w)
{
	if (!w) putc('0', f);
	else fprintf(f, "0x%8.8x (%ld%s)",
		w, w, w == MIN_WORD ? "/-"
		: (w == MAX_WORD ? "/+"
		: (w == MIN_LONGWORD ? "/--" 
		: (w == MAX_LONGWORD ? "/++" : ""))));
}

void print_longword P1((w),longword w)
{
	fprint_longword(stdout, w);
}

void do_longword P1((w), longword w)
{
	if (interactive) print_longword(w);
	if (do_expect) {
		if (w != L_expect) {
			if (!interactive) fprint_longword(stderr, w);
			fprintf(stderr, " != %s (%ld, %ld) -- expected ",
				opname, L_op1, L_op2 );
			fprint_longword(stderr, L_expect);
			putc( '\n', stderr );
		}
	} else if (interactive) putchar('\n');
}

void do_word P1((w), word w )
{
	if (interactive) print_word(w);
	if (do_expect) {
		if (w != expect) {
			if (!interactive) fprint_word(stderr, w);
			fprintf(stderr, " != %s (%ld, %ld) -- expected ",
				opname, L_op1, L_op2 );
			fprint_word(stderr, expect);
			putc('\n', stderr);
		}
	} else if (interactive) putchar('\n');
}

void __global_variables_init(){
__boundcheck_metadata_store((void *)(&bitoff),(void *)((size_t)(&bitoff)+sizeof(bitoff)*8-1));
__boundcheck_metadata_store((void *)(&interactive),(void *)((size_t)(&interactive)+sizeof(interactive)*8-1));
__boundcheck_metadata_store((void *)(&opname),(void *)((size_t)(&opname)+sizeof(opname)*8-1));
__boundcheck_metadata_store((void *)(&L_op1),(void *)((size_t)(&L_op1)+sizeof(L_op1)*8-1));
__boundcheck_metadata_store((void *)(&L_op2),(void *)((size_t)(&L_op2)+sizeof(L_op2)*8-1));
__boundcheck_metadata_store((void *)(&L_expect),(void *)((size_t)(&L_expect)+sizeof(L_expect)*8-1));
__boundcheck_metadata_store((void *)(&op1),(void *)((size_t)(&op1)+sizeof(op1)*8-1));
__boundcheck_metadata_store((void *)(&op2),(void *)((size_t)(&op2)+sizeof(op2)*8-1));
__boundcheck_metadata_store((void *)(&expect),(void *)((size_t)(&expect)+sizeof(expect)*8-1));
__boundcheck_metadata_store((void *)(&do_expect),(void *)((size_t)(&do_expect)+sizeof(do_expect)*8-1));
}
int main(ac, av) char ** av;
{
	__global_variables_init();

	char	buf[299];
__boundcheck_metadata_store((void *)(&buf),(void *)((size_t)(&buf)+sizeof(buf)*8-1));

	char	* c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));

	FILE 	* in;
__boundcheck_metadata_store((void *)(&in),(void *)((size_t)(&in)+sizeof(in)*8-1));


	if (ac > 2) {
		fprintf(stderr, "Usage: %s [filename]\n", av[__boundcheck_ptr_cast_to_array_reference(186,48,"main",(void *)(av),(void *)(av+0),0)]);
fail:
#ifdef EXIT_FAILURE
		exit(EXIT_FAILURE);
#else
		exit(1);
#endif
	}
	if (ac < 2) in = stdin;
__boundcheck_metadata_trans_check((void *)(in),(void *)(stdin),(void *)(stdin));

	else if (!(in = fopen(av[__boundcheck_ptr_cast_to_array_reference(195,27,"main",(void *)(av),(void *)(av+1),1)], "r"))) {
		perror(av[__boundcheck_ptr_cast_to_array_reference(196,13,"main",(void *)(av),(void *)(av+1),1)]);
		fprintf(stderr, "%s: cannot open file \"%s\" for reading\n",
			av[__boundcheck_ptr_cast_to_array_reference(198,7,"main",(void *)(av),(void *)(av+0),0)], av[__boundcheck_ptr_cast_to_array_reference(198,14,"main",(void *)(av),(void *)(av+1),1)]);
		goto fail;
	}

	interactive = isatty(fileno(in));

	for (;;) {
		if (interactive) fprintf(stderr, "? ");

		if (!fgets(buf, sizeof(buf), in)) exit(0);
		if (c = strchr(buf, '\n')) *(char *)(__boundcheck_ptr_reference(208,31,"main",(void *)(c),(void *)(c))) = 0;

		if (*(char [299])(__boundcheck_ptr_reference(210,8,"main",(void *)(buf),(void *)(buf))) == ';' || *(char [299])(__boundcheck_ptr_reference(210,23,"main",(void *)(buf),(void *)(buf))) == '#') continue;
		if (*(char [299])(__boundcheck_ptr_reference(211,8,"main",(void *)(buf),(void *)(buf))) == '\'') {
			puts(buf + 1);
			continue;
		}
		if (*(char [299])(__boundcheck_ptr_reference(215,8,"main",(void *)(buf),(void *)(buf))) == '\"') {
			fprintf(stderr,  "%s\n", buf + 1);
			continue;
		}

		c = parse(buf);

		if (!c) continue;
		if (!strcmp(c,   "add")) {
			do_word(    gsm_add( op1, op2 ));
			continue;
		}
		if (!strcmp(c,   "M_add")) {
			do_word(    M_gsm_add( op1, op2 ));
			continue;
		}
		if (!strcmp(c, "sub")) {
			do_word(    gsm_sub( op1, op2 ));
			continue;
		}
		if (!strcmp(c, "M_sub")) {
			do_word(    M_gsm_sub( op1, op2 ));
			continue;
		}
		if (!strcmp(c, "mult")) {
			do_word(    gsm_mult( op1, op2 ));
			continue;
		}
		if (!strcmp(c, "M_mult")) {
			do_word(    M_gsm_mult( op1, op2 ));
			continue;
		}
		if (!strcmp(c, "mult_r")) {
			do_word(    gsm_mult_r(op1, op2));
			continue;
		}
		if (!strcmp(c, "M_mult_r")) {
			do_word(    M_gsm_mult_r(op1, op2));
			continue;
		}
		if (!strcmp(c, "abs" )) {
			do_word(    gsm_abs(op1) );
			continue;
		} 
		if (!strcmp(c, "M_abs" )) {
			do_word(    M_gsm_abs(op1) );
			continue;
		} 
		if (!strcmp(c, "div" )) {
			do_word(    gsm_div( op1, op2 ));
			continue;
		}
		if (!strcmp(c,  "norm" )) {
			do_word(	gsm_norm(L_op1));
			continue;
		} 
		if (!strcmp(c,  "<<" )) {
			do_word(    gsm_asl( op1, op2));
			continue;
		} 
		if (!strcmp(c,  ">>" )) {
			do_word(    gsm_asr( op1, op2 ));
			continue;
		}
		if (!strcmp(c,  "L_mult")) {
			do_longword( gsm_L_mult( op1, op2 ));
			continue;
		}
		if (!strcmp(c,  "M_L_mult")) {
			do_longword( M_gsm_L_mult( op1, op2 ));
			continue;
		}
		if (!strcmp(c,  "L_add" )) {
			do_longword( gsm_L_add( L_op1, L_op2 ));
			continue;
		} 
		if (!strcmp(c,  "M_L_add" )) {
			do_longword( M_gsm_L_add( L_op1, L_op2 ));
			continue;
		} 
		if (!strcmp(c,  "L_sub" )) {
			do_longword( gsm_L_sub( L_op1, L_op2 ));
			continue;
		} 
		if (!strcmp(c,  "L_<<" )) {
			do_longword(    gsm_L_asl( L_op1, L_op2 ));
			continue;
		} 
		if (!strcmp(c,  "L_>>")) {
			do_longword(    gsm_L_asr( L_op1, L_op2 ));
			continue;
		}
		help();
	}
}

#include "private.h"

/*
 * Function stubs for macro implementations of commonly used
 * math functions
 */
word M_gsm_add P2((op1, op2),word op1, word op2)
{
	longword ltmp;
__boundcheck_metadata_store((void *)(&ltmp),(void *)((size_t)(&ltmp)+sizeof(ltmp)*8-1));

	return GSM_ADD(op1, op2);
}

word M_gsm_sub P2((op1, op2), word op1, word op2)
{
	longword ltmp;
__boundcheck_metadata_store((void *)(&ltmp),(void *)((size_t)(&ltmp)+sizeof(ltmp)*8-1));

	return GSM_SUB(op1, op2);
}

word M_gsm_mult P2((op1, op2), word op1, word op2)
{
	return GSM_MULT(op1, op2);
}

word M_gsm_mult_r P2((op1, op2), word op1, word op2)
{
	return GSM_MULT_R(op1, op2);
}

word M_gsm_abs P1((op1), word op1)
{
	return GSM_ABS(op1);
}

longword M_gsm_L_mult P2((op1, op2), word op1, word op2)
{
	return GSM_L_MULT(op1, op2);
}

longword M_gsm_L_add P2((op1, op2), longword op1, longword op2)
{
	ulongword utmp;
__boundcheck_metadata_store((void *)(&utmp),(void *)((size_t)(&utmp)+sizeof(utmp)*8-1));

	return GSM_L_ADD(op1, op2);
}
