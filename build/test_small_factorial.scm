(define (factorial n acc)
  (if (<= n 1)
      acc
      (factorial (- n 1) (* n acc))))

(factorial 5 1)
(factorial 10 1)
(exit)
