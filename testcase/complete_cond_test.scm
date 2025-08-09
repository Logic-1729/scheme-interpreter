;; 完整的 cond 表达式测试
;; 包括城市游泳池收费函数

;; 原始的收费函数（简化版，用整数表示费用）
(define (fee-simple age)
  (cond
   ((or (<= age 3) (>= age 65)) 0)
   ((<= 4 age 6) 50)      ;; 0.5美元 → 50分
   ((<= 7 age 12) 100)    ;; 1.0美元 → 100分
   ((<= 13 age 15) 150)   ;; 1.5美元 → 150分
   ((<= 16 age 18) 180)   ;; 1.8美元 → 180分
   (else 200)))           ;; 2.0美元 → 200分

;; 测试收费函数
(fee-simple 2)    ;; 应该是 0 (3岁以下免费)
(fee-simple 5)    ;; 应该是 50 (4-6岁)
(fee-simple 10)   ;; 应该是 100 (7-12岁)
(fee-simple 14)   ;; 应该是 150 (13-15岁)
(fee-simple 17)   ;; 应该是 180 (16-18岁)
(fee-simple 25)   ;; 应该是 200 (其他年龄)
(fee-simple 70)   ;; 应该是 0 (65岁以上免费)

;; 测试成绩等级函数
(define (grade score)
  (cond
   ((>= score 90) 4)  ;; A
   ((>= score 80) 3)  ;; B
   ((>= score 70) 2)  ;; C
   ((>= score 60) 1)  ;; D
   (else 0)))         ;; F

(grade 95)  ;; 应该是 4 (A)
(grade 85)  ;; 应该是 3 (B)
(grade 75)  ;; 应该是 2 (C)
(grade 65)  ;; 应该是 1 (D)
(grade 50)  ;; 应该是 0 (F)

;; 测试没有匹配条件的 cond
(define (impossible-test)
  (cond
   ((< 5 3) 1)
   ((> 2 10) 2)))

(impossible-test)  ;; 应该返回 void（未定义）

;; 测试简单的数值比较
(cond
 ((> 10 5) 100)
 (else 0))

;; 测试布尔值
(cond
 (#f 1)
 (#t 2))

(exit)
