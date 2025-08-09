;; 测试 display 函数

;; 测试数字显示
(display 42)
(display 0)

;; 测试字符串显示 - 应该不显示引号
(display "Hello, World!")
(display "")

;; 测试其他类型
(display #t)
(display #f)
(display 'symbol)
(display '())
(display '(1 2 3))

;; 测试组合使用
(begin 
  (display "The answer is: ")
  (display 42)
  (display "\n"))

;; 测试不同的值类型
(define x 100)
(display "x = ")
(display x)

(define p (cons 1 2))
(display "\nPair: ")
(display p)

(exit)
