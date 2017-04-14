(DEFUN M(X)
(IF (= X 1) 1
(+ (/ 1 X) (M (+ X -1)))))

(DEFUN Z(X)
(IF (= X 100) 100
	(* (+ (M X) (Z (+ X 1)))))) 

(DEFUN G(X)
(IF (= X 1) 1
	(+ (/ 1 X) (G (+ X -1)))))

(DEFUN F(A B C D E H I J K L)
(IF (= A 0) 1
	(+ 1 (F (+ A -1) (G B) (G C) (G D) (G E) (G H) (G I) (G J) (G K) (G L)))))

(DEFUN TEST(X)
	(F X X X X X X X X X X))
