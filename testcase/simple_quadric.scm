;; 定义简单的sqrt函数
(define (simple-sqrt n)
  (if (= n 0) 0
      (if (= n 1) 1
          (if (= n 4) 2
              (if (= n 9) 3
                  (if (= n 16) 4
                      (if (= n 25) 5
                          1)))))))  ;; 默认返回1

;; 测试
(simple-sqrt 0)
(simple-sqrt 1)
(simple-sqrt 4)
(simple-sqrt 9)
(simple-sqrt 16)
(simple-sqrt 25)

;; 现在测试二次方程
(define (simple-quadric a b c)
  (if (= a 0)
      'error
      (let ((d (- (* b b) (* 4 a c))))
        (if (< d 0)
            '()
            (if (= d 0)
                (list (/ (- 0 b) (* 2 a)))
                (let ((sqrt-d (simple-sqrt d)))
                  (let ((x1 (/ (+ (- 0 b) sqrt-d) (* 2 a)))
                        (x2 (/ (- (- 0 b) sqrt-d) (* 2 a))))
                    (list x1 x2))))))))

;; 测试 x^2 - 5x + 6 = 0, d = 25 - 24 = 1, sqrt(1) = 1
;; 解应该是 (5±1)/2 = 3, 2
(simple-quadric 1 -5 6)

(exit)
