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

;; 测试3: 简单的递归
(begin
  (define (factorial n)
    (if (<= n 1)
        1
        (* n (factorial (- n 1)))))
  
  (factorial 5))

;; 测试4: 内部定义后的表达式
(begin
  (define helper 42)
  (define (use-helper) helper)
  (use-helper))

(exit)
