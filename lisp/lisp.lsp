(defun f (n) (f n))
(defun fact (n) (if (= n 0) 1 (* n (fact (- n 1)))))
