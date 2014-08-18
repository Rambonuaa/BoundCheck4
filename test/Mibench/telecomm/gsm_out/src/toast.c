/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/src/toast.c,v 1.1.1.1 2000/11/06 19:54:26 mguthaus Exp $ */

#include	"toast.h"

/*  toast -- lossy sound compression using the gsm library.
 */

char   * progname;

int	f_decode   = 0;		/* decode rather than encode	 (-d) */
int 	f_cat	   = 0;		/* write to stdout, not foo.gsm	 (-c) */
int	f_force	   = 0;		/* force deletion 		 (-f) */
int	f_precious = 0;		/* avoid deletion		 (-p) */
int	f_fast	   = 0;		/* use faster fpt algorithm	 (-F) */
int	f_verbose  = 0;		/* debugging			 (-V) */

struct stat instat;		/* stat (inname) 		 */

FILE	*in, 	 *out;
char	*inname, *outname;

/*
 *  The function (*output)() writes a frame of 160 samples given as
 *  160 signed 16 bit values (gsm_signals) to <out>.
 *  The function (*input)() reads one such frame from <in>.
 *  The function (*init_output)() begins output (e.g. writes a header).,
 *  The function (*init_input)() begins input (e.g. skips a header).
 *
 *  There are different versions of input, output, init_input and init_output
 *  for different formats understood by toast; which ones are used 
 *  depends on the command line arguments and, in their absence, the
 *  filename; the fallback is #defined in toast.h
 *
 *  The specific implementations of input, output, init_input and init_output
 *  for a format `foo' live in toast_foo.c.
 */

int	(*output   ) P((gsm_signal *)),
	(*input    ) P((gsm_signal *));
int	(*init_input)  P((void)),
	(*init_output) P((void));

static int	generic_init P0() { return 0; }	/* NOP */

struct fmtdesc {

	char * name, * longname, * suffix;

	int  (* init_input )  P((void)),
	     (* init_output)  P((void));

	int  (* input ) P((gsm_signal * )),
	     (* output) P((gsm_signal * ));

} f_audio = {
		"audio",
		"8 kHz, 8 bit u-law encoding with Sun audio header", ".au",
		audio_init_input,
		audio_init_output,
		ulaw_input,
		ulaw_output
}, f_ulaw = {
		"u-law", "plain 8 kHz, 8 bit u-law encoding", ".u",
		generic_init,
		generic_init,
		ulaw_input,
		ulaw_output 

}, f_alaw = {
		"A-law", "8 kHz, 8 bit A-law encoding", ".A",
		generic_init,
		generic_init,
		alaw_input,
		alaw_output

}, f_linear = {
		"linear",
		"16 bit (13 significant) signed 8 kHz signal", ".l",
		generic_init,
		generic_init,
		linear_input,
		linear_output
};

struct fmtdesc * alldescs[] = {
	&f_audio,
	&f_alaw,
	&f_ulaw,
	&f_linear,
	(struct fmtdesc *)NULL
};

#define	DEFAULT_FORMAT	f_ulaw		/* default audio format, others	*/
					/* are: f_alaw,f_audio,f_linear */
struct fmtdesc * f_format  = 0;

/*
 *  basename + suffix of a pathname
 */
static char * endname P1((name), char * name)
{
	if (name) {
		char * s = strrchr(name, '/');
__boundcheck_metadata_store((void *)(&s),(void *)((size_t)(&s)+sizeof(s)*8-1));

		if (s && s[__boundcheck_ptr_cast_to_array_reference(110,14,"endname",(void *)(s),(void *)(s+1),1)]) name = s + 1;
__boundcheck_metadata_trans_check((void *)(name),(void *)(s),(void *)(s + 1));

	}
	return name;

}

/*
 *  Try to figure out what we're supposed to do from the argv[0], if
 *  any, and set the parameters accordingly.
 */
