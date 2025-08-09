;; 测试 define，set!，set-car!，set-cdr! 的输出行为

;; define 不显示返回值
(define x 10)
;; 正常表达式显示返回值
x

;; set! 不显示返回值
(set! x 20)
;; 正常表达式显示返回值
x

;; pair 操作
(define p (cons 1 2))
p

;; set-car! 不显示返回值
(set-car! p 99)
;; 正常表达式显示返回值
p

;; set-cdr! 不显示返回值  
(set-cdr! p 88)
;; 正常表达式显示返回值
p

;; 其他函数仍然显示返回值
(+ 1 2)
(car p)
(cdr p)

(exit)
