(define a 10)
a
(define b 20)
b
(+ a b)
(define square (lambda (x) (* x x)))
(square 5)
(define factorial (lambda (n) (if (= n 0) 1 (* n (factorial (- n 1))))))
(factorial 4)
(expt 2 3)
(expt 3 4)
(define power (lambda (base exp) (expt base exp)))
(power 2 5)
(exit)
