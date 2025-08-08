; 测试define功能
; 基本变量定义
(define vhello 3)
vhello

; 函数定义
(define sum3 (lambda (a b c) (+ a b c)))
(sum3 1 2 3)

; 复杂函数定义
(define factorial (lambda (n) (if (= n 0) 1 (* n (factorial (- n 1))))))
(factorial 5)

; 变量重定义
(define vhello 10)
vhello

; 测试define with lambda的简化语法(如果支持的话)
; (define (square x) (* x x))
; (square 4)

(exit)
