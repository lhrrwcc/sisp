(DEFINE (MM A B C D E F G H)
    (EVLIS '((CONS A B) (CONS B C) (CONS C D)
             (CONS D E) (CONS E F) (CONS F G)
             (CONS G H)) 
    )
)
(DEFINE (F X Y Z W)
    (LIST   (CONS (MM W Z Y X X Y Z W) (MM W Z Y X X Y Z W)) 
            (CONS (MM W Z Y X X Y Z W) (MM W Z Y X X Y Z W))
            (CONS (MM W Z Y X X Y Z W) (MM W Z Y X X Y Z W))
            (CONS (MM W Z Y X X Y Z W) (MM W Z Y X X Y Z W))
            (CONS (MM W Z Y X X Y Z W) (MM W Z Y X X Y Z W))
            (CONS (MM W Z Y X X Y Z W) (MM W Z Y X X Y Z W))
    )
)
(DEFINE (G X Y Z W)
    (LIST   (CONS (F X Y Z W) (F X Y Z W)) 
            (CONS (F X Y Z W) (F X Y Z W))
            (CONS (F X Y Z W) (F X Y Z W))
            (CONS (F X Y Z W) (F X Y Z W))
            (CONS (F X Y Z W) (F X Y Z W))
            (CONS (F X Y Z W) (F X Y Z W))
            (CONS (F X Y Z W) (F X Y Z W))
            (CONS (F X Y Z W) (F X Y Z W))
            (CONS (F X Y Z W) (F X Y Z W))
            (CONS (F X Y Z W) (F X Y Z W))    
    )
)
(DEFINE (H X Y Z W)
    (LIST   (G X Y Z W)
            (G X Y Z W)
            (G X Y Z W)
            (G X Y Z W)
            (G X Y Z W)
            (G X Y Z W)
            (G X Y Z W)
    )
)

(DEFINE (TEST X Y Z W)
    (IF (= X 0) NIL
        (CONS (H 1 2 3 4) (TEST (+ X -1) (H X Y Z W) (G X Y Z W)  (F X Y Z W))))
)
(DEFINE (STRESS N)
    (TEST N (H N (+ N 1) (+ N 2) (+ N 3))
            (G N (+ N 1) (+ N 2) (+ N 3))
            (F N (+ N 1) (+ N 2) (+ N 3))
    )
)
;(stress 2)
(stress 2)
(stress 3)
(stress 4)
(stress 5)
(stress 2)
(stress 3)
(stress 4)
(stress 5)
(stress 2)
(stress 3)
(stress 4)
(stress 5)

;(stress 100)
;(stress 200)
;(stress 500)
;(stress 1000)
;(stress 1500)
;(stress 2500)

;;(quit)