;; 全面测试quote功能

;; 测试基本数据类型的quote
(quote 42)        ;; 数字
(quote -17)       ;; 负数
(quote hello)     ;; 符号
(quote #t)        ;; 布尔值真
(quote #f)        ;; 布尔值假

;; 测试列表的quote
(quote ())               ;; 空列表
(quote (1))              ;; 单元素列表
(quote (1 2 3))          ;; 多元素列表
(quote (+ 1 2 3))        ;; 不求值的表达式
(quote (define x 10))    ;; 不求值的定义
(quote (if #t 1 2))      ;; 不求值的条件表达式

;; 测试嵌套列表的quote
(quote (1 (2 3) 4))      ;; 嵌套列表
(quote ((+ 1 2) (- 3 4))) ;; 嵌套表达式

;; 测试单引号语法（应该等价于quote）
'42              ;; 等价于 (quote 42)
'hello           ;; 等价于 (quote hello)
'#t              ;; 等价于 (quote #t)
'#f              ;; 等价于 (quote #f)
'()              ;; 等价于 (quote ())
'(1 2 3)         ;; 等价于 (quote (1 2 3))
'(+ 1 2 3)       ;; 等价于 (quote (+ 1 2 3))
'(1 (2 3) 4)     ;; 等价于 (quote (1 (2 3) 4))

;; 对比：quote vs 求值
(+ 1 2 3)        ;; 求值，应该得到 6
(quote (+ 1 2 3)) ;; 不求值，应该得到 (+ 1 2 3)
'(+ 1 2 3)       ;; 不求值，应该得到 (+ 1 2 3)

;; 测试quote的列表操作
(car '(1 2 3))   ;; 应该得到 1
(cdr '(1 2 3))   ;; 应该得到 (2 3)
(cons 'a '(b c)) ;; 应该得到 (a b c)

(exit)
