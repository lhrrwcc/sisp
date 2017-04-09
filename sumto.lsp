(defun sumto(n)
	(if (eq n 1) 1
		(+ n (sumto (+ n -1)))))

(defun sumto_help(n a)
	(if (eq n 0) a
	  (sumto_help (+ n -1) (+ a n))))

(defun sumto2(n)
	(sumto_help n 0))
