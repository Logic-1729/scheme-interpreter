;; 定义整数平方根函数（下取整）
(define (sqrt n)
  (if (< n 0)
      'error
      (if (= n 0)
          0
          (if (= n 1)
              1
              (letrec ((sqrt-helper (lambda (guess)
                                      (if (<= (* guess guess) n)
                                          (if (> (* (+ guess 1) (+ guess 1)) n)
                                              guess
                                              (sqrt-helper (+ guess 1)))
                                          (sqrt-helper (- guess 1))))))
                (sqrt-helper 1))))))

;; 测试sqrt函数
(sqrt 0)
(sqrt 1)
(sqrt 4)
(sqrt 9)
(sqrt 16)
(sqrt 15)
(sqrt 8)
(sqrt 25)

;; 定义二次方程求解函数
(define (quadric-equation a b c)
  (if (= 0 a)
      'error
      (let ((d (- (* b b) (* 4 a c))))
        (if (< d 0)
            '()
            (let ((e (/ (- 0 b) (* 2 a))))
              (if (= 0 d)
                  (list e)
                  (let ((f (/ (sqrt d) (* 2 a))))
                    (list (+ e f) (- e f)))))))))

;; 测试二次方程
(quadric-equation 1 -5 6)  ;; x^2 - 5x + 6 = 0, 应该有解 x=2,3
(quadric-equation 1 0 -4)  ;; x^2 - 4 = 0, 应该有解 x=±2
(quadric-equation 1 0 1)   ;; x^2 + 1 = 0, 应该无实数解

(exit)
