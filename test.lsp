(DEFUN G(X)
	   (IF (= X 1) 1
        (+ (/ 1 X) (G (+ X -1)))))
(DEFUN H(X)
	   (IF (= X 1) 1
        (* (/ 1 X) (H (+ X -1)))))

(DEFUN D(X)
	   (IF (= X 1) 1
        (/ (/ 1 X) (D (+ X -1)))))

(DEFUN TESTNUM(N)
	   (IF (= N 0) NIL
		   (CONS (G N) 
		    (CONS (H N) 
			(CONS (D N)
			 (CONS (TESTNUM (+ N -1))))))))

(SETQ A (TESTNUM 10))
(SETQ B (TESTNUM 20))
(SETQ C (TESTNUM 30))

(DEFUN GC(X)
	   (IF (= X 0) NIL
        (list (LIST NIL NIL t nil t nil t nil a a a a b c c c c) (GC (+ X -1)))))

(DEFUN HC(X)
	   (IF (= X 0) NIL
        (LIST (LIST 'A A 'B B 'C C t t t nil nil t) (HC (+ X -1)))))

(DEFUN DC(X)
	   (IF (= X 0) NIL
        (CONS (LIST A B C a b c a b c) (CONS (DC (+ X -1))))))
        

(DEFUN TESTCONS(N)
	   (IF (= N 0) NIL
		   (CONS (GC N) 
		    (CONS (HC N) 
			(CONS (DC N)
			 (CONS (TESTCONS (+ N -1))))))))

(DEFUN TEST(N)
    (IF (= N 0) NIL
	(CONS (TESTNUM N) (CONS (TESTCONS N) (TEST (+ N -1))))))


(DEFUN STRESS()
(PROGN
(TEST 20) (TEST 21)
(TEST 22) (TEST 23)
(TEST 24) (TEST 25)
(TEST 26) (TEST 27)
(TEST 28) (TEST 29)
(TEST 30) (TEST 31)
(TEST 32) (TEST 33)
(TEST 34) (TEST 35)
(TEST 36) (TEST 37)
(TEST 38) (TEST 39)
))
;(TEST 20) 
;(TEST 21)
;(TEST 22) 
;(TEST 23)
;(TEST 24) 
;(TEST 25)
;(TEST 26) 
;(TEST 27)
;(TEST 28) 
;(TEST 29)
;(TEST 30) 
;(TEST 31)
;(TEST 32) 
;(TEST 33)
;(TEST 34) 
;(TEST 35)
;(TEST 36) 
;(TEST 37)
;(TEST 38) 
;(TEST 39)

;(STRESS)
;(STRESS)
;(STRESS)
;(STRESS)
;(STRESS)
;(STRESS)
;(STRESS)
;(STRESS)
;(STRESS)
;(STRESS)