static void parse_argv0 P1((av0), char * av0 )
{
	int 	l;
__boundcheck_metadata_store((void *)(&l),(void *)((size_t)(&l)+sizeof(l)*8-1));


	progname = av0 = endname(av0 ? av0 : "toast");
__boundcheck_metadata_trans_check((void *)(progname),(void *)(av0),(void *)(av0 = endname(av0 ? av0 : "toast")));


	/*  If the name starts with `un', we want to decode, not code.
	 *  If the name ends in `cat', we want to write to stdout,
	 *  and decode as well.
	 */

	if (!strncmp(av0, "un", 2)) f_decode = 1;
	if (  (l = strlen(av0)) >= 3 /* strlen("cat") */
	   && !strcmp( av0 + l - 3, "cat" )) f_cat = f_decode = 1;
}


/*
 *  Check whether the name (possibly generated by appending
 *  .gsm to something else) is short enough for this system.
 */
static int length_okay P1((name), char * name)
{
	long	max_filename_length = 0;
__boundcheck_metadata_store((void *)(&max_filename_length),(void *)((size_t)(&max_filename_length)+sizeof(max_filename_length)*8-1));

	char	* end;
__boundcheck_metadata_store((void *)(&end),(void *)((size_t)(&end)+sizeof(end)*8-1));


	/* If our _pathname_ is too long, we'll usually not be
	 * able to open the file at all -- don't worry about that.
	 * 
	 * But if the _filename_ is too long, there is danger of
	 * silent truncation on some systems, which results
	 * in the target replacing the source!
	 */

	if (!name) return 0;
	end = endname(name);

#ifdef	NAME_MAX
	max_filename_length  = NAME_MAX;
#else
#ifdef	_PC_NAME_MAX
#ifdef USE_PATHCONF
	{	char * s, tmp; 
		
		/*  s = dirname(name)
		 */
		if ((s = end) > name) {
			if (s > name + 1) s--;
			tmp = s;
			*s  = 0;
		}

		errno = 0;
		max_filename_length = pathconf(s > name ? name : ".",
			_PC_NAME_MAX);
		if (max_filename_length == -1 && errno) {
			perror( s > name ? name : "." );
			fprintf(stderr,
		"%s: cannot get dynamic filename length limit for %s.\n",
				progname, s > name ? name : ".");
			return 0;
		}
		if (s > name) *s = tmp;
	}
#endif /* USE_PATHCONF  */
#endif /* _PC_NAME_MAX  */
#endif /* !NAME_MAX 	*/

	if (max_filename_length > 0 && strlen(end) > max_filename_length) {
		fprintf(stderr,
			"%s: filename \"%s\" is too long (maximum is %ld)\n",
			progname, endname(name), max_filename_length );
		return 0;
	}

	return 1;
}

/*
 *  Return a pointer the suffix of a string, if any.
 *  A suffix alone has no suffix, an empty suffix can not be had.
 */
static char * suffix P2((name, suf), char *name, char * suf) 
{
	size_t nlen = strlen(name);
__boundcheck_metadata_store((void *)(&nlen),(void *)((size_t)(&nlen)+sizeof(nlen)*8-1));

	size_t slen = strlen(suf);
__boundcheck_metadata_store((void *)(&slen),(void *)((size_t)(&slen)+sizeof(slen)*8-1));


	if (!slen || nlen <= slen) return (char *)0;
	name += nlen - slen;
	return memcmp(name, suf, slen) ? (char *)0 : name;
}


static void catch_signals P1((fun), SIGHANDLER_T (*fun) ()) 
{
#ifdef	SIGHUP
	signal( SIGHUP,   fun );
#endif
#ifdef	SIGINT
	signal( SIGINT,   fun );
#endif
#ifdef	SIGPIPE
	signal( SIGPIPE,  fun );
#endif
#ifdef	SIGTERM
	signal( SIGTERM,  fun );
#endif
#ifdef	SIGXFSZ
	signal( SIGXFSZ,  fun );
#endif
}

