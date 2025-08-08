(define sum-gp (lambda (a0 r n)
  (* a0
     (if (= r 1)
         n
         (/ (- 1 (expt r n)) (- 1 r))))))

(sum-gp 2 3 4)
(sum-gp 1 2 5)
(sum-gp 5 1 10)

(exit)
