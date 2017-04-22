(defun g(x) (list x))
(defun h(y) (list y))
(defun f(x y z) (list (g x) (g y) (h z)))
(f 1 2 3)