static SIGHANDLER_T onintr P0()
{
	char * tmp = outname;
__boundcheck_metadata_store((void *)(&tmp),(void *)((size_t)(&tmp)+sizeof(tmp)*8-1));


#ifdef	HAS_SYSV_SIGNALS
	catch_signals( SIG_IGN );
#endif

	outname = (char *)0;
	if (tmp) (void)unlink(tmp);

	exit(1);
}

/*
 *  Allocate some memory and complain if it fails.
 */
static char * emalloc P1((len), size_t len)
{
	char * s;
__boundcheck_metadata_store((void *)(&s),(void *)((size_t)(&s)+sizeof(s)*8-1));

	if (!(s = __boundcheck_malloc(len))) {
		fprintf(stderr, "%s: failed to malloc %d bytes -- abort\n",
			progname, len);
		onintr();
		exit(1);
	}
	return s;
}

static char* normalname P3((name, want, cut), char *name, char *want,char *cut)
{
	size_t	maxlen;
__boundcheck_metadata_store((void *)(&maxlen),(void *)((size_t)(&maxlen)+sizeof(maxlen)*8-1));

	char 	* s;
__boundcheck_metadata_store((void *)(&s),(void *)((size_t)(&s)+sizeof(s)*8-1));
 *char * p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));


	p = (char *)0;
	if (!name) return p;

	maxlen = strlen(name) + 1 + strlen(want) + strlen(cut);
	p = __boundcheck_strcpy(emalloc(maxlen), name);

	if (s = suffix(p, cut)) __boundcheck_strcpy(s, want);
	else if (*(char *)(__boundcheck_ptr_reference(273,12,"normalname",(void *)(want),(void *)(want))) && !suffix(p, want)) __boundcheck_strcat(p, want);

	return p;
}

/*
 *  Generate a `plain' (non-encoded) name from a given name.
 */
static char * plainname P1((name), char *name)
{
	return normalname(name, "", SUFFIX_TOASTED );
}

/*
 *  Generate a `code' name (foo.Z) from a given name.
 */
static char * codename P1((name), char *name)
{
	return normalname( name, SUFFIX_TOASTED, "" );
}

/*
 *  If we're supposed to ask (fileno (stderr) is a tty, and f_force not
 *  set), ask the user whether to overwrite a file or not.
 */
static int ok_to_replace P1(( name ), char * name)
{
	int reply;
__boundcheck_metadata_store((void *)(&reply),(void *)((size_t)(&reply)+sizeof(reply)*8-1));
int c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));


	if (f_force) return 1;			/* YES, do replace   */
	if (!isatty(fileno(stderr))) return 0;	/* NO, don't replace */

	fprintf(stderr,
		"%s already exists; do you wish to overwrite %s (y or n)? ",
		name, name);
	fflush(stderr);

	for (c = reply = getchar(); c != '\n' && c != EOF; c = getchar()) ;
	if (reply == 'y') return 1;

	fprintf(stderr, "\tnot overwritten\n");
	return 0;
}

static void update_mode P0()
{
	if (!instat.st_nlink) return;		/* couldn't stat in */

#ifdef HAS_FCHMOD
	if (fchmod(fileno(out), instat.st_mode & 07777)) {
		perror(outname);
		fprintf(stderr, "%s: could not change file mode of \"%s\"\n",
			progname, outname);
	}
#else 
	if (outname && chmod(outname, instat.st_mode & 07777)) {
		perror(outname);
		fprintf(stderr, "%s: could not change file mode of \"%s\"\n",
			progname, outname);
	}
#endif /* HAS_FCHMOD */
}

static void update_own P0()
{
	if (!instat.st_nlink) return; /* couldn't stat in */
#ifdef HAS_FCHOWN
	(void)fchown(fileno(out), instat.st_uid, instat.st_gid);
#else 
	(void)chown(outname, instat.st_uid, instat.st_gid);
#endif /* HAS_FCHOWN */
}

