;; 简单测试除法
(/ 10 2)
(/ (- 0 10) 2)
(* 2 3)

;; 测试二次方程的各个部分
(define a 1)
(define b -5)
(define c 6)

;; 判别式 d = b^2 - 4ac
(define d (- (* b b) (* 4 a c)))
d

;; 如果d>=0，计算解
(if (>= d 0)
    (let ((sqrt-d 1))  ;; 假设sqrt(d)=1来测试
      (let ((e (/ (- 0 b) (* 2 a))))
        (let ((f (/ sqrt-d (* 2 a))))
          (list (+ e f) (- e f)))))
    '())

(exit)
