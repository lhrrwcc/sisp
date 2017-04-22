#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include "sisp.h"
#include "extern.h"
#include "eval.h"
#include "misc.h"
#include "libsisp.h"
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
		p = parse_object(0);
	    if(p != NULL) {
			q = eval(p);
		} else
			break;
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
			printf("; PARSER ERROR.\n");
		if (p != NULL && q != NULL)
		    princ_object(stdout,q);
		puts("");
		garbage_collect();
	}
	free(token_buffer);
}

static void 
process_file(char *filename)
{
	if (filename != NULL && strcmp(filename, "-") != 0) {
		if ((input_file = fopen(filename, "r")) == NULL)
			printf("%s", filename);
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
//	int fd;
//	char buildf[] = "/tmp/sisp.XXXXXXXX";
	init_objects();
 //	if ((fd = mkstemp(buildf)) > 0) {
	//	if (write_m(fd) != 0) {
	//		unlink(buildf);
			//warnx("cannot create file\n");
	//	}
	//	process_file(__builtin_functions);
	//	unlink(buildf);
//	} else
//		printf("cannot load functions\n");
	if(argc > 1)
		while(*++argv)  
			process_file(*argv);
	process_file(NULL);
	return 0;
}