;; 全面测试延迟 define 功能

;; 测试1：简单的相互递归（偶数/奇数检查）
(define (is-even n)
  (cond
   ((= n 0) #t)
   ((< n 0) (is-even (- n)))
   (else (is-odd (- n 1)))))

(define (is-odd n)
  (cond
   ((= n 0) #f)
   ((< n 0) (is-odd (- n)))
   (else (is-even (- n 1)))))

;; 测试这些函数
(is-even 10)
(is-odd 10)
(is-even 15)
(is-odd 15)

;; 测试2：三个函数的相互递归
(define (func-a n)
  (cond
   ((<= n 0) 1)
   (else (+ 1 (func-b (- n 1))))))

(define (func-b n)
  (cond
   ((<= n 0) 2)
   (else (+ 2 (func-c (- n 1))))))

(define (func-c n)
  (cond
   ((<= n 0) 3)
   (else (+ 3 (func-a (- n 1))))))

;; 测试这些函数
(func-a 3)
(func-b 3)
(func-c 3)

;; 测试3：混合普通表达式来触发 define 处理
"Testing mixed expressions"

;; 测试4：更多 define
(define (double x) (* x 2))
(define (triple x) (* x 3))

;; 使用这些新函数
(double 5)
(triple 5)

;; 最终测试表达式
(+ (double 10) (triple 4))

(exit)
