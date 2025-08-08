(number? 42)
(number? -17)
(number? 0)
(number? #t)
(number? #f)
(number? 'symbol)
(number? (list 1 2))

(list? (list))
(list? (list 1))
(list? (list 1 2 3))
(list? 42)
(list? #t)
(list? 'symbol)

(list? (cons 1 2))
(list? (cons 1 (list)))
(list? (cons 1 (cons 2 (list))))
(list? (cons 1 (cons 2 (cons 3 (list)))))

(define nums (list 1 2 3 4))
(list? nums)
(number? (car nums))

(define (all-numbers? lst)
  (if (list? lst)
      (if (null? lst)
          #t
          (if (number? (car lst))
              (all-numbers? (cdr lst))
              #f))
      #f))

(all-numbers? (list 1 2 3))
(all-numbers? (list 1 #t 3))
(all-numbers? 42)

(exit)
