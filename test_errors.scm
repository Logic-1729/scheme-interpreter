;; 测试错误情况

;; 测试 set! 对未定义变量的处理
;; (set! undefined_var 123)  ;; 这应该报错

;; 测试 set-car! 和 set-cdr! 对非pair的处理
(set-car! 123 456)

(exit)