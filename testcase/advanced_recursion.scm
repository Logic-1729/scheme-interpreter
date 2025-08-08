(define hanoi-moves (lambda (n)
  (if (= n 1)
      1
      (+ (* 2 (hanoi-moves (- n 1))) 1))))

(define collatz-steps (lambda (n)
  (if (= n 1)
      0
      (if (= (modulo n 2) 0)
          (+ 1 (collatz-steps (quotient n 2)))
          (+ 1 (collatz-steps (+ (* 3 n) 1)))))))

(define binary-search-depth (lambda (n)
  (if (= n 1)
      1
      (+ 1 (binary-search-depth (quotient n 2))))))

(define perfect-square-check (lambda (n guess)
  (if (= (* guess guess) n)
      guess
      (if (> (* guess guess) n)
          0
          (perfect-square-check n (+ guess 1))))))

(define is-perfect-square (lambda (n) 
  (if (= (perfect-square-check n 1) 0) #f #t)))

(hanoi-moves 3)
(hanoi-moves 4)

(collatz-steps 1)
(collatz-steps 2)
(collatz-steps 3)

(binary-search-depth 8)
(binary-search-depth 16)

(is-perfect-square 9)
(is-perfect-square 10)
(is-perfect-square 16)

(define tail-factorial (lambda (n acc)
  (if (= n 0)
      acc
      (tail-factorial (- n 1) (* acc n)))))

(define factorial-tail (lambda (n) (tail-factorial n 1)))

(factorial-tail 5)
(factorial-tail 6)

(exit)