static void update_times P0()
{
	if (!instat.st_nlink) return; 	/* couldn't stat in */

#ifdef HAS_UTIMES
	if (outname) {
		struct timeval tv[2];

		tv[0].tv_sec  = instat.st_atime;
		tv[1].tv_sec  = instat.st_mtime;
		tv[0].tv_usec = tv[1].tv_usec = 0;
		(void) utimes(outname, tv);
	}
#else
#ifdef HAS_UTIME

	if (outname) {

#ifdef	HAS_UTIMBUF
		struct utimbuf ut;

		ut.actime     = instat.st_atime;
		ut.modtime    = instat.st_mtime;

#	ifdef	HAS_UTIMEUSEC
		ut.acusec     = instat.st_ausec;
		ut.modusec    = instat.st_musec;
#	endif 	/* HAS_UTIMEUSEC */

		(void) utime(outname, &ut);

#else /* UTIMBUF */

		time_t ut[2];
__boundcheck_metadata_store((void *)(&ut),(void *)((size_t)(&ut)+sizeof(ut)*8-1));


		ut[_RV_insert_check(0,2,381,6,"update_times",0)] = instat.st_atime;
		ut[_RV_insert_check(0,2,382,6,"update_times",1)] = instat.st_mtime;

		(void) utime(outname, ut);

#endif	/* UTIMBUF */
	}
#endif /* HAS_UTIME */
#endif /* HAS_UTIMES */
}


static int okay_as_input P3((name,f,st), char* name, FILE* f, struct stat * st)
{
# ifdef	HAS_FSTAT
	if (fstat(fileno(f), st) < 0)
# else
	if (stat(name, st) < 0)
# endif
	{
		perror(name);
		fprintf(stderr, "%s: cannot stat \"%s\"\n", progname, name);
		return 0;
	}

	if (!S_ISREG(st->st_mode)) {
		fprintf(stderr,
			"%s: \"%s\" is not a regular file -- unchanged.\n",
			progname, name);
		return 0;
	}
	if (st->st_nlink > 1 && !f_cat && !f_precious) {
		fprintf(stderr, 
		      "%s: \"%s\" has %s other link%s -- unchanged.\n",
			progname,name,st->st_nlink - 1,"s" + (st->st_nlink<=2));
		return 0;
	}
	return 1;
}

static void prepare_io P1(( desc), struct fmtdesc * desc)
{
	output      = desc->output;
	input       = desc->input;

	init_input  = desc->init_input;
	init_output = desc->init_output;
}

static struct fmtdesc * grok_format P1((name), char * name)
{
	char * c;
__boundcheck_metadata_store((void *)(&c),(void *)((size_t)(&c)+sizeof(c)*8-1));

	struct fmtdesc ** f;
__boundcheck_metadata_store((void *)(&f),(void *)((size_t)(&f)+sizeof(f)*8-1));


	if (name) {
		c = plainname(name);

		for (f = alldescs; *(struct fmtdesc **)(__boundcheck_ptr_reference(438,23,"grok_format",(void *)(f),(void *)(f))); f++)
			if (  (*(struct fmtdesc **)(__boundcheck_ptr_reference(439,12,"grok_format",(void *)(f),(void *)(f))))->suffix
			   && *(char *)(__boundcheck_ptr_reference(440,11,"grok_format",(void *)((*f)->suffix),(void *)(*(struct fmtdesc **)(__boundcheck_ptr_reference(440,13,"grok_format",(void *)(f),(void *)(f))))->suffix))
			   && suffix(c, (*(struct fmtdesc **)(__boundcheck_ptr_reference(441,22,"grok_format",(void *)(f),(void *)(f))))->suffix)) {

				__boundcheck_free(c);
				return *(struct fmtdesc **)(__boundcheck_ptr_reference(444,13,"grok_format",(void *)(f),(void *)(f)));
			}

		__boundcheck_free(c);
	}
	return (struct fmtdesc *)0;
}

