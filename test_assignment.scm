;; 测试 set!、set-car! 和 set-cdr! 的实现

;; 测试 set!
(define x 10)
x

(set! x 20)
x

(define y 100)
y

(set! y (+ y 50))
y

;; 测试 set-car!
(define lst (cons 1 2))
lst

(set-car! lst 99)
lst

(define pair1 (cons 'a 'b))
pair1

(set-car! pair1 'hello)
pair1

;; 测试 set-cdr!
(define lst2 (cons 1 2))
lst2

(set-cdr! lst2 88)
lst2

(define pair2 (cons 'x 'y))
pair2

(set-cdr! pair2 'world)
pair2

;; 测试复杂情况
(define nested (cons (cons 1 2) (cons 3 4)))
nested

(set-car! (car nested) 'first)
nested

(set-cdr! (cdr nested) 'last)
nested

;; 测试错误情况（应该抛出错误）
;; (set-car! 123 456)  ;; 这会报错
;; (set-cdr! 'symbol 'value)  ;; 这也会报错

(exit)
