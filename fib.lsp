(defun f(n) 
  (if 
	(or (eq n 0) (eq n 1)) 
		1 
		(+ (f (+ n -1)) (f (+ n -2)))))

(defun fta(n y z)
  (cond 
	((eq n 0) (+ y z))
	((eq n 1) (+ y z))
		(fta (+ n -1) (+ y n -1) (+ z n -2))))
(defun ft(n)
  (fta n n n))
