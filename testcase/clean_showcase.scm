(define pi 314)
(define (circle-area r) (* pi (* r r)))

(define (max2 a b) (if (> a b) a b))
(define (max3 a b c) (max2 a (max2 b c)))

(define (abs-val x) (if (< x 0) (- 0 x) x))

(define (even-check n) (= (modulo n 2) 0))

(define (sum-gp a0 r n)
  (* a0
     (if (= r 1)
         n
         (/ (- 1 (expt r n)) (- 1 r)))))

(circle-area 5)
(max3 10 7 15)
(abs-val -8)
(even-check 10)
(sum-gp 2 3 4)

(exit)
