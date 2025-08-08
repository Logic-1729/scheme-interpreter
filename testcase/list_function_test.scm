(list)

(list 1)

(list 1 2)

(list 1 2 3 4 5)

(list 0)

(define (make-range from to)
  (if (> from to)
      (list)
      (cons from (make-range (+ from 1) to))))

(make-range 1 5)

(list (+ 1 2) (* 3 4) (/ 10 2))

(define nums (list 10 20 30))
nums
(car nums)
(cdr nums)

(list #t #f)

(list (list 1 2) (list 3 4))

(define (first-n-squares n)
  (define (helper i acc)
    (if (> i n)
        acc
        (helper (+ i 1) (cons (* i i) acc))))
  (helper 1 (list)))

(first-n-squares 4)

(exit)
