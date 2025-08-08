;; 验证 quotient 和 modulo 的数学性质
;; (quotient a b) * b + (modulo a b) = a
(+ (* (quotient 13 4) 4) (modulo 13 4))  
(+ (* (quotient -13 4) 4) (modulo -13 4))  
(+ (* (quotient 13 -4) -4) (modulo 13 -4)) 
(+ (* (quotient -13 -4) -4) (modulo -13 -4)) 
(exit)
