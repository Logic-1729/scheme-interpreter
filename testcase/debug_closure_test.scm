;; 详细测试环境和闭包
(define test-closure
  (let ((x 100))
    (lambda ()
      x)))

;; 测试闭包是否能读取变量
(test-closure)

;; 现在测试 set!
(define test-set-closure
  (let ((y 200))
    (lambda ()
      (set! y 300)
      y)))

;; 测试修改
(test-set-closure)

(exit)
