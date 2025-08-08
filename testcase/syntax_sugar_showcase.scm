(define pi 3.14159)
(define (circle-area r) (* pi (* r r)))
(define (circle-circumference r) (* 2 (* pi r)))

(define (max2 a b) (if (> a b) a b))
(define (max3 a b c) (max2 a (max2 b c)))

(define (abs x) (if (< x 0) (- 0 x) x))
(define (sign x) 
  (if (> x 0) 1 
      (if (< x 0) -1 0)))

(define (even? n) (= (modulo n 2) 0))
(define (odd? n) (not (even? n)))

(define (sum-range from to)
  (if (> from to)
      0
      (+ from (sum-range (+ from 1) to))))

(quote "=== BASIC FUNCTIONS ===")
(circle-area 5)
(max3 10 7 15)
(abs -8)
(sign -5)

(quote "=== PREDICATES ===")
(even? 10)
(odd? 10)
(even? 7)

(quote "=== RECURSIVE ===")
(sum-range 1 10)

(quote "=== ORIGINAL FUNCTION ===")
(define (sum-gp a0 r n)
  (* a0
     (if (= r 1)
         n
         (/ (- 1 (expt r n)) (- 1 r)))))

(sum-gp 2 3 4)

(exit)