static int open_input P2((name, st), char * name, struct stat * st)
{
	struct fmtdesc * f = f_format;
__boundcheck_metadata_store((void *)(&f),(void *)((size_t)(&f)+sizeof(f)*8-1));


	st->st_nlink = 0;	/* indicates `undefined' value */
	if (!name) {
		inname = (char *)NULL;
		in     = stdin;
__boundcheck_metadata_trans_check((void *)(in),(void *)(stdin),(void *)(stdin));

	}
	else {
		if (f_decode) inname = codename(name);
		else {
			if (!f_cat && suffix(name, SUFFIX_TOASTED)) {
				fprintf(stderr,
			"%s: %s already has \"%s\" suffix -- unchanged.\n",
					progname, name, SUFFIX_TOASTED );
				return 0;
			}
			inname = __boundcheck_strcpy(emalloc(strlen(name)+1), name);
		}
		if (!(in = fopen(inname, READ))) {
			perror(inname);	/* not guaranteed to be valid here */
			fprintf(stderr, "%s: cannot open \"%s\" for reading\n",
				progname, inname);
			return 0;
		}
		if (!okay_as_input(inname, in, st)) return 0;
		if (!f) f = grok_format(inname);
	}
	prepare_io( f ? f : & DEFAULT_FORMAT );
	return 1;
}

static int open_output P1((name), char *name)
{
	if (!name || f_cat) {
		out     = stdout;
__boundcheck_metadata_trans_check((void *)(out),(void *)(stdout),(void *)(stdout));

		outname = (char *)NULL;
	}
	else {
		int outfd = -1;
__boundcheck_metadata_store((void *)(&outfd),(void *)((size_t)(&outfd)+sizeof(outfd)*8-1));

		char * o;
__boundcheck_metadata_store((void *)(&o),(void *)((size_t)(&o)+sizeof(o)*8-1));


		o = (*(char *(*)(char *))(__boundcheck_ptr_reference(495,9,"open_output",(void *)(f_decode ? plainname : codename),(void *)(f_decode ? plainname : codename))))(name);
		if (!length_okay(o)) return 0;
		if ((outfd = open(o, O_WRITE_EXCL, 0666)) >= 0)
			out = fdopen(outfd, WRITE);
		else if (errno != EEXIST) out = (FILE *)NULL;
		else if (ok_to_replace(o)) out = fopen(o, WRITE);
		else return 0;

		if (!out) {
			perror(o);
			fprintf(stderr,
				"%s: can't open \"%s\" for writing\n",
				progname, o);
			if (outfd >= 0) (void)close(outfd);
			return 0;
		}

		outname = o;
__boundcheck_metadata_trans_check((void *)(outname),(void *)(o),(void *)(o));

	}
	return 1;
}

static int process_encode P0()
{
	gsm      	r;
__boundcheck_metadata_store((void *)(&r),(void *)((size_t)(&r)+sizeof(r)*8-1));

	gsm_signal    	s[ 160 ];
__boundcheck_metadata_store((void *)(&s),(void *)((size_t)(&s)+sizeof(s)*8-1));

	gsm_frame	d;
__boundcheck_metadata_store((void *)(&d),(void *)((size_t)(&d)+sizeof(d)*8-1));

 
	int		cc;
__boundcheck_metadata_store((void *)(&cc),(void *)((size_t)(&cc)+sizeof(cc)*8-1));


	if (!(r = gsm_create())) {
		perror(progname);
		return -1;
	}
	(void)gsm_option(r, GSM_OPT_FAST,    &f_fast);
	(void)gsm_option(r, GSM_OPT_VERBOSE, &f_verbose);

	while ((cc = (*(int (*)(gsm_signal *))(__boundcheck_ptr_reference(532,17,"process_encode",(void *)(input),(void *)(input))))(s)) > 0) {
		if (cc < sizeof(s) / sizeof(*(gsm_signal [160])(__boundcheck_ptr_reference(533,32,"process_encode",(void *)(s),(void *)(s)))))
			memset((char *)(s+cc), 0, sizeof(s)-(cc * sizeof(*(gsm_signal [160])(__boundcheck_ptr_reference(534,54,"process_encode",(void *)(s),(void *)(s))))));
		gsm_encode(r, s, d);
		if (fwrite((char *)d, sizeof(d), 1, out) != 1) {
			perror(outname ? outname : "stdout");
			fprintf(stderr, "%s: error writing to %s\n",
				progname, outname ? outname : "stdout");
			gsm_destroy(r);
			return -1;
		}
	}
	if (cc < 0) {
		perror(inname ? inname : "stdin");
		fprintf(stderr, "%s: error reading from %s\n",
			progname, inname ? inname : "stdin");
		gsm_destroy(r);
		return -1;
	}
	gsm_destroy(r);

	return 0;
}

