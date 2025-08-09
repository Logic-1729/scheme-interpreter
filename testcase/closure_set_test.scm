;; 测试在闭包中使用 set!
((let ((x 10))
   (lambda () 
     (set! x 20)
     x)))

(exit)
