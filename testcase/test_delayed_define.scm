;; 测试延迟 define 和相互递归

;; 测试简单的相互递归
(define (is-even n)
  (if (= n 0)
      #t
      (is-odd (- n 1))))

(define (is-odd n)
  (if (= n 0)
      #f
      (is-even (- n 1))))

;; 测试这些函数
(is-even 4)
(is-odd 4)
(is-even 5)
(is-odd 5)

;; 测试更复杂的相互递归
(define (factorial n)
  (if (<= n 1)
      1
      (* n (factorial (- n 1)))))

;; 这个应该仍然工作（简单递归）
(factorial 5)

;; 现在测试一个简单的表达式来触发 define 处理
42

(exit)
