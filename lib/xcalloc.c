#include <stdlib.h>
#include <assert.h>
#include <err.h>

#include "config.h"
/*
 * Return an allocated memory area.
 */

__inline__ void 
*xcalloc ( unsigned int n, size_t size)
{
	void *ptr;
	
	assert(size > 0);
	
	if ((ptr = calloc(n, size)) == NULL)
		err(1, NULL);
	return ptr;
}
