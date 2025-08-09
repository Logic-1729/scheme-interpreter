;; 综合测试 display 功能

;; 测试基本类型显示
(display "=== 基本类型测试 ===\n")
(display "整数: ") (display 42) (display "\n")
(display "布尔值: ") (display #t) (display " ") (display #f) (display "\n")
(display "符号: ") (display 'hello) (display "\n")
(display "空值: ") (display '()) (display "\n")

;; 测试字符串和转义字符
(display "=== 字符串测试 ===\n")
(display "普通字符串: Hello, World!\n")
(display "带引号: Say \"Hello\" to me\n")
(display "制表符:\tTab\there\n")
(display "反斜杠: C:\\path\\to\\file\n")

;; 测试复合数据类型
(display "=== 复合类型测试 ===\n")
(define lst '(1 2 3 4 5))
(display "列表: ") (display lst) (display "\n")

(define p (cons 'a 'b))
(display "序对: ") (display p) (display "\n")

;; 测试函数显示
(display "=== 函数测试 ===\n")
(define (square x) (* x x))
(display "函数 square: ") (display square) (display "\n")

;; 测试表达式计算结果的显示
(display "=== 计算结果测试 ===\n")
(display "2 + 3 = ") (display (+ 2 3)) (display "\n")
(display "5 * 6 = ") (display (* 5 6)) (display "\n")

;; 测试循环显示
(display "=== 循环显示测试 ===\n")
(define (print-numbers n)
  (if (> n 0)
      (begin
        (display n) (display " ")
        (print-numbers (- n 1)))
      (display "\n")))

(display "倒数: ")
(print-numbers 10)

;; 测试格式化输出
(display "=== 格式化输出测试 ===\n")
(define name "Alice")
(define age 25)
(display "用户信息:\n")
(display "  姓名: ") (display name) (display "\n")
(display "  年龄: ") (display age) (display " 岁\n")

(display "测试完成!\n")
(exit)
