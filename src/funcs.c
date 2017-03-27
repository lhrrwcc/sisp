#ifdef LINUX
#include <strings.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include "config.h"
#include "sisp.h"
#include "extern.h"
#include "funcs.h"
#include "eval.h"
#include "misc.h"

#define CONSP(p) ( 					\
		p->type == OBJ_CONS && 		\
		cdr(p)->type != OBJ_CONS && \
		cdr(p) != nil )
#define arg car(args)

bool lazy_eval = false;


__inline__ 
objectp bq(objectp args)
{
	objectp p1, r, first, prev;
	first = prev = NULL;
	do { 
		p1 = arg;
		r = new_object(OBJ_CONS);
		if (p1->type == OBJ_CONS)
			r->vcar = bq(p1);
		else if(p1->type == OBJ_IDENTIFIER && 
				!strcmp(p1->value.id, "COMMA")) { 
			r->vcar = eval(args);
			if (first == NULL)
				first = r;
			if (prev != NULL)
				prev->vcdr = r;
			prev = r;
			return car(first);
		} else 
			r->vcar = p1;
		if (first == NULL)
			first = r;
		if (prev != NULL)
			prev->vcdr = r;
		prev = r;
	} while ((args = cdr(args)) != nil);	
	return first;
}

static objectp 
F_less(objectp args)
{
    objectp a, b;
    a = eval(car(args));
    b = eval(cadr(args));
    ASSERTP(!(a->type == OBJ_INTEGER && b->type == OBJ_INTEGER), LESS);
	return (a->value.d < b->value.d) ? t : nil;
}

static objectp
F_lesseq(objectp args)
{
    objectp a, b;
    a = eval(car(args));
    b = eval(cadr(args));
    ASSERTP(!(a->type == OBJ_INTEGER && b->type == OBJ_INTEGER), LESS EQ);
	return (a->value.d <= b->value.d) ? t : nil;
}

static objectp
F_great(objectp args)
{
    objectp a, b;
    a = eval(car(args));
    b = eval(cadr(args));
    ASSERTP(!(a->type == OBJ_INTEGER && 
				b->type == OBJ_INTEGER), GREAT);
	return (a->value.d > b->value.d) ? t : nil;
}

static objectp
F_greateq(objectp args)
{
    objectp a, b;
    a = eval(car(args));
    b = eval(cadr(args));
    ASSERTP(!(a->type == OBJ_INTEGER && 
				b->type == OBJ_INTEGER), GREAT EQ); 
	return (a->value.d >= b->value.d) ? t : nil;
}

static objectp
F_add(objectp args)
{
    int i;
    objectp p;
    i = 0;
	do {
		p = eval(car(args)); 
		ASSERTP(p->type != OBJ_INTEGER, ADD);	
	    i+=p->value.d;
    } while ((args = cdr(args)) != nil);
	p = new_object(OBJ_INTEGER);
   	p->value.d = i;
	return p;
}

static objectp
F_prod(objectp args)
{
    int i;
    objectp p;
    i = 1;
    do {
		p = eval(car(args)); 
	    ASSERTP(p->type != OBJ_INTEGER, PROD);
		i = i*p->value.d;
    } while ((args = cdr(args)) != nil);
	p = new_object(OBJ_INTEGER);
    p->value.d = i;
    return p;
}

objectp 
F_atom(objectp args)
{
	objectp p;
	p = eval(car(args));
	switch (p->type) {
		case OBJ_T: 
		case OBJ_NIL: 
		case OBJ_IDENTIFIER:
		case OBJ_INTEGER:
			return t;
		case OBJ_CONS:
			return nil;
		default:
			return NULL;
	}
	ASSERTP(1, ATOM);
}

objectp 
F_cond(objectp args)
{
	objectp p2;
	do {
		p2 = eval(car(car(args)));
		ASSERTP(p2 == NULL, COND);
		if (p2 != nil) {
			if (cdar(args) != nil)
				return F_progn(cdar(args));
			return p2;
		}
	} while ((args = cdr(args)) != nil);
	return nil;
}

