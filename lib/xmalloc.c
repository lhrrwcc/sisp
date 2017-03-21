#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <err.h>

#include "config.h"
/*
 * Return an allocated memory area.
 */

static void* 
fixup_null_alloc ( size_t n)
{
  void *p;

  p = 0;
  if (n == 0)
    p = malloc ((size_t) 1);
  if (p == 0)
    err(1, "Memory exhausted");
  return p;
}


__inline__ void* 
xmalloc (size_t n)
{
  void *p;
  
  assert(n > 0);
  p = malloc (n);
  if (p == 0)
  	p = fixup_null_alloc (n);
  return p;
}
