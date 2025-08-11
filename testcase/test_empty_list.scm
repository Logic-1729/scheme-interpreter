;; 测试 '() 的运行过程
(display "Testing '() evaluation process:")

;; 1. 基本的空列表引用
'()

;; 2. 等价形式
(quote ())

;; 3. 检查结果类型
(null? '())
(pair? '())

;; 4. 与其他引用形式对比
'(1 2 3)
'a
'#t

;; 5. 空列表的操作
(cons 1 '())
(append '() '(1 2 3))
(length '())

(exit)
