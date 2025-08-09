;; 测试 cond 表达式
;; 城市游泳池的收费函数
(define (fee age)
  (cond
   ((or (<= age 3) (>= age 65)) 0)
   ((<= 4 age 6) 0.5)
   ((<= 7 age 12) 1.0)
   ((<= 13 age 15) 1.5)
   ((<= 16 age 18) 1.8)
   (else 2.0)))

;; 测试不同年龄的收费
(fee 2)   ; 应该返回 0
(fee 5)   ; 应该返回 0.5  
(fee 10)  ; 应该返回 1.0
(fee 14)  ; 应该返回 1.5
(fee 17)  ; 应该返回 1.8
(fee 25)  ; 应该返回 2.0
(fee 70)  ; 应该返回 0

;; 测试简单的 cond
(cond
  (#f "false branch")
  (#t "true branch")
  (else "else branch"))

;; 测试只有 else 的 cond
(cond
  (else "only else"))

;; 测试空的 cond (应该返回 void)
(cond)
(exit)