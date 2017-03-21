#include "config.h"

#include <string.h>

/*
 * Duplicate a string.
 */

__inline__ char *xstrdup(const char *s)
{
	return strcpy(xmalloc(strlen(s) + 1), s);
}
