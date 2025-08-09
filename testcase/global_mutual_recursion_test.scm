;; 测试全局级别的延迟 define

;; 全局级别的相互递归函数
(define (global-even n)
  (if (= n 0)
      #t
      (global-odd (- n 1))))

(define (global-odd n)
  (if (= n 0)
      #f
      (global-even (- n 1))))

;; 测试全局相互递归
(global-even 6)
(global-odd 6)
(global-even 7)
(global-odd 7)

;; 混合一些其他定义
(define (square x) (* x x))
(define (cube x) (* x x x))

;; 使用这些函数
(square 4)
(cube 3)

;; 测试表达式来触发 define 处理
"All tests completed!"

(exit)
