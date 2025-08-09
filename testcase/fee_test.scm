;; 测试游泳池收费函数（使用数字）
(define (fee age)
  (cond
   ((or (<= age 3) (>= age 65)) 0)
   ((<= 4 age 6) 1)
   ((<= 7 age 12) 2)
   ((<= 13 age 15) 3)
   ((<= 16 age 18) 4)
   (else 5)))

;; 测试不同年龄
(fee 2)
(fee 5)
(fee 10)
(fee 14)
(fee 17)
(fee 25)
(fee 70)

(exit)
