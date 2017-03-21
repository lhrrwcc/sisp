(DEFUN RM(A L)
	   (COND 
		 ((EQ L NIL) NIL)
		 ((EQ (CAR L) A) (RM A (CDR L)))
		 (T (CONS (CAR L) (RM A (CDR L))))))
