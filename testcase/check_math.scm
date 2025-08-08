;; 验证数学性质：q*b + r = a，其中 q = quotient(a,b), r = modulo(a,b)

;; 对于 -13, 4: quotient = -3, modulo = 3
;; 验证: -3 * 4 + 3 = -12 + 3 = -9 ≠ -13

;; 让我测试一下具体数值
(quotient -13 4)  ;; 应该是 -3
(modulo -13 4)    ;; 应该是 3 
(+ (* (quotient -13 4) 4) (modulo -13 4))  ;; 这应该等于 -13

(exit)
