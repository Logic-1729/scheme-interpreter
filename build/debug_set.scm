(define x 10)
(let ((y 20))
  (set! x 30)  ; 全局变量
  (set! y 40)  ; let绑定变量
  (list x y))
(exit)
