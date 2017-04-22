#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include "sisp.h"
#include "extern.h"
#include "misc.h"

#define HANDSIG(ARG,EXP)				\
	do { if (ARG==NULL)	{				\
		fprintf(stderr, "; " #EXP ".");	\
		longjmp(je, 1); }				\
	} while(0)

objectp 			nil;
objectp 			t;
objectp 			null;

static objectp 		free_objs_list = NULL;
static objectp 		used_objs_list = NULL;
static object_pairp setobjs_list   = NULL;

static unsigned int gc_id = 0;

static int 			free_objs = 0;
static int 			used_objs = 0;

__inline__ objectp 
new_object(a_type type)
{
	objectp p;
	if (free_objs_list == NULL)
		p = (objectp) malloc(sizeof(struct object));
	else { 
		p = free_objs_list;
		free_objs_list = free_objs_list->next;
		--free_objs;
	}
	p->next = used_objs_list;
	used_objs_list = p;
	p->type = type;
	if (type == OBJ_CONS) 
		p->vcar = p->vcdr = nil;
	p->gc = 0;
	++used_objs;
	return p;
}

__inline__  objectp 
search_object_rational(long int num, long int den)
{
	objectp p;
	for (p = used_objs_list; p != NULL; p = p->next)
		if (p->type == OBJ_RATIONAL)
			if(p->value.r.n == num && p->value.r.d == den)
				return p; 
	return (objectp) NULL;
}

__inline__  objectp 
search_object_integer(long int i)
{
	objectp p;
	for (p = used_objs_list; p != NULL; p = p->next)
		if (p->type == OBJ_INTEGER && p->value.i == i)
			return p;
	return (objectp) NULL;
}

__inline__  objectp 
search_object_identifier(char *s)
{
	objectp p;
	for (p = used_objs_list; p != NULL; p = p->next)
		if (p->type == OBJ_IDENTIFIER && !strcmp(p->value.id, s))
			return p;
	return (objectp) NULL;
}

void 
init_objects(void)
{
	nil 	= new_object(OBJ_NIL);
	t		= new_object(OBJ_T);
	null	= new_object(OBJ_NULL);
}

void
remove_object(void)
{
	setobjs_list = setobjs_list->next;
}

void
set_object(objectp name, objectp value)
{
	object_pairp p, next;
	HANDSIG(value, SET OBJECT);

	for (p = setobjs_list; p != NULL; p = next) {
		next = p->next;
		if (name->type == OBJ_IDENTIFIER && p->name->value.id != NULL && 
			!strcmp(name->value.id, p->name->value.id)) {
			p->value = value;
			return;
		}
	}
	p = (object_pairp) malloc(sizeof(struct object_pair));
	p->next = setobjs_list;
	setobjs_list = p;
	p->name = name;
	p->value = value;
}
/*
 * this is similar to get_object function except it might return null
 * It is needed in order to know if a variable used as a parameter
 * in a function definition already has a value
 */
__inline__ objectp
try_object(objectp name)
{
	object_pairp p;
	if (name == null)
		return null;
	for (p = setobjs_list; p != NULL; p = p->next)
		if(p->name->value.id != NULL &&
			!strcmp(name->value.id, p->name->value.id))
			return p->value;
	return null;
}

__inline__ objectp 
get_object(objectp name)
{
	object_pairp p;
	HANDSIG(name, GETOBJECT);

	for (p = setobjs_list; p != NULL; p = p->next)
		if (p->name->value.id != NULL && 
			!strcmp(name->value.id, p->name->value.id))
			return p->value;
	
	HANDSIG(NULL, OBJECT NOT FOUND);
	return null;
}

void 
dump_objects(void)
{
	object_pairp p;
	objectp q;
	int i =0;
	printf("used: %d\tfree: %d\n",used_objs, free_objs);
/*
	printf("objetos guardados: \n");
	for (p = setobjs_list; p != NULL; p = p->next) {
		princ_object(stdout,p->name); printf(":\t ");
		princ_object(stdout,p->value); printf("\n");		
	}
*/
	printf("valores libres: \n");
	if(free_objs <= 20)
	for (q = free_objs_list; q != NULL; q = q->next) {
		 i++;
		 princ_object(stdout,q); printf("\t:%d\n",i);
	}
	printf("\nused: %d\tfree: %d\n", used_objs, free_objs);

}	

__inline__ static void
tag_tree(objectp p)
{
	if(p->gc == gc_id) 
		return;
	p->gc = gc_id;
	if(p->type == OBJ_CONS) {
		tag_tree(p->vcar);
		tag_tree(p->vcdr);
	}
}

__inline__ static void 
tag_whole_tree(void)
{
	object_pairp p;
	for (p = setobjs_list; p != NULL; p = p->next) {
		tag_tree(p->name);
		tag_tree(p->value);
	}
}

__inline__ void 
garbage_collect(void)
{
	objectp p, new_used_objs_list = t, next;
	if(++gc_id == UINT_MAX-1)
	    gc_id = 1;
	tag_whole_tree();
	for (p = used_objs_list; p != NULL && p != t; p = next) {
		next = p->next;
		if (p->gc != gc_id && p != null) {
			p->next = free_objs_list;
			free_objs_list = p;
			++free_objs;
			--used_objs;
		} else {
			p->next = new_used_objs_list;
			new_used_objs_list = p;
		}
	}
	used_objs_list = new_used_objs_list;
}

/*
 * GARBAGE COLLECTOR WITH THREADS

static
void *tt(void *tree)
{
	objectp *t_ptr = (objectp *)tree;
	if((*t_ptr)->gc == gc_id)
		return NULL;
	(*t_ptr)->gc = gc_id;
	if((*t_ptr)->type == OBJ_CONS) {
		tt(&((*t_ptr)->vcar));
		tt(&((*t_ptr)->vcdr));
	}
}

__inline__ static
void *tag_tree(void *tree)
{
	objectp *t_ptr = (objectp *)tree;
	pthread_t tag_l,tag_r;
	if((*t_ptr)->gc == gc_id)
		return NULL;
	(*t_ptr)->gc = gc_id;
	if( (*t_ptr)->type == OBJ_CONS) {
		pthread_create(&tag_l, NULL, tt, &((*t_ptr)->vcar));
		pthread_create(&tag_r, NULL, tt, &((*t_ptr)->vcdr));
		pthread_join(tag_l, NULL);
		pthread_join(tag_r, NULL);
	}
}
__inline__ static void
tag_whole_tree(void)
{
	object_pairp p;
	pthread_t tag_name, tag_value;
	for (p = setobjs_list; p != NULL; p = p->next) {
		pthread_create(&tag_name, NULL, tt, &(p->name));
		pthread_create(&tag_value, NULL, tt, &(p->value));
		pthread_join(tag_name, NULL);
		pthread_join(tag_value, NULL);
	}

}
*/