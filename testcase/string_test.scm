;; 测试 string? 函数
(string? "hello")        ;; 应该返回 #t
(string? 123)            ;; 应该返回 #f  
(string? #t)             ;; 应该返回 #f
(string? 'symbol)        ;; 应该返回 #f
(string? '())            ;; 应该返回 #f
(string? (cons 1 2))     ;; 应该返回 #f

;; 在 cond 中使用 string?
(define (describe-value x)
  (cond
   ((string? x) "It's a string")
   ((number? x) "It's a number")
   ((boolean? x) "It's a boolean")
   ((symbol? x) "It's a symbol")
   ((null? x) "It's null")
   ((pair? x) "It's a pair")
   (else "Unknown type")))

(describe-value "hello")
(describe-value 42)
(describe-value #t)
(describe-value 'foo)
(describe-value '())
(describe-value (cons 1 2))

;; 测试空字符串
(string? "")

;; 测试带转义字符的字符串
(string? "hello\nworld")

(exit)