objectp
F_cons(objectp args)
{
	objectp p;
	p = new_object(OBJ_CONS);
	p->vcar = eval(car(args));
	p->vcdr = eval(cadr(args));
	return p;
}

objectp
F_ord(objectp args)
{
	objectp q, p;
	register int i;
	p = eval(car(args));
	i = 0;
	q = new_object(OBJ_INTEGER);	
	if(p == nil) {
		q->value.d = 0;
		return q;
	}
	if (!(cdr(p)->type == OBJ_CONS || cdr(p)->type == OBJ_NIL)) {
		q->value.d = 1;
		return q;
	}
	do {
		i++;
	} while ((p = cdr(p)) != nil);
	q->value.d = i;
	return q;    
}

__inline__ static
 objectp F_car(objectp args)
{
	return car(eval(car(args)));
}

__inline__ static
objectp F_cdr(objectp args)
{
	return cdr(eval(arg));
}

objectp 
F_if(objectp args)
{
	return eval(car(args)) != nil ? eval(cadr(args)) :
		F_progn(cddr(args));
}

objectp 
F_list(objectp args)
{
	objectp first = NULL, prev = NULL, p1;
	do {
		p1 = new_object(OBJ_CONS);
		p1->vcar = eval(car(args));
		ASSERTP(p1->vcar == NULL, LIST);
		if (first == NULL)
			first = p1;
		if (prev != NULL)
			prev->vcdr = p1;
		prev = p1;
	} while ((args = cdr(args)) != nil);
	return first;
}

objectp
F_map(objectp args)
{
	objectp p, p1, first, prev;
	first = prev = NULL;
	p1 = eval(cadr(args));
	ASSERTP(p1->type != OBJ_CONS, MAP);
	do {
		p = new_object(OBJ_CONS);
		p->vcar = new_object(OBJ_CONS);
		p->vcar->vcar =  car(args);
		p->vcar->vcdr = new_object(OBJ_CONS);
		p->vcar->vcdr->vcar = new_object(OBJ_CONS);
		p->vcar->vcdr->vcar->vcar =	new_object(OBJ_IDENTIFIER);
		p->vcar->vcdr->vcar->vcar->value.id = xstrdup("QUOTE");
 		p->vcar->vcdr->vcar->vcdr = new_object(OBJ_CONS);
		p->vcar->vcdr->vcar->vcdr->vcar = car(p1);
		p->vcar->vcdr->vcar->vcdr->vcdr = nil;
		if (first == NULL)
			first = p;
		if (prev != NULL)
			prev->vcdr = p;
		prev = p;
	} while ((p1 = cdr(p1)) != nil);
	p = first;
	ASSERTP(p == NULL, MAP);
	first = prev = NULL;
    do {
         p1 = new_object(OBJ_CONS);
         p1->vcar = eval(p->vcar);
         if(first == NULL) 
		 	first = p1;
         if(prev != NULL)  
		 	prev->vcdr = p1;
         prev = p1;
    } while((p = cdr(p)) != nil);
    return first;
}

objectp 
F_quit(objectp args)
{
	/* Do i have to free every object it was created? */
	
	kill(getppid(), SIGTERM);
	exit(0);
	return NULL;
}

objectp
F_quote(objectp args)
{
	return car(args);
}

objectp 
F_and(objectp args)
{
	objectp p1;
	do {
		p1 = eval(car(args));
		if (p1 == nil)
			return nil;
	} while ((args = cdr(args)) != nil);
	return p1;
}

objectp 
F_or(objectp args)
{
	objectp p1;
	do {
		p1 = eval(car(args));
		if (p1 != nil)
			return p1;
	} while ((args = cdr(args)) != nil);
	return nil;
}

objectp 
F_not(objectp args)
{
	return eval(car(args)) != nil ? nil : t;	
}

objectp 
F_unless(objectp args)
{
	if (eval(car(args)) == nil)
		return F_progn(cdr(args));
	return nil;
}

