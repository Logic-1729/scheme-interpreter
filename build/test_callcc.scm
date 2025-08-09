;; 测试call/cc (call-with-current-continuation)
(call/cc (lambda (k) (+ 2 (k 3))))
(exit)
