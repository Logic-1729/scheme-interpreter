(define is-even (lambda (n)
  (if (= n 0)
      #t
      (is-odd (- n 1)))))

(define is-odd (lambda (n)
  (if (= n 0)
      #f
      (is-even (- n 1)))))

(is-even 4)
(is-odd 4)
(is-even 7)
(is-odd 7)
(is-even 0)
(is-odd 1)

(exit)
