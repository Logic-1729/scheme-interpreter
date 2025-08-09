;; 测试交叉引用和内部定义

;; 测试1: 在begin块中的内部定义
(begin
  (define x 10)
  (define y 20)
  (define (add-xy) (+ x y))
  (add-xy))

;; 测试2: 交叉引用的函数定义
(begin
  (define (even? n)
    (if (= n 0) 
        #t 
        (odd? (- n 1))))
  
  (define (odd? n)
    (if (= n 0) 
        #f 
        (even? (- n 1))))
  
  (even? 4))

;; 测试3: 内部定义的作用域
(begin
  (define inner-var 100)
  (begin
    (define inner-var 200)
    inner-var))

;; 测试4: 复杂的相互递归
(begin
  (define (fib n)
    (if (<= n 1)
        n
        (+ (fib (- n 1)) (fib (- n 2)))))
  
  (define (factorial n)
    (if (<= n 1)
        1
        (* n (factorial (- n 1)))))
  
  (list (fib 6) (factorial 5)))

(exit)
