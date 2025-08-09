(define (test-short-circuit-and)
  (begin
    (define x 100)
    (and #f (define x 200))
    x))
(test-short-circuit-and)
(exit)
