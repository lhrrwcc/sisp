#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sisp.h"
#include "eval.h"
#include "funcs.h"
#include "extern.h"

int 
card(objectp p)
{
	register int i = 0;
	while (p != nil && p->type == OBJ_CONS) {
		p = p->vcdr;
		++i;
	}
	return i;
}


objectp
sst(objectp b, objectp v, objectp body)
{
	objectp p, first, prev, q;
	first = prev = NULL;
	do {
		p = car(body);
		q = new_object(OBJ_CONS);
		if (p->type == OBJ_IDENTIFIER && b->type == OBJ_IDENTIFIER &&
				!strcmp(p->value.id, b->value.id)) 
			q->vcar = v;
		else if (p->type == OBJ_CONS)
			q->vcar = sst(b,v,p);
		else if (p->type == OBJ_INTEGER && p->value.d == b->value.d)
			    q->vcar = v;
		else
			q->vcar = p;
		if (first == NULL)
			first = q;
		if (prev != NULL)
			prev->vcdr = q;
		prev = q;
	} while ((body = cdr(body)) != nil);
	return first;
}

objectp
eqcons(objectp a, objectp b)
{
	objectp c;
	if (a->type != b->type)
		return nil;
	switch (a->type) {
		case OBJ_IDENTIFIER:
			return strcmp(a->value.id, b->value.id) == 0 ? t : nil;
		case OBJ_CONS:
			c = eqcons(car(a), car(b));
			if(c == nil) 
				return nil;
			break;
		default:
			return t;
	}
	return c->type == OBJ_T ? eqcons(cdr(a), cdr(b)) : nil;
}


void 
princ_object(FILE *fout, objectp p)
{
	if(!p)
		return;
	switch (p->type) {
	case OBJ_NIL:
		fputs("NIL", fout);
		break;
	case OBJ_T:
		fputc('T', fout);
		break;
	case OBJ_IDENTIFIER:
		if(p->value.id != NULL)
			fputs(p->value.id, fout);
		break;
	case OBJ_NULL:
		break;
	case OBJ_INTEGER:
		fprintf(fout, "%lli", p->value.d);
		break;
	case OBJ_CONS:
		fputc('(', fout);
		do {
			princ_object(fout, p->vcar);
			p = p->vcdr;
			if (p != nil) {
				fputc(' ', fout);
				if (p->type != OBJ_CONS) {
					fputs(". ", fout);
					princ_object(fout, p);
				}
			}
		} while (p != nil && p->type == OBJ_CONS);
		fputc(')', fout);
		break;
	default:
		return;
	}
}
