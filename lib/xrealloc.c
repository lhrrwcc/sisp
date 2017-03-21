#include <stdlib.h>

#include <assert.h>
#include <err.h>

#include "config.h"

__inline__ void*
xrealloc(void *ptr, size_t size)
{
	void *newptr;
	
	assert(ptr != NULL);
	assert(size > 0);
	
	if ((newptr = realloc(ptr, size)) == NULL)
		err(1, NULL);
	
	return newptr;
}

