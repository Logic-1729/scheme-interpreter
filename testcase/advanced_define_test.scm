;; 更复杂的交叉引用和内部定义测试

;; 测试1: 全局级别的相互递归
(define (is-even n)
  (cond
   ((= n 0) #t)
   (else (is-odd (- n 1)))))

(define (is-odd n)
  (cond
   ((= n 0) #f)
   (else (is-even (- n 1)))))

;; 测试相互递归
(is-even 6)
(is-odd 6)
(is-even 7)
(is-odd 7)

;; 测试2: 复杂的内部定义 - 多个函数相互调用
(define (process-list n)
  (begin
    (define (double-if-even x)
      (cond
       ((is-zero x) 0)
       ((even-helper x) (* 2 x))
       (else x)))
    
    (define (even-helper x)
      (= (modulo x 2) 0))
    
    (define (is-zero x)
      (= x 0))
    
    (define (apply-to-range start end)
      (cond
       ((> start end) 0)
       (else (+ (double-if-even start) 
                (apply-to-range (+ start 1) end)))))
    
    ;; 使用内部定义的函数
    (apply-to-range 1 n)))

(process-list 5)

;; 测试3: 嵌套的内部定义
(define (nested-defines x)
  (begin
    (define (outer-func n)
      (begin
        (define (inner-func m)
          (cond
           ((= m 0) 1)
           (else (* m (inner-func (- m 1))))))
        
        (define multiplier 2)
        (* multiplier (inner-func n))))
    
    (define base 3)
    (+ base (outer-func x))))

(nested-defines 4)

;; 测试4: 在函数中使用内部定义
(define (fibonacci n)
  (begin
    (define (fib-helper a b count)
      (cond
       ((= count 0) b)
       (else (fib-helper (+ a b) a (- count 1)))))
    
    (cond
     ((= n 0) 0)
     ((= n 1) 1)
     (else (fib-helper 1 0 n)))))

(fibonacci 0)
(fibonacci 1)
(fibonacci 5)
(fibonacci 10)

;; 测试5: 内部定义中的递归
(define (tree-sum depth)
  (begin
    (define (make-tree d)
      (cond
       ((= d 0) 1)
       (else (+ 1 
                (make-tree (- d 1))
                (make-tree (- d 1))))))
    
    (define (sum-nodes d)
      (cond
       ((= d 0) 1)
       (else (+ 1 
                (* 2 (sum-nodes (- d 1)))))))
    
    ;; 使用简化版本避免指数复杂度
    (sum-nodes depth)))

(tree-sum 3)

(exit)
