(defun nullj (x)
  (eq x '()))

(defun andj (x y)
  (cond (x (cond (y 't) ('t '())))
        ('t '())))

(defun notj (x)
  (cond (x '())
        ('t 't)))

(defun appendj (x y)
  (cond ((nullj x) y)
        ('t (cons (car x) (appendj (cdr x) y)))))

(defun listj (x y)
  (cons x (cons y '())))

(defun pairj (x y)
  (cond ((andj (nullj x) (nullj y)) '())
        ((andj (notj (atom x)) (notj (atom y)))
         (cons (listj (car x) (car y))
               (pairj (cdr x) (cdr y))))))

(defun assocj (x y)
  (cond ((eq (caar y) x) (cadar y))
        ('t (assocj x (cdr y)))))

(defun evalj (e a)
  (cond
    ((atom e) (assocj e a))
    ((atom (car e))
     (cond
       ((eq (car e) 'quote) (cadr e))
       ((eq (car e) 'atom)  (atom   (evalj (cadr e) a)))
       ((eq (car e) 'eq)    (eq     (evalj (cadr e) a)
                                    (evalj (caddr e) a)))
       ((eq (car e) 'car)   (car    (evalj (cadr e) a)))
       ((eq (car e) 'cdr)   (cdr    (evalj (cadr e) a)))
       ((eq (car e) 'cons)  (cons   (evalj (cadr e) a)
                                    (evalj (caddr e) a)))
       ((eq (car e) 'cond)  (evconj (cdr e) a))
       ('t (evalj (cons (assocj (car e) a)
                        (cdr e))
                  a))))
    ((eq (caar e) 'label)
     (evalj (cons (caddar e) (cdr e))
            (cons (listj (cadar e) (car e)) a)))
    ((eq (caar e) 'lambda)
     (evalj (caddar e)
            (appendj (pairj (cadar e) (evlisj (cdr e) a))
                     a)))))

(defun evconj (c a)
  (cond ((evalj (caar c) a)
         (evalj (cadar c) a))
        ('t (evconj (cdr c) a))))

(defun evlisj (m a)
  (cond ((nullj m) '())
        ('t (cons (evalj  (car m) a)
                  (evlisj (cdr m) a)))))

