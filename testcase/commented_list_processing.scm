;; ==========================================
;; 列表处理函数集合 - 展示注释功能
;; ==========================================

;; 将列表中的每个数字乘以2
(define (list*2 ls)
  (if (null? ls)
      (list)    ;; 空列表的情况
      (cons (* 2 (car ls))    ;; 将第一个元素乘以2
            (list*2 (cdr ls)))))  ;; 递归处理剩余元素

;; 将列表中的每个数字加1
(define (list+1 ls)
  (if (null? ls)
      (list)
      (cons (+ 1 (car ls))
            (list+1 (cdr ls)))))

;; 计算列表所有元素的和
(define (list-sum ls)
  (if (null? ls)
      0    ;; 空列表的和为0
      (+ (car ls) (list-sum (cdr ls)))))  ;; 当前元素加上剩余元素的和

;; 计算列表的长度
(define (list-length ls)
  (if (null? ls)
      0    ;; 空列表长度为0
      (+ 1 (list-length (cdr ls)))))  ;; 1加上剩余部分的长度

;; 反转列表的辅助函数
(define (reverse-helper ls acc)
  (if (null? ls)
      acc    ;; 返回累积的结果
      (reverse-helper (cdr ls) (cons (car ls) acc))))  ;; 递归调用

;; 反转列表
(define (list-reverse ls)
  (reverse-helper ls (list)))  ;; 调用辅助函数，初始累积器为空列表

;; ==========================================
;; 测试部分
;; ==========================================

;; 创建测试数据
(define test-list (list 1 2 3 4 5))  ;; 测试列表
(define empty-list (list))           ;; 空列表

;; 测试 list*2 函数
(list*2 test-list)        ;; 期望结果: (2 4 6 8 10)
(list*2 empty-list)       ;; 期望结果: ()

;; 测试 list+1 函数  
(list+1 test-list)        ;; 期望结果: (2 3 4 5 6)
(list+1 (list 0 -1 10))   ;; 期望结果: (1 0 11)

;; 测试聚合函数
(list-sum test-list)      ;; 期望结果: 15 (1+2+3+4+5)
(list-length test-list)   ;; 期望结果: 5

;; 测试反转函数
(list-reverse test-list)  ;; 期望结果: (5 4 3 2 1)
(list-reverse (list 1 2)) ;; 期望结果: (2 1)

;; 程序结束
(exit)
