
#include <stdio.h>
#include <stdlib.h>
#include "sisp.h"
#include "extern.h"
#include "vmm.h"
#define abs(x) (x<0) ? -x : x
void
free_pools(void)
{
	objectp prev, p, next;
	int i;
		
	for(i=0;i<7;i++) {
		prev = NULL;
		for(p = pool[i].head.u; p != NULL; prev = p, p = next) {
			next = p->next;
			if(prev == NULL);
				pool[i].head.u = pool[i].head.u->next;
			prev = next;
			if(p->type == OBJ_IDENTIFIER)
				free(p->value.id);
			free(p);
			pool[i].used_size--;
		}
		prev = NULL;
		for(p = pool[i].head.f; p != NULL; prev = p, p = next) {
			next = p->next;
			if(prev == NULL);
				pool[i].head.f = pool[i].head.f->next;
			prev = next;
			if(p->type == OBJ_IDENTIFIER)
				free(p->value.id);
			free(p);
			pool[i].free_size--;
		}
	}
}

void
recycle_pool(a_type type)
{
    ssize_t chunk_size;
    objectp p, next ,prev;

    if(type == OBJ_IDENTIFIER)
        chunk_size = 16;
    else if(type == OBJ_CONS)
        chunk_size = 32;
    else if(type == OBJ_INTEGER || type == OBJ_RATIONAL)
        chunk_size = 8;
    else
        chunk_size = 64;

    for(p=pool[type].head.f; 
        p != NULL && pool[type].free_size > chunk_size; 
        prev = p, p=next) {
        next = p->next;
	    if(prev == NULL);
		    pool[type].head.f = pool[type].head.f->next;
		prev = next;
    	if(p->type == OBJ_IDENTIFIER)
				free(p->value.id);
		free(p);
		pool[type].free_size--;
    }
    return ;    
}

void
feed_pool(a_type type)
{
    ssize_t units;
    objectp new_heap_list;

    if(type == 3)
        units = 15;
    else if(type == 4)
        units = 63;
    else if(type == 5 || type == 6)
        units = 7;
    else
        return ;

	pool[type].free_size = units+1;
    pool[type].head.f = malloc(OBJ_SIZE);
	pool[type].head.f->next = NULL;
	new_heap_list = pool[type].head.f;

	while(units--) {
		pool[type].head.f->next = malloc(OBJ_SIZE);
		pool[type].head.f = pool[type].head.f->next; 
	}
	pool[type].head.f->next = NULL;
	pool[type].head.f = new_heap_list;
}

objectp
oballoc(a_type OBJ_TYPE)
{
    objectp p;

    if (OBJ_TYPE <= 2)
        return malloc(OBJ_SIZE);
    if (pool[OBJ_TYPE].free_size == 0)
        feed_pool(OBJ_TYPE);
    p = pool[OBJ_TYPE].head.f;
    pool[OBJ_TYPE].head.f = pool[OBJ_TYPE].head.f->next;
	pool[OBJ_TYPE].free_size--;

    return p;
}