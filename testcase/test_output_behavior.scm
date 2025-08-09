;; 测试define和set!的输出行为
;; 这些应该只输出空行，不显示#<void>

;; 测试define
(define x 10)
(define y 20)

;; 显示值（应该显示数字）
x
y

;; 测试set!
(set! x 100)
(set! y 200)

;; 显示修改后的值
x
y

;; 测试列表的set-car!和set-cdr!
(define lst (cons 1 2))
lst
(set-car! lst 10)
(set-cdr! lst 20)
lst

;; 测试其他表达式仍然正常显示
(+ 1 2)
"hello"
'(a b c)

;; 退出
(exit)
