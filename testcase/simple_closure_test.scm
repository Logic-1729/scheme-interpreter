;; 测试简单的闭包
(let ((x 10))
  (lambda () x))

;; 如果上面有问题，测试一个函数调用
((let ((x 10))
   (lambda () x)))

(exit)
