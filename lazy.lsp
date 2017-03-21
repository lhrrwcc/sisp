(defun g(x) (list x y))
(defun f(x y) (list (g x)))

(setq a '(1 2 3 4))

(defun h(x) (list x a))
