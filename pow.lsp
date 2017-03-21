(defun f(n) 
  (if 
	(or (eq n 0) (eq n 1)) 
		1 
		(+ (f (+ n -1)) (f (+ n -2)))))
(DEFUN I(X)
   (CONS (CAR L) X))
(DEFUN POW(L)
   (IF (NULL L)
	   '(NIL)
	   (LET
		 ((PS (POW (CDR L))))
		 (APPEND PS (MAP I PS)))))
(defun factaux(n a)
    (if (eq n 1) a
	(factaux (+ n -1) (* n a))))
(defun fact(n)
    (factaux n 1))
(defun listfact(n)
  (if (eq n 0) nil
	(cons (fact n) (listfact (+ n -1)))))
(defun listfib(n)
  (if (eq n 0) nil
	(cons (f n) (listfib (+ n -1)))))
(defun makel(n)
  (if (eq n 0) nil
	(cons nil (makel (+ n -1)))))
(defun STRESS(n)
  (if (eq n 0) 'DONE
	(progn 
	  (pow (append (listfact n) (listfib n))) 
	  (STRESS (+ n -1)))))
(STRESS 10)
(STRESS 10)
(STRESS 10)
(QUIT)
