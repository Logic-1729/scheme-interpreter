;; 测试重命名后的类型系统

;; 测试基本数值
42
-17

;; 测试布尔值
#t
#f

;; 测试符号
hello
world

;; 测试 pair 和 null
(cons 1 2)
()

;; 测试 procedure
(lambda (x) (+ x 1))

;; 测试算术运算
(+ 1 2 3)
(* 2 3 4)
(- 10 3 2)
(/ 12 3 2)

;; 测试比较
(< 1 2)
(= 5 5)

;; 测试 quote 和单引号
'hello
'(1 2 3)
(quote world)

;; 测试 quotient 和 modulo
(quotient 13 4)
(modulo 13 4)

(exit)
