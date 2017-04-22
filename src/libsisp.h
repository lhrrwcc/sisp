char __builtin_functions[] =
	"(defun caar (x) (car (car x)))\n"
	"(defun cadr (x) (car (cdr x)))\n"
	"(defun cdar (x) (cdr (car x)))\n"
	"(defun cddr (x) (cdr (cdr x)))\n"
	"(defun caaar (x) (car (car (car x))))\n"
	"(defun caadr (x) (car (car (cdr x))))\n"
	"(defun cadar (x) (car (cdr (car x))))\n"
	"(defun caddr (x) (car (cdr (cdr x))))\n"
	"(defun cdaar (x) (cdr (car (car x))))\n"
	"(defun cdadr (x) (cdr (car (cdr x))))\n"
	"(defun cddar (x) (cdr (cdr (car x))))\n"
	"(defun cdddr (x) (cdr (cdr (cdr x))))\n"
	"(defun butlast(l)\n"
	"  (cond ((eq (cdr l) nil) nil)\n"
	"        (t (cons (car l) (butlast (cdr l))))))\n"
	"(defun get-names(x)\n"
	"    (cond ((eq x nil) nil)"
	"           (t (cons (caar x) (get-names (cdr x))))))\n"	
	"(defun get-values(x)\n"
	"    (cond ((eq x nil) nil)"
	"           (t (cons (cdar x) (get-values (cdr x))))))\n"	
	"(defun unpair(x)\n"
	"    (list (get-names x) (get-values x)))\n"
	"(defun rplaca (x y)\n"
	"  (cons y (cdr x)))\n"
	"(defun rplacd (x y)\n"
	"  (cons (car x) y))\n"
	"(defun alt(x)\n"
	"  (cond ((or (null x)\n"
	"	      (null (cdr x))) x)\n"
	"	 (t (cons (car x) (alt (cddr x))))))\n"
	"(defun succ (x y)\n"
	"  (cond ((or (null y) (null (cdr y))) nil)\n"
	"	((eq (car y) x) (cadr y))\n"
	"	(t (succ x (cdr y)))))\n"
	"(defun pred (x y)\n"
	"  (cond ((or (null y) (null (cdr y))) nil)\n"
	"	((eq (cadr y) x) (car y))\n"
	"	(t (pred x (cdr y)))))\n";