;; 测试尾递归优化
(define (factorial n acc)
  (if (<= n 1)
      acc
      (factorial (- n 1) (* n acc))))

(factorial 1000 1)
(exit)
