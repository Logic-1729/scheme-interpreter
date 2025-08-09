;; 全面测试各种类型检查函数
(define test-values 
  (list "string" 42 #t 'symbol '() (cons 1 2) (lambda (x) x)))

;; 但由于这个解释器可能不支持 list 和 lambda 在这个上下文，我们分别测试

;; 测试字符串
(string? "hello world")
(number? "hello world")
(boolean? "hello world") 
(symbol? "hello world")
(null? "hello world")
(pair? "hello world")

;; 测试数字
(string? 42)
(number? 42)
(boolean? 42)
(symbol? 42)
(null? 42)
(pair? 42)

;; 测试布尔值
(string? #t)
(number? #t)
(boolean? #t)
(symbol? #t)
(null? #t)
(pair? #t)

;; 测试符号
(string? 'hello)
(number? 'hello)
(boolean? 'hello)
(symbol? 'hello)
(null? 'hello)
(pair? 'hello)

;; 测试 null
(string? '())
(number? '())
(boolean? '())
(symbol? '())
(null? '())
(pair? '())

;; 测试 pair
(string? (cons 1 2))
(number? (cons 1 2))
(boolean? (cons 1 2))
(symbol? (cons 1 2))
(null? (cons 1 2))
(pair? (cons 1 2))

;; 使用 string? 在条件表达式中
(cond
 ((string? "test") "Found a string!")
 (else "Not a string"))

(cond
 ((string? 123) "Found a string!")
 (else "Not a string"))

(exit)
