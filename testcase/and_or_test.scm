(and #f 0)
(and 1 2 3)
(and 1 2 3 #f)

(or #f 0)
(or 1 2 3)
(or #f 1 2 3)
(or #f #f #f)

(and)
(or)

(and #t #t #t)
(or #f #f #t)

(define (test-short-circuit-and)
  (define x 100)
  (and #f (define x 200))
  x)

(test-short-circuit-and)

(define (test-short-circuit-or)
  (define y 100)
  (or #t (define y 200))
  y)

(test-short-circuit-or)

(and (> 5 3) (< 2 4) (= 1 1))
(or (> 5 10) (< 2 1) (= 3 3))

(define (safe-divide x y)
  (and (not (= y 0)) (/ x y)))

(safe-divide 10 2)
(safe-divide 10 0)

(exit)