objectp
F_while(objectp args)
{
	objectp p;
	while(1) {
		p = eval(car(args));
		if (p == nil)
		    break;
		F_progn(cdr(args));
	}
	return nil;
}

objectp
F_assoc(objectp args)
{
	objectp var, val;
	objectp assoc;
	var = car(args);
	assoc = eval(car(cdr(args)));
	do {
		val = caar(assoc);
		if (val->type == OBJ_IDENTIFIER)
			if (!strcmp(val->value.id, var->value.id))
				return eval(car(cdr(car(assoc))));
	} while ((assoc = cdr(assoc)) != nil);
	return null;
}

__inline__ objectp
F_progn(objectp args)
{
	do {
		if (cdr(args) == nil)
			break;
			eval(car(args));
	} while ((args = cdr(args)) != nil);
	
	return try_eval(car(args));
}

objectp 
F_prog1(objectp args)
{
	objectp p1;
	p1 = eval(car(args));
	args = cdr(args);
	if (args == nil)
		return p1;
	do {
		eval(car(args));
	} while ((args = cdr(args)) != nil);
	return p1;
}

objectp 
F_prog2(objectp args)
{
	objectp p1;
	eval(car(args));
	args = cdr(args);
	if (args == nil)
	    return nil;
	p1 = eval(car(args));
	args = cdr(args);
	if (args == nil)
	    return p1;
	do {
		 eval(car(args));
	} while ((args = cdr(args)) != nil);
	return p1;
}

objectp
F_eqab(objectp a, objectp b)
{
    if (a->type != b->type)
		return nil;
    switch (a->type) {
		case OBJ_IDENTIFIER:
	    	return strcmp(a->value.id, b->value.id) == 0 ? t : nil;
		case OBJ_CONS:
			return eqcons(a, b);
		default:
	    	break;
    }
    ASSERTP(1, EQ A B);
	return (objectp) NULL;
}

objectp
F_eq(objectp args)
{
	objectp a, b;
	a = eval(car(args));
	b = eval(cadr(args));
	ASSERTP(a == NULL || b == NULL, EQ);
	if (a->type != b->type) 
		return nil;
	switch (a->type) {
		case OBJ_IDENTIFIER:
			return strcmp(a->value.id, b->value.id) == 0 ? t : nil; 
		case OBJ_CONS:
			return eqcons(a, b);
		case OBJ_INTEGER:
			return (a->value.d == b->value.d) ? t : nil;
		default:
			return t;
	}
	ASSERTP(1, EQ);
	return (objectp) NULL;
}

objectp
F_member(objectp args)
{
	objectp m, x, set;
	m = eval(car(args));
	set = eval(cadr(args));
	do {
		x = car(set);
		switch (m->type) {
		case OBJ_IDENTIFIER:
			if (x->type == OBJ_IDENTIFIER && 
					!strcmp(m->value.id, x->value.id)) 
				return t;
			break;
		case OBJ_CONS:
			if (x->type == OBJ_CONS && eqcons(m, x) == t)
				return t;
			break;
		case OBJ_T:
			if (x->type == OBJ_T)
				return t;
			break;
		case OBJ_NIL:
			if (x->type == OBJ_NIL)
				return t;
			break;
		case OBJ_INTEGER:
			if (x->type == OBJ_INTEGER) 
				if (x->value.d == m->value.d) 
					return t;
			break;
		default:
			break;
		}
		set = cdr(set);
	} while (set->type != OBJ_NIL);
	return nil;
}

objectp 
F_setq(objectp args)
{
	objectp p2;
	do {
		p2 = eval(cadr(args));
		ASSERTP(car(args)->type != OBJ_IDENTIFIER,SETQ); 
		set_object(car(args), p2);
	} while ((args = cddr(args)) != nil);
	return p2;
}

objectp 
F_listp(objectp args)
{
	return eval(car(args))->type == OBJ_CONS ? t : nil;
}

