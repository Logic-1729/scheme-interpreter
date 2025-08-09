;; 测试正确格式的 let set!
(let ((x 10))
  (begin
    (set! x 20)
    x))

(exit)
