(define (factorial n acc)
  (if (<= n 1)
      acc
      (factorial (- n 1) (* n acc))))

(factorial 20 1)
(factorial 30 1)
(factorial 50 1)
(exit)