objectp 
F_consp(objectp args)
{
	objectp p;
	p = eval(car(args));
	if (p->type == OBJ_CONS && cdr(p)->type != OBJ_CONS && cdr(p) != nil)
		return t;
	return nil;
}

objectp
F_lambda(objectp args) 
{
    return cdr(args);
}

objectp 
F_pair(objectp args)
{
	objectp p, p1, p2, first = NULL, prev = NULL;
	p1 = eval(car(args));
	p2 = eval(cadr(args));
	ASSERTP(p1->type != OBJ_CONS || p2->type != OBJ_CONS || 
			CONSP(p1) || CONSP(p2),PAIR);
	do {
		p = new_object(OBJ_CONS);
		p->vcar = new_object(OBJ_CONS);
		p->vcar->vcar = car(p1);
		p->vcar->vcdr = new_object(OBJ_CONS);
		p->vcar->vcdr->vcar =car(p2);
		if (first == NULL)
			first = p;
		if (prev != NULL)
			prev->vcdr = p;
		prev = p;
	} while ((p1 = cdr(p1)) != nil && (p2 = cdr(p2)) != nil);
	return first;
}

objectp 
F_pair_neval(objectp args)
{
	objectp p, p1, p2, first = NULL, prev = NULL;
	p1 = car(args);
	p2 = cadr(args);
	ASSERTP(p1->type != OBJ_CONS || p2->type != OBJ_CONS ||
			CONSP(p1) || CONSP(p2),PAIR);
	do {
		p = new_object(OBJ_CONS);
		p->vcar = new_object(OBJ_CONS);
		p->vcar->vcar = car(p1);
		p->vcar->vcdr = new_object(OBJ_CONS);
		p->vcar->vcdr->vcar = car(p2);
		if (first == NULL)
			first = p;
		if (prev != NULL)
			prev->vcdr = p;
		prev = p;
	} while ((p1 = cdr(p1)) != nil && (p2 = cdr(p2)) != nil);
	return first;
}

objectp 
F_append(objectp args)
{
	objectp p, p1, first, prev;
	first = prev = NULL;
	do {
		p = eval(car(args));
		ASSERTP((p->type != OBJ_CONS || CONSP(p)), APPEND);
		do { 
			p1 = new_object(OBJ_CONS);
			p1->vcar = car(p);
			if (first == NULL)
				first = p1;
			if (prev != NULL)
				prev->vcdr = p1;
			prev = p1;
			p = cdr(p);
		} while (p != nil);
	} while((args = cdr(args)) !=nil);
	return first;
}

objectp
F_bquote(objectp args)
{
	return car(args)->type == OBJ_IDENTIFIER ? car(args) :
		car(bq(args));
}

objectp 
F_comma(objectp args)
{
	return eval(car(args));
}

objectp 
F_let(objectp args)
{
	objectp var, bind, bind_list, body, r, q, first, prev;
	first = prev = NULL;
	bind_list = car(args);
	if (bind_list->type != OBJ_CONS)
		return F_progn(cdr(args));
	body = cdr(args);
	do {
		bind = caar(bind_list);
		var = eval(cadr(car(bind_list)));
		r = new_object(OBJ_CONS);
		r->vcar = try_eval(bind);
		if (first == NULL)
			first = r;
		if (prev != NULL)
			prev->vcdr = r;
		prev = r;
		set_object(bind,var);
	} while ((bind_list = cdr(bind_list)) != nil);
	q = F_progn(body);
	bind_list = car(args);
	do {
		bind = caar(bind_list);
		var = car(first);
		set_object(bind,var);
		first = cdr(first);		
	} while ((bind_list = cdr(bind_list)) != nil);
	return q;
}
/*
 * (SUBST 'VAR RPL LIST)
 * (SUBSET '(('VAR_1 RPL1) ('VAR_2 RPL2) ...) LIST)
 */
