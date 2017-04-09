#ifndef _EVAL_H
#define ASSERTP(EXPR,F) \
	do { if(EXPR) { \
	fprintf(stderr,"; " #F ": " #EXPR ".\n");   \
	longjmp(je,1); } \
	} while(0)

#define eval(p) (                                       \
	(p->type) == OBJ_T ? t :                            \
	(p->type) == OBJ_NIL ? nil :                        \
	(p->type) == OBJ_NULL ? handsig("UNDEFINED",null) :	\
	(p->type) == OBJ_INTEGER ? (p) :                    \
	(p->type) == OBJ_RATIONAL ? eval_rat(p) :                   \
	(p->type) == OBJ_IDENTIFIER ?  get_object(p) :      \
	(p->type) == OBJ_CONS ? eval_cons(p) : (p))

/* TRY_EVAL is identical to eval except it allows null objects to
 * be evaluated.
 * It is needed in order to eval objects which might not have a value
 * such as functions arguments
 */
#define try_eval(p) (                                           \
	(p->type) == OBJ_T ? t :                                    \
	(p->type) == OBJ_NIL ? nil :                                \
	(p->type) == OBJ_NULL ? null :                              \
	(p->type) == OBJ_INTEGER ? (p) :                            \
	(p->type) == OBJ_RATIONAL ? eval_rat(p) :                     		\
	(p->type) == OBJ_IDENTIFIER ?  try_object(p) :              \
	(p->type) == OBJ_CONS ? eval_cons(p) : (p))

#define car(p) (									\
		(p->type) == OBJ_CONS ? p->value.c.car :	\
		(p->type) == OBJ_NIL  ? nil	: null)

#define cdr(p) (									\
		(p->type) == OBJ_CONS ? p->value.c.cdr :	\
		(p->type) == OBJ_NIL  ? nil : null)

#define cddr(p) cdr(cdr(p))
#define cadr(p) car(cdr(p))
#define caar(p)	car(car(p))
#define cdar(p) cdr(car(p))

objectp eval_rat(objectp);
objectp eval_cons(objectp);

#define _EVAL_H
#endif