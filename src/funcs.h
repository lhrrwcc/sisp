#ifndef _FUNCS_H
extern objectp F_quit(objectp);
extern objectp F_used_obj(objectp);
extern objectp F_eq(objectp);
extern objectp F_princ(objectp);
extern objectp F_atom(objectp);
extern objectp F_cons(objectp);
extern objectp F_list(objectp);
extern objectp F_quote(objectp);
extern objectp F_bquote(objectp);
extern objectp F_comma(objectp);
extern objectp F_while(objectp);
extern objectp F_progn(objectp);
extern objectp F_setq(objectp);
extern objectp F_gc(objectp);
extern objectp F_consp(objectp);
extern objectp F_lambda(objectp); 
extern objectp F_typeof(objectp);
extern objectp F_eval(objectp);
extern objectp F_append(objectp);
#define FUNCS_N 45
funcs functions[FUNCS_N];
#define _FUNCS_H

#endif