objectp
F_subst(objectp args)
{
	objectp sym, val, body;
	sym = car(args);
	if (sym->type == OBJ_CONS) {
		objectp s;
		s = car(args);
		body = eval(cadr(args));
		ASSERTP(body->type != OBJ_CONS, SUBST);
		do {
			sym = caar(s);
			val = eval(cadr(car(s)));
			body = sst(sym, val, body);			
		} while ((s = cdr(s)) != nil);
		return body;
	} else {
		body = eval(car(cddr(args)));
		ASSERTP(body->type != OBJ_CONS, SUBST);
		val = eval(cadr(args));
		return sst(sym, val, body);
	}
	ASSERTP(1,SUBST);
	return (objectp) NULL;
}

objectp 
F_labels(objectp args)
{
	objectp var, bind, bind_list, body, r, q, s, first, prev;
	first = prev = NULL;
	bind_list = car(args);
	ASSERTP((bind_list->type!=OBJ_CONS || bind_list == nil), LABELS);
	body = cdr(args);
	ASSERTP((body->type != OBJ_CONS || body == nil), LABELS);
	do {
		bind = caar(bind_list);
		var = cdar(bind_list);
		s = new_object(OBJ_CONS);
		s->vcar = new_object(OBJ_IDENTIFIER);
		s->vcar->value.id = xstrdup("LAMBDA");
		s->vcdr = var; 
		r = new_object(OBJ_CONS);
		r->vcar = try_eval(bind);
		if(first == NULL)
			first = r;
		if(prev != NULL)
			prev->vcdr = r;
		prev = r;
		set_object(bind,s);
	} while ((bind_list = cdr(bind_list)) != nil);
	q = F_progn(body);
	bind_list = car(args);
	do {
		bind = caar(bind_list);
		var = car(first);
		set_object(bind,var);
		first = cdr(first);
	} while ((bind_list = cdr(bind_list)) != nil);
	return q;
}

objectp 
F_eval(objectp args)
{
//	objectp r;
	return eval(eval(car(args)));
//	ASSERTP(1, r == null);
//	return eval(r);	    
}

objectp 
F_defmacro(objectp args)
{
	objectp p1, p4, lexpr;
	p1 = car(args);
	p4 = new_object(OBJ_CONS);
	p4->vcar = new_object(OBJ_IDENTIFIER);
	p4->vcar->value.id = xstrdup("BACKQUOTE");
	p4->vcdr = cddr(args);
	lexpr = new_object(OBJ_CONS);
	lexpr->vcar = new_object(OBJ_IDENTIFIER);
	lexpr->vcar->value.id = xstrdup("LAMBDA");
	lexpr->vcdr = new_object(OBJ_CONS);
	lexpr->vcdr->vcar = cadr(args);
	lexpr->vcdr->vcdr = new_object(OBJ_CONS);
	lexpr->vcdr->vcdr->vcar = p4;
	set_object(p1, lexpr); 
	return lexpr;
}

objectp 
F_defun(objectp args)
{
	objectp p1, lexpr;
	p1 = car(args);
	lexpr = new_object(OBJ_CONS);
	lexpr->vcar = new_object(OBJ_IDENTIFIER);
	lexpr->vcar->value.id = xstrdup("LAMBDA");
	lexpr->vcdr = new_object(OBJ_CONS);
	lexpr->vcdr->vcar = cadr(args);
	lexpr->vcdr->vcdr = cddr(args);
	set_object(p1, lexpr); 
	return lexpr;
}

objectp 
F_typeof(objectp args)
{
	objectp p = eval(car(args));
	switch (p->type) {
		case OBJ_INTEGER:
			printf("INTEGER");
			break;
		case OBJ_NULL:
			printf("NULL");
			break;
	    case OBJ_NIL:
		    printf("NIL");
			break;
		case OBJ_T:
			printf("T");
			break;
		case OBJ_CONS:
			printf("LIST");
			break;
		case OBJ_IDENTIFIER:
			if(strcmp(p->value.id,"LAMBDA") == 1)
				printf("FUNCTION");
			else
				printf("IDENTIFIER");
			break;
	}
	return NULL;
}