static int process_decode P0()
{
	gsm      	r;
__boundcheck_metadata_store((void *)(&r),(void *)((size_t)(&r)+sizeof(r)*8-1));

	gsm_frame	s;
__boundcheck_metadata_store((void *)(&s),(void *)((size_t)(&s)+sizeof(s)*8-1));

	gsm_signal	d[ 160 ];
__boundcheck_metadata_store((void *)(&d),(void *)((size_t)(&d)+sizeof(d)*8-1));

 
	int		cc;
__boundcheck_metadata_store((void *)(&cc),(void *)((size_t)(&cc)+sizeof(cc)*8-1));


	if (!(r = gsm_create())) {	/* malloc failed */
		perror(progname);
		return -1;
	}
	(void)gsm_option(r, GSM_OPT_FAST,    &f_fast);
	(void)gsm_option(r, GSM_OPT_VERBOSE, &f_verbose);

	while ((cc = fread(s, 1, sizeof(s), in)) > 0) {

		if (cc != sizeof(s)) {
			if (cc >= 0) fprintf(stderr,
			"%s: incomplete frame (%d byte%s missing) from %s\n",
					progname, sizeof(s) - cc,
					"s" + (sizeof(s) - cc == 1),
					inname ? inname : "stdin" );
			gsm_destroy(r);
			errno = 0;
			return -1;
		}
		if (gsm_decode(r, s, d)) {
			fprintf(stderr, "%s: bad frame in %s\n", 
				progname, inname ? inname : "stdin");
			gsm_destroy(r);
			errno = 0;
			return -1;
		}

		if ((*(int (*)(gsm_signal *))(__boundcheck_ptr_reference(591,9,"process_decode",(void *)(output),(void *)(output))))(d) < 0) {
			perror(outname);
			fprintf(stderr, "%s: error writing to %s\n",
					progname, outname);
			gsm_destroy(r);
			return -1;
		}
	}

	if (cc < 0) {
		perror(inname ? inname : "stdin" );
		fprintf(stderr, "%s: error reading from %s\n", progname,
			inname ? inname : "stdin");
		gsm_destroy(r);
		return -1;
	}

	gsm_destroy(r);
	return 0;
}

