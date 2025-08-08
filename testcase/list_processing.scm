;; 列表处理函数集合

;; 将列表中的每个数字乘以2
(define (list*2 ls)
  (if (null? ls)
      '()
      (cons (* 2 (car ls))
            (list*2 (cdr ls)))))

;; 将列表中的每个数字加1
(define (list+1 ls)
  (if (null? ls)
      '()
      (cons (+ 1 (car ls))
            (list+1 (cdr ls)))))

;; 将列表中的每个数字平方
(define (list-square ls)
  (if (null? ls)
      '()
      (cons (* (car ls) (car ls))
            (list-square (cdr ls)))))

;; 将列表中的每个数字取负值
(define (list-negate ls)
  (if (null? ls)
      '()
      (cons (- 0 (car ls))
            (list-negate (cdr ls)))))

;; 计算列表的长度
(define (list-length ls)
  (if (null? ls)
      0
      (+ 1 (list-length (cdr ls)))))

;; 计算列表所有元素的和
(define (list-sum ls)
  (if (null? ls)
      0
      (+ (car ls) (list-sum (cdr ls)))))

;; 计算列表所有元素的积
(define (list-product ls)
  (if (null? ls)
      1
      (* (car ls) (list-product (cdr ls)))))

;; 找到列表中的最大值
(define (list-max ls)
  (if (null? ls)
      'error
      (if (null? (cdr ls))
          (car ls)
          (let ((rest-max (list-max (cdr ls))))
            (if (> (car ls) rest-max)
                (car ls)
                rest-max)))))

;; 找到列表中的最小值
(define (list-min ls)
  (if (null? ls)
      'error
      (if (null? (cdr ls))
          (car ls)
          (let ((rest-min (list-min (cdr ls))))
            (if (< (car ls) rest-min)
                (car ls)
                rest-min)))))

;; 反转列表
(define (list-reverse ls)
  (define (reverse-helper ls acc)
    (if (null? ls)
        acc
        (reverse-helper (cdr ls) (cons (car ls) acc))))
  (reverse-helper ls '()))

;; 过滤出正数
(define (filter-positive ls)
  (if (null? ls)
      '()
      (if (> (car ls) 0)
          (cons (car ls) (filter-positive (cdr ls)))
          (filter-positive (cdr ls)))))

;; 过滤出偶数
(define (filter-even ls)
  (if (null? ls)
      '()
      (if (= (modulo (car ls) 2) 0)
          (cons (car ls) (filter-even (cdr ls)))
          (filter-even (cdr ls)))))

;; 检查列表是否包含某个元素
(define (list-contains? ls x)
  (if (null? ls)
      #f
      (if (= (car ls) x)
          #t
          (list-contains? (cdr ls) x))))

;; 获取列表的第n个元素（从0开始）
(define (list-ref ls n)
  (if (null? ls)
      'error
      (if (= n 0)
          (car ls)
          (list-ref (cdr ls) (- n 1)))))

;; 连接两个列表
(define (list-append ls1 ls2)
  (if (null? ls1)
      ls2
      (cons (car ls1) (list-append (cdr ls1) ls2))))

;; 测试用例
(define test-list '(1 2 3 4 5))
(define test-list2 '(-2 -1 0 1 2 3))
(define empty-list '())

;; 测试 list*2
(list*2 test-list)        ;; 应该返回 (2 4 6 8 10)
(list*2 empty-list)       ;; 应该返回 ()

;; 测试 list+1
(list+1 test-list)        ;; 应该返回 (2 3 4 5 6)
(list+1 '(0 -1 10))       ;; 应该返回 (1 0 11)

;; 测试 list-square
(list-square test-list)   ;; 应该返回 (1 4 9 16 25)
(list-square '(-2 -1 2))  ;; 应该返回 (4 1 4)

;; 测试 list-negate
(list-negate test-list)   ;; 应该返回 (-1 -2 -3 -4 -5)
(list-negate test-list2)  ;; 应该返回 (2 1 0 -1 -2 -3)

;; 测试长度、和、积
(list-length test-list)   ;; 应该返回 5
(list-sum test-list)      ;; 应该返回 15
(list-product test-list)  ;; 应该返回 120

;; 测试最大值和最小值
(list-max test-list)      ;; 应该返回 5
(list-min test-list)      ;; 应该返回 1
(list-max test-list2)     ;; 应该返回 3
(list-min test-list2)     ;; 应该返回 -2

;; 测试反转
(list-reverse test-list)  ;; 应该返回 (5 4 3 2 1)
(list-reverse '(1 2))     ;; 应该返回 (2 1)

;; 测试过滤
(filter-positive test-list2)  ;; 应该返回 (1 2 3)
(filter-even test-list)       ;; 应该返回 (2 4)

;; 测试包含
(list-contains? test-list 3)  ;; 应该返回 #t
(list-contains? test-list 6)  ;; 应该返回 #f

;; 测试索引访问
(list-ref test-list 0)    ;; 应该返回 1
(list-ref test-list 2)    ;; 应该返回 3
(list-ref test-list 4)    ;; 应该返回 5

;; 测试列表连接
(list-append '(1 2) '(3 4))     ;; 应该返回 (1 2 3 4)
(list-append test-list '(6 7))  ;; 应该返回 (1 2 3 4 5 6 7)

(exit)
