;; 简化的列表处理函数测试

;; 将列表中的每个数字乘以2
(define (list*2 ls)
  (if (null? ls)
      (list)
      (cons (* 2 (car ls))
            (list*2 (cdr ls)))))

;; 测试用例
(define test-list (list 1 2 3))

;; 测试 list*2
(list*2 test-list)        ;; 应该返回 (2 4 6)
(list*2 (list))           ;; 应该返回 ()

(exit)