objectp 
F_setlazy(objectp args)
{
    objectp p;
	p = eval(car(args));
	if(p == t) {
		lazy_eval = true;
		return t;
    } else if(p == nil) {
		lazy_eval = false;
		return nil;
	}
	ASSERTP(1, SET LAZY);
	return null;
}

objectp 
F_gc(objectp args)
{
	garbage_collect();
	return NULL;
}

objectp
F_xor(objectp args)
{
	objectp first;
	first = eval(car(args));
	do {
		if (eval(car(args)) != first)
			return nil;
	} while ((args = cdr(args)) != nil);
	return t;
}

objectp
F_evlis(objectp args)
{
	objectp p, first, prev;
	first = prev = NULL;
	args = car(args);
	ASSERTP(args->type != OBJ_CONS, EVLIS);
	do { 
		p = new_object(OBJ_CONS);
		p->vcar = eval(car(args));
		if (first == NULL)
			first = p;
		if (prev != NULL)
			prev->vcdr = p;
		prev = p;
	} while((args = cdr(args)) != nil);
	return first;
}

objectp 
F_pop(objectp stack)
{
	objectp p, q, r;
	ASSERTP(car(stack)->type != OBJ_IDENTIFIER, POP);
	r = eval(car(stack));
	ASSERTP(r->type != OBJ_CONS, POP);
	q = car(r);
	p = cdr(r);
	set_object(car(stack), p);
	return q;
}

objectp
F_push(objectp args)
{
	objectp s, e, r, first, prev;
	first = prev = NULL;
	ASSERTP(car(cdr(args))->type != OBJ_IDENTIFIER, PUSH);
	e = eval(car(args));
	s = eval(car(cdr(args)));
	ASSERTP(s->type != OBJ_CONS, PUSH);
	r = new_object(OBJ_CONS);
	r->vcar = e;
	first = r;
	prev = r;
	do { 
		r = new_object(OBJ_CONS);
		r->vcar = car(s);
		if (first == NULL)
			first = r;
		if (prev != NULL)
			prev->vcdr = r;
		prev = r;
	} while ((s = cdr(s)) != nil);
	set_object(car(cdr(args)), first);
	return first;
}
objectp
F_dump(objectp args)
{
	dump_objects();
	return nil;
	
}

funcs functions[FUNCS_N] = {
	{"*",F_prod},
	{"+",F_add},
	{"<",F_less},
	{"<=",F_lesseq},
	{"=",F_eq},
	{">",F_great},
	{">=", F_greateq},
	{"ADD",F_add },
	{"AND",F_and},
	{"APPEND",F_append},
	{"ASSOC", F_assoc},
	{"ATOMP",F_atom},
	{"BACKQUOTE",F_bquote},
	{"CAR",F_car},
	{"CDR",F_cdr},
	{"COMMA",F_comma},
	{"COND",F_cond},
	{"CONS",F_cons},
	{"CONSP",F_consp},
	{"DEFMACRO",F_defmacro},
	{"DEFUN",F_defun},
	{"DUMP", F_dump},
	{"EQ",F_eq},
	{"EVAL",F_eval},
	{"EVLIS",F_evlis},
	{"IF",F_if},
	{"LABELS",F_labels},
	{"LET",F_let},
	{"LIST",F_list},
	{"LISTP",F_listp},
	{"MAP",F_map},
	{"MEMBERP",F_member},
	{"NOT",F_not},
	{"NULL",F_not},
	{"OR",F_or},
	{"ORD",F_ord},
	{"PAIR" , F_pair},	
	{"PAIR*",F_pair_neval},	
	{"POP", F_pop},
	{"PROD", F_prod },
	{"PROG1",F_prog1},
	{"PROG2",F_prog2},
	{"PROGN",F_progn},
	{"PUSH", F_push},
	{"QUIT",F_quit},
	{"QUOTE",F_quote},
	{"SETLAZYEVAL",	F_setlazy},
	{"SETQ",F_setq},
	{"SUBST",F_subst},
	{"TYPEOF",F_typeof},
	{"WHILE",F_while},
	{"XOR",F_xor}
};
