#ifndef _EXTERN_H
#include <setjmp.h>
extern bool		lazy_eval;
extern int 		getchl(void);
extern char 	*buffer;
extern int write_m(int);
extern jmp_buf 	jb;
extern jmp_buf 	je;
extern jmp_buf 	jl;
/* object.c */
extern objectp 	nil;
extern objectp 	t;
extern objectp 	null;

extern void 	remove_from_v(void);
extern void 	init_objects(void);
extern objectp 	new_object(a_type);
extern objectp 	search_object_identifier(char *);
extern objectp 	search_object_integer(long int);
extern objectp 	search_object_rational(long int, long int);

extern void 	set_object(objectp, objectp);
extern objectp 	get_object(objectp);
extern objectp 	try_object(objectp);
extern objectp 	handsig(const char *);
extern void 	garbage_collect(void);
extern void 	dump_objects(void);
/* parser.c */
extern objectp 	parse_object(int);
/* lexer.c */
extern FILE 	*input_file;
extern char 	*token_buffer;
extern void 	init_lex(void);
extern int 		gettoken(void);
/* funcs.c */
#define _EXTERN_H

#endif