static int process P1((name), char * name)
{
	int step = 0;
__boundcheck_metadata_store((void *)(&step),(void *)((size_t)(&step)+sizeof(step)*8-1));


	out     = (FILE *)0;
	in      = (FILE *)0;

	outname = (char *)0;
	inname  = (char *)0;

	if (!open_input(name, &instat) || !open_output(name))
		goto err;

	if ((*(int (*)(void))(__boundcheck_ptr_reference(625,8,"process",(void *)(f_decode ? init_output : init_input),(void *)(f_decode ? init_output    : init_input))))()) {
		fprintf(stderr, "%s: error %s %s\n",
			progname,
			f_decode ? "writing header to" : "reading header from",
			f_decode ? (outname ? outname : "stdout")
				 : (inname ? inname : "stdin"));
		goto err;
	}

	if ((*(int (*)(void))(__boundcheck_ptr_reference(634,8,"process",(void *)(f_decode ? process_decode : process_encode),(void *)(f_decode ? process_decode : process_encode))))())
		goto err;

	if (fflush(out) < 0 || ferror(out)) {
		perror(outname ? outname : "stdout");
		fprintf(stderr, "%s: error writing \"%s\"\n", progname,
				outname ? outname:"stdout");
		goto err;
	}

	if (out != stdout) {

		update_times();
		update_mode ();
		update_own  ();

		if (fclose(out) < 0) {
			perror(outname);
			fprintf(stderr, "%s: error writing \"%s\"\n",
				progname, outname);
			goto err;
		}
		if (outname != name) __boundcheck_free(outname);
		outname = (char *)0;
	}
	out = (FILE *)0;
	if (in  != stdin) {
		(void)fclose(in), in = (FILE *)0;
		if (!f_cat && !f_precious) {
			if (unlink(inname) < 0) {
				perror(inname);
				fprintf(stderr,
					"%s: source \"%s\" not deleted.\n",
					progname, inname);
			}
			goto err;
		}
		if (inname != name) __boundcheck_free(inname);
		inname = (char *)0;
	}
	return 0;

	/*
	 *  Error handling and cleanup: 
	 *  - error out: close out, unlink it, close in, free the names.
	 *  - 
	 */
err:
	if (out && out != stdout) {
		(void)fclose(out), out = (FILE *)0;
		if (unlink(outname) < 0 && errno != ENOENT && errno != EINTR) {
			perror(outname);
			fprintf(stderr, "%s: could not unlink \"%s\"\n",
				progname, outname);
		}
	}
	if (in && in != stdin) (void)fclose(in), in = (FILE *)0;

	if (inname  && inname  != name) __boundcheck_free(inname);
	if (outname && outname != name) __boundcheck_free(outname);

	return -1;
}

static void version P0()
{
	printf( "%s 1.0, version %s\n",
		progname,
		"$Id: toast.c,v 1.1.1.1 2000/11/06 19:54:26 mguthaus Exp $" );
}

static void help P0()
{
	printf("Usage: %s [-fcpdhvaulsF] [files...]\n", progname);
	printf("\n");

	printf(" -f  force     Replace existing files without asking\n");
	printf(" -c  cat       Write to stdout, do not remove source files\n");
	printf(" -d  decode    Decode data (default is encode)\n");
	printf(" -p  precious  Do not delete the source\n");
	printf("\n");

	printf(" -u  u-law     Force 8 kHz/8 bit u-law in/output format\n");
	printf(" -s  sun .au   Force Sun .au u-law in/output format\n");
	printf(" -a  A-law     Force 8 kHz/8 bit A-law in/output format\n");
	printf(" -l  linear    Force 16 bit linear in/output format\n");
	printf("\n");

	printf(" -F  fast      Sacrifice conformance to performance\n");
	printf(" -v  version   Show version information\n");
	printf(" -h  help      Print this text\n");
	printf("\n");
}


static void set_format P1((f), struct fmtdesc * f)
{
	if (f_format && f_format != f) {
		fprintf( stderr,
	"%s: only one of -[uals] is possible (%s -h for help)\n",
			progname, progname);
		exit(1);
	}

	f_format = f;
__boundcheck_metadata_trans_check((void *)(f_format),(void *)(f),(void *)(f));

}

