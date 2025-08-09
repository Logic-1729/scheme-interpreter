;; 测试银行账户的闭包实现
(define bank-account (let ((balance 10)) (lambda (n) (set! balance (+ balance n)) balance)))

;; 测试使用
(bank-account 5)
(bank-account -3)
(bank-account 0)

(exit)
