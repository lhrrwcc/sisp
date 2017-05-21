#ifndef _VMM_H

extern objectp oballoc(a_type);
extern void obfree(objectp);
extern void recycle_pool(a_type);

struct t_pool {
	ssize_t	used_size;
	ssize_t	free_size;
	struct {
		objectp	u;
		objectp f;
	} head;
} pool[7];

#define _VMM_H

#endif 
