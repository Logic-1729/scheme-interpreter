(define sum-of-squares (lambda (n)
  (if (= n 0)
      0
      (+ (* n n) (sum-of-squares (- n 1))))))

(define sum-of-cubes (lambda (n)
  (if (= n 0)
      0
      (+ (* n (* n n)) (sum-of-cubes (- n 1))))))

(define catalan (lambda (n)
  (if (= n 0)
      1
      (if (= n 1)
          1
          (+ (catalan (- n 1)) 
             (* (catalan (- n 1)) (catalan (- n 1))))))))

(define tribonacci (lambda (n)
  (if (< n 3)
      1
      (+ (tribonacci (- n 1)) 
         (tribonacci (- n 2)) 
         (tribonacci (- n 3))))))

(define digital-root (lambda (n)
  (if (< n 10)
      n
      (digital-root (+ (quotient n 10) (modulo n 10))))))

(sum-of-squares 3)
(sum-of-squares 4)

(sum-of-cubes 3)
(sum-of-cubes 4)

(tribonacci 4)
(tribonacci 5)
(tribonacci 6)

(digital-root 38)
(digital-root 123)
(digital-root 9876)

(define palindrome-check (lambda (n original)
  (if (= n 0)
      (= original 0)
      (if (= (modulo n 10) (modulo original 10))
          (palindrome-check (quotient n 10) (quotient original 10))
          #f))))

(define is-palindrome (lambda (n)
  (palindrome-check n n)))

(is-palindrome 121)
(is-palindrome 1221)
(is-palindrome 123)

(exit)
