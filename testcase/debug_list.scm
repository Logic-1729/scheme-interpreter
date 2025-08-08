;; 测试基本的列表操作
(define test-list '(1 2 3))
test-list
(car test-list)
(cdr test-list)
(null? test-list)
(null? '())

;; 测试简单的递归函数
(define (list-length ls)
  (if (null? ls)
      0
      (+ 1 (list-length (cdr ls)))))

(list-length '())
(list-length '(1))
(list-length '(1 2))

(exit)
