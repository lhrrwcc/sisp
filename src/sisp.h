#ifndef SISP_H
#define IDENTIFIER	7
#define INTEGER		8
#define DOUBLE      9
typedef enum	{false, true} bool;
	
typedef enum 	{OBJ_NULL, OBJ_NIL, OBJ_T, OBJ_IDENTIFIER,
				OBJ_CONS, OBJ_INTEGER, OBJ_DOUBLE} a_type;
typedef struct 	object *objectp;

struct object {
  a_type type;
  unsigned int gc; /* unsigned? */
  union {
    char *id;
    int i;
    double d;
    struct {
      objectp car;
      objectp cdr;
    } c;
  } value;
  objectp next;
};

typedef struct object_pair *object_pairp;

struct object_pair {
	objectp name;
	objectp value;
	object_pairp next;
};

typedef struct {
	char *name;
	objectp (*func)(objectp args);
} funcs;

#define vcar	value.c.car
#define vcdr	value.c.cdr

#define SISP_H
#endif
