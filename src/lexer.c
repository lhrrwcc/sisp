#include "config.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include "sisp.h"
#include "extern.h"

#define LEX_BUF_MAX 1024
#define xgetc() ((lex_bufp > lex_buf) ? *--lex_bufp : \
		toupper(fgetc(input_file)))
#define xungetc(c) *lex_bufp++ = c
#define CLEAN_BUFFER 	\
	do {				\
		xungetc(c);		\
		while(((c = xgetc()) == '\n') && c != EOF)	\
		;				\
		longjmp(jl,1);	\
	} while (0)

FILE 		*input_file;
static int 	lex_buf[LEX_BUF_MAX];
static int 	*lex_bufp;
char 		*token_buffer;
static int	token_buffer_max;
static int 	lpar = 0;
static int 	rpar = 0;
jmp_buf 	jl;

void
init_lex(void)
{
	token_buffer_max = 64;
	token_buffer = (char *)xmalloc(token_buffer_max);
	lex_bufp = lex_buf;
}

void
done_lex(void)
{
	free(token_buffer);
}

static char*
extend_buf(char *p)
{
	int off = p - token_buffer;
	token_buffer_max += 128;
	token_buffer = (char *)xrealloc(token_buffer, token_buffer_max);
	return token_buffer + off;
}

__inline__ int
gettoken(void)
{
	char *p;
	int c;
	for (;;) {
		c = xgetc();
		switch (c) {
		case ',':
			c = xgetc();
			if (c == ')')
				CLEAN_BUFFER;
			else {
				xungetc(c);
				c = ',';
				return c;
			}
		case '`':
			c = xgetc();
			if (c == ')')
				CLEAN_BUFFER;
			else {
				xungetc(c);
				c = '`';
				return c;
			}
		case '(':
			lpar++;
			return c;
		case ')':
			rpar++;
			if (rpar > lpar) {
				rpar = lpar = 0;
				CLEAN_BUFFER;
			}
			return ')';
		case ' ': 
			while (isspace(c = xgetc())) ;
			xungetc(c);
		case '\f': case '\t': case '\v': case '\r': case '\n':
			break;
		case ';':
			while ((c = xgetc()) != '\n');
			break;
		case '0': case '1': case '2': case '3':	case '4': case '5': 
		case '6': case '7': case '8': case '9': case '-':
			p = token_buffer;
			do {
				if (p - token_buffer >= token_buffer_max)
					p = extend_buf(p);
				*p++ = c;
				c = xgetc();
			} while (isdigit(c));
			if (c == ' ' || c == ')' || c == '(' || c == '\n') {
				xungetc(c);
				*p = '\0';
				return INTEGER;
			} else 
				CLEAN_BUFFER;
		case '_': case '+': case '.': 
		case '*': case '/': case '#': case '<': case '>': case '=': 
		case '&': case 'a': case 'b': case 'c': case 'd': case 'e': 
		case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': 
		case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': 
		case 'r': case 's': case 't': case 'u': case 'v': case 'w': 
		case 'x': case 'y': case 'z': case 'A': case 'B': case 'C': 
		case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': 
		case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': 
		case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': 
		case 'V': case 'W': case 'X': case 'Y': case 'Z':
			p = token_buffer;
			do {
				if (p - token_buffer >= token_buffer_max)
					p = extend_buf(p);
				*p++ = c;
				c = xgetc();
			} while (isalnum(c) || strchr("_-+*/#<>=&", c) != NULL);
			xungetc(c);
			*p = '\0';
			return IDENTIFIER;
		default:
			return c;
		}
	}
}