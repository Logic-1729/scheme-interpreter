(define (factorial n)
  (if (= n 0) 
      1 
      (* n (factorial (- n 1)))))

(define (fibonacci n)
  (if (<= n 1) 
      n 
      (+ (fibonacci (- n 1)) (fibonacci (- n 2)))))

(define (gcd a b)
  (if (= b 0) 
      a 
      (gcd b (modulo a b))))

(factorial 5)
(fibonacci 8)
(gcd 48 18)

(define (power base exp)
  (if (= exp 0)
      1
      (* base (power base (- exp 1)))))

(power 2 5)

(exit)
