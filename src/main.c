#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <err.h>
#include <setjmp.h>
#include <time.h>
#include "config.h"
#include "sisp.h"
#include "extern.h"
#include "version.h"
#include "eval.h"
#include "misc.h"

#define STACK_ROUTINE   getrlimit(RLIMIT_STACK,&l); \
						l.rlim_cur = l.rlim_max;    \
						setrlimit(RLIMIT_STACK,&l);

#define DATA_ROUTINE    getrlimit(RLIMIT_DATA,&l);  \
						l.rlim_cur = l.rlim_max;    \
						setrlimit(RLIMIT_DATA,&l);

#define RSS_ROUTINE     getrlimit(RLIMIT_RSS,&l);   \
						l.rlim_cur = l.rlim_max;    \
						setrlimit(RLIMIT_RSS,&l);
#define GET(p) 	((!setjmp(je)) ? eval(p) : NULL)
#define TRY 	((!setjmp(jb)) ? parse_object(0) : NULL)

jmp_buf jb, je;

static void 
pf(void)
{
	objectp p, q;
	p = q = NULL;
	init_lex();
	while(1) {
		p = TRY;
	    if(p != NULL) { 
			q = GET(p);
//			princ_object(stdout,p);
//			printf("\n");
		}
		else
			break;
	    remove_from_v();
		garbage_collect();
	}
	free(token_buffer);
}

static void 
process_input(void)
{
	objectp p, q;
	p = q = NULL;
	init_lex();
	while (1) {
		printf(": ");
		fflush(stdout);
		p = TRY;
		if (p != NULL) 
		    q = GET(p);
		else
			printf(";; PARSER ERROR.\n");
		if (p != NULL && q != NULL)
		    princ_object(stdout,q);
		puts("");
		remove_from_v();
		garbage_collect();
	}
	free(token_buffer);
}

static void 
process_file(char *filename)
{
	if (filename != NULL && strcmp(filename, "-") != 0) {
		if ((input_file = fopen(filename, "r")) == NULL)
			err(1, "%s", filename);
	} else
		input_file = stdin;
	if(input_file != stdin) 
		pf();
	else
		process_input();
}

int 
main(int argc, char **argv)
{
	int fd;
	char buildf[] = "/tmp/sisp.XXXXXXXX";
	struct rlimit l;
	init_objects();
/* VER SI ES CONVENIENTE USAR ESTO
 *	
 *	STACK_ROUTINE
 *	RSS_ROUTINE
 *	DATA_ROUTINE
 */
 	if ((fd = mkstemp(buildf)) > 0) {
		if (write_m(fd) != 0) {
			unlink(buildf);
			warnx("%s", write_m);
		}
		process_file(buildf);
		unlink(buildf);
	} else
		warnx("cannot load functions\n");

	if(argc > 1)
		while(*++argv)  
			process_file(*argv);
	process_file(NULL);
	return 0;
}
