#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "sisp.h"
#include "eval.h"
#include "funcs.h"
#include "extern.h"
#include "misc.h"

objectp 
handsig(char *str,objectp p)
{
	printf("; %s.", str);
	longjmp(je, 1);
	return null;
}

static int 
compar(const void *p1, const void *p2)
{
	return strcmp(((funcs *)p1)->name, ((funcs *)p2)->name);
}

static int 
ss(objectp p, objectp body)
{
	objectp p1, p2;
	do {
		p1 = car(body);
		switch (p1->type) {
		case OBJ_IDENTIFIER:
			if (!strcmp(p1->value.id, p->value.id))
				return 1;
			p2 = try_object(p1);
			if (p2->type == OBJ_CONS) 
				if(ss(p,p2) == 1)
					return 1;
			break;
		case OBJ_CONS:
			if (ss(p,p1))
				return 1;
			break;
		case OBJ_T:
		case OBJ_NIL:
		case OBJ_NULL:
			if (p == p1)
				return 1;
			break;
		case OBJ_INTEGER:
			if (p1->value.i == p->value.i)
				return 1;
			p2 = try_object(p1);
			if (p2->type == OBJ_CONS)
			 if(ss(p,p2) == 1)
				return 1;
			break;
		case OBJ_DOUBLE:
			if (p1->value.d == p->value.d)
				return 1;
			p2 = try_object(p1);
			if (p2->type == OBJ_CONS)
			 if(ss(p,p2) == 1)
				return 1;
			break;
		}
	} while ((body = cdr(body)) != nil);
	return 0;
}

__inline__ static objectp 
eval_func_lazy(objectp p, objectp args)
{
	objectp first, prev;
	objectp q, r, bind_list, value;
	first = prev = NULL;
	bind_list = cadr(p);
	do {
		if (!(ss(car(bind_list), car(cddr(p)))))
			value = nil;
		else 
			value = eval(car(args));
		/* HACK! */
		lazy_eval = false;
		r = new_object(OBJ_CONS);
		r->vcar = try_eval(car(bind_list));
		if (first == NULL)
			first = r;
		if (prev != NULL)
			prev->vcdr = r;
		prev = r;
		set_object(car(bind_list), value);
		args = cdr(args);
	} while ((bind_list = cdr(bind_list)) != nil);
	q = F_progn(cddr(p));
	bind_list = cadr(p);
	do {
		value = car(first);
		set_object(car(bind_list), value);
		first = cdr(first);
	} while ((bind_list = cdr(bind_list)) != nil);
	/* UNHACK! */
	lazy_eval = true;
	return q;
}

static objectp 
eval_func(objectp p, objectp args)
{
	objectp a, b;
	objectp q, M;
	objectp bind_list;
	q = a = b = NULL;
	bind_list = cadr(p);
	ASSERTP(bind_list == NULL,EVAL_FUNC);
	do {
		M = new_object(OBJ_CONS);
		M->vcar = new_object(OBJ_CONS);
		M->vcar->vcar = eval(car(args));
		M->vcar->vcdr = new_object(OBJ_CONS);
		M->vcar->vcdr->vcar = try_eval(car(bind_list));
		if (a == NULL) 
			a = M;
		if (b != NULL) 
			b->vcdr = M;
		b = M;
		args = cdr(args);
	} while ((bind_list = cdr(bind_list)) != nil);
	bind_list = cadr(p);
	args = a;
	do {
		set_object(car(bind_list), caar(a));
		a = cdr(a);
	} while ((bind_list = cdr(bind_list)) != nil);
	q = null;
	if (!setjmp(je))
		q = F_progn(cddr(p));
	bind_list = cadr(p);
	a = args;
	do {
		set_object(car(bind_list), cadr(car(a)));
		a = cdr(a);
	} while ((bind_list = cdr(bind_list)) != nil);
	return q;
}

objectp 
eval_cons(objectp p)
{
	objectp func_name;
	unsigned int n_args = 0;
	if (car(p)->type == OBJ_IDENTIFIER) {
		funcs key, *item;
		if (!strcmp(car(p)->value.id, "LAMBDA")) 
			return p;
		key.name = car(p)->value.id;
		if ((item = bsearch(&key, functions, 
						sizeof(functions)/sizeof(functions[0]), 
						sizeof(functions[0]), compar)) != NULL) 
			return item->func(cdr(p));
		func_name = get_object(car(p));
		if(func_name == null || func_name == NULL)
			longjmp(je,1);
		if (card(cdr(p)) != (n_args=card(cadr(func_name)))) {
			printf(";; %s: EXPECTED %d ARGUMENTS.", car(p)->value.id, n_args);
			longjmp(je,1);
		}
		return lazy_eval==true ? eval_func_lazy(func_name, cdr(p)) : 
								 eval_func(func_name, cdr(p));
	}
	handsig("NOT A FUNCTION", car(p));
	return null;
}