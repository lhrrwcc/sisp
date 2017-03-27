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
(setq s0 nil)
(setq s1 (S s0))
(setq s2 (S s1))
(setq s3 (S s2))
(setq s4 (S s3))
(setq s5 (S s4))
(setq s6 (S s5))
(setq s7 (S s6))
(setq s8 (S s7))
(setq s9 (S s8))
