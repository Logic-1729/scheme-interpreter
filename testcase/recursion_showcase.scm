(define test-message (lambda () 
  (quote "Testing recursive functions")))

(define factorial (lambda (n) 
  (if (= n 0) 1 (* n (factorial (- n 1))))))

(define fibonacci (lambda (n)
  (if (<= n 1) n (+ (fibonacci (- n 1)) (fibonacci (- n 2))))))

(define gcd (lambda (a b)
  (if (= b 0) a (gcd b (modulo a b)))))

(define power-recursive (lambda (base exp)
  (if (= exp 0) 1 (* base (power-recursive base (- exp 1))))))

(define count-digits (lambda (n)
  (if (< n 10) 1 (+ 1 (count-digits (quotient n 10))))))

(define sum-to-n (lambda (n)
  (if (= n 0) 0 (+ n (sum-to-n (- n 1))))))

(define ackermann (lambda (m n)
  (if (= m 0) (+ n 1)
      (if (= n 0) (ackermann (- m 1) 1)
          (ackermann (- m 1) (ackermann m (- n 1)))))))

(define hanoi-moves (lambda (n)
  (if (= n 1) 1 (+ (* 2 (hanoi-moves (- n 1))) 1))))

(define collatz-steps (lambda (n)
  (if (= n 1) 0
      (if (= (modulo n 2) 0)
          (+ 1 (collatz-steps (quotient n 2)))
          (+ 1 (collatz-steps (+ (* 3 n) 1)))))))

(define tail-factorial (lambda (n acc)
  (if (= n 0) acc (tail-factorial (- n 1) (* acc n)))))
(define factorial-optimized (lambda (n) (tail-factorial n 1)))

(quote "=== FACTORIAL TESTS ===")
(factorial 5)
(factorial 6)
(factorial-optimized 5)

(quote "=== FIBONACCI TESTS ===")
(fibonacci 5)
(fibonacci 8)
(fibonacci 10)

(quote "=== GCD TESTS ===")
(gcd 48 18)
(gcd 21 14)
(gcd 17 13)

(quote "=== POWER TESTS ===")
(power-recursive 2 3)
(power-recursive 3 4)
(expt 2 3)
(expt 3 4)

(quote "=== UTILITY FUNCTIONS ===")
(count-digits 12345)
(sum-to-n 10)

(quote "=== COMPLEX RECURSION ===")
(ackermann 2 3)
(hanoi-moves 4)
(collatz-steps 3)

(exit)
