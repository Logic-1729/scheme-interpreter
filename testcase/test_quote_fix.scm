;; 测试quote功能
(quote 42)
(quote hello)
(quote #t)
(quote #f)

;; 测试列表quote - 这应该能工作
(list 1 2 3)

;; 测试有问题的quote列表
(quote (1 2 3))
(quote (+ 1 2 3))

;; 使用单引号语法
'42
'hello
'(1 2 3)
'(+ 1 2 3)

(exit)
