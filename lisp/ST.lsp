(defun S(n) 
  (union n (cons n ())))
(defun zero(n)
  (equal n nil))
(defun C(n m) 
  (member n m))
(defun A(n m) 
  (if (zero m) n
	(S (A n (P m)))))
(defun R(fn acc l) 
  (if (null l) acc 
	(R fn (fn acc (car l)) (cdr l))))
(defun P(n)
  (if (zero n) nil
	(R union () n)))
(defun M (n m)
  (if (zero m) nil
	(A n (M n (P m)))))
(defun B (n m)
  (if (zero m) nil
	(P (B n (P m)))))
(defun ord (n)
  (if (null n) nil
	(S (ord (cdr n)))))
(defun np (n)
  (eql n (ord n)))
(defun eqs (a b)
  (and (subsetp a b) (subsetp b a)))
(defun U(n)
  (if (= n nil) nil
	(union (car n) (U (cdr n)))))
(defun X(n)
  (if (= n nil) nil
	(intersection (car n) (X (cdr n)))))
(setq 0 nil)
(setq 1 (S 0))
(setq 2 (S 1))
(setq 3 (S 2))
(setq 4 (S 3))
(setq 5 (S 4))
(setq 6 (S 5))
(setq 7 (S 6))
(setq 8 (S 7))
(setq 9 (S 8))
