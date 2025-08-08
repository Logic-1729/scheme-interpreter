;; 测试各种void情况
(void)                    ;; 显式void调用
(define a 5)              ;; define不应显示
(define (square x) (* x x))  ;; 函数定义不应显示
(begin)                   ;; 空begin可能产生void
(begin (void))            ;; begin包含void
(if #t (void) 1)          ;; 条件中的void

(exit)