void __global_variables_init(){
__boundcheck_metadata_store((void *)(&progname),(void *)((size_t)(&progname)+sizeof(progname)*8-1));
__boundcheck_metadata_store((void *)(&f_decode),(void *)((size_t)(&f_decode)+sizeof(f_decode)*8-1));
__boundcheck_metadata_store((void *)(&f_cat),(void *)((size_t)(&f_cat)+sizeof(f_cat)*8-1));
__boundcheck_metadata_store((void *)(&f_force),(void *)((size_t)(&f_force)+sizeof(f_force)*8-1));
__boundcheck_metadata_store((void *)(&f_precious),(void *)((size_t)(&f_precious)+sizeof(f_precious)*8-1));
__boundcheck_metadata_store((void *)(&f_fast),(void *)((size_t)(&f_fast)+sizeof(f_fast)*8-1));
__boundcheck_metadata_store((void *)(&f_verbose),(void *)((size_t)(&f_verbose)+sizeof(f_verbose)*8-1));
__boundcheck_metadata_store((void *)(&(instat)),(void *)((size_t)(&(instat))+sizeof(instat)*8-1));
__boundcheck_metadata_store((void *)(&(in)),(void *)((size_t)(&(in))+sizeof(in)*8-1));
__boundcheck_metadata_store((void *)(&(out)),(void *)((size_t)(&(out))+sizeof(out)*8-1));
__boundcheck_metadata_store((void *)(&inname),(void *)((size_t)(&inname)+sizeof(inname)*8-1));
__boundcheck_metadata_store((void *)(&outname),(void *)((size_t)(&outname)+sizeof(outname)*8-1));
__boundcheck_metadata_store((void *)(&output),(void *)((size_t)(&output)+sizeof(output)*8-1));
__boundcheck_metadata_store((void *)(&input),(void *)((size_t)(&input)+sizeof(input)*8-1));
__boundcheck_metadata_store((void *)(&init_input),(void *)((size_t)(&init_input)+sizeof(init_input)*8-1));
__boundcheck_metadata_store((void *)(&init_output),(void *)((size_t)(&init_output)+sizeof(init_output)*8-1));
__boundcheck_metadata_store((void *)(&(f_audio)),(void *)((size_t)(&(f_audio))+sizeof(f_audio)*8-1));
__boundcheck_metadata_store((void *)(&(f_ulaw)),(void *)((size_t)(&(f_ulaw))+sizeof(f_ulaw)*8-1));
__boundcheck_metadata_store((void *)(&(f_alaw)),(void *)((size_t)(&(f_alaw))+sizeof(f_alaw)*8-1));
__boundcheck_metadata_store((void *)(&(f_linear)),(void *)((size_t)(&(f_linear))+sizeof(f_linear)*8-1));
__boundcheck_metadata_store((void *)(&alldescs),(void *)((size_t)(&alldescs)+sizeof(alldescs)*8-1));
__boundcheck_metadata_store((void *)(&(f_format)),(void *)((size_t)(&(f_format))+sizeof(f_format)*8-1));
}
int main P2((ac, av), int ac, char **av)
{
	__global_variables_init();

	int  		opt;
__boundcheck_metadata_store((void *)(&opt),(void *)((size_t)(&opt)+sizeof(opt)*8-1));

	extern int	optind;

	parse_argv0( *(char **)(__boundcheck_ptr_reference(746,16,"main",(void *)(av),(void *)(av))) );

	while ((opt = getopt(ac, av, "fcdpvhuaslVF")) != EOF)
	switch (opt) {

	case 'd': f_decode   = 1; break;
	case 'f': f_force    = 1; break;
	case 'c': f_cat      = 1; break;
	case 'p': f_precious = 1; break;
	case 'F': f_fast     = 1; break;

#ifndef	NDEBUG
	case 'V': f_verbose  = 1; break;	/* undocumented */
#endif

	case 'u': set_format( &f_ulaw   ); break;
	case 'l': set_format( &f_linear ); break;
	case 'a': set_format( &f_alaw	); break;
	case 's': set_format( &f_audio  ); break;

	case 'v': version(); exit(0);
	case 'h': help();    exit(0);

	default: 
		fprintf(stderr,
			"Usage: %s [-fcpdhvuaslF] [files...] (-h for help)\n",
			progname);
		exit(1);
	}

	f_precious |= f_cat;

	av += optind;
	ac -= optind;

	catch_signals(onintr);

	if (ac <= 0) process( (char *)0 );
	else while (ac--) process( *(char **)(__boundcheck_ptr_reference(784,30,"main",(void *)(av),(void *)(av++))) );

	exit(0);
}
