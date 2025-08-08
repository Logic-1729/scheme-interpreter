(define factorial (lambda (n) 
  (if (= n 0) 1 (* n (factorial (- n 1))))))
(factorial 5)

(define fibonacci (lambda (n)
  (if (<= n 1) 
      n 
      (+ (fibonacci (- n 1)) (fibonacci (- n 2))))))
(fibonacci 5)
(fibonacci 8)

(define sum-to-n (lambda (n)
  (if (= n 0) 
      0 
      (+ n (sum-to-n (- n 1))))))
(sum-to-n 10)

(define power (lambda (base exp)
  (if (= exp 0)
      1
      (* base (power base (- exp 1))))))
(power 2 3)
(power 3 4)

(define gcd (lambda (a b)
  (if (= b 0)
      a
      (gcd b (modulo a b)))))
(gcd 48 18)
(gcd 21 14)

(define count-digits (lambda (n)
  (if (< n 10)
      1
      (+ 1 (count-digits (quotient n 10))))))
(count-digits 123)
(count-digits 9876)

(define ackermann (lambda (m n)
  (if (= m 0)
      (+ n 1)
      (if (= n 0)
          (ackermann (- m 1) 1)
          (ackermann (- m 1) (ackermann m (- n 1)))))))
(ackermann 0 5)
(ackermann 1 3)
(ackermann 2 3)

(define reverse-number (lambda (n acc)
  (if (= n 0)
      acc
      (reverse-number (quotient n 10) (+ (* acc 10) (modulo n 10))))))
(define reverse-num (lambda (n) (reverse-number n 0)))
(reverse-num 1234)

(exit)
