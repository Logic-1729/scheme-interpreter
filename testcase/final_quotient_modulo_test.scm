;; 完整的 quotient 和 modulo 测试

;; 基本功能测试
(quotient 13 4)     ;; → 3
(quotient -13 4)    ;; → -3  
(quotient 13 -4)    ;; → -3
(quotient -13 -4)   ;; → 3

(modulo 13 4)       ;; → 1
(modulo -13 4)      ;; → 3 (与除数同号)
(modulo 13 -4)      ;; → -3 (与除数同号)  
(modulo -13 -4)     ;; → -1 (与除数同号)

;; 边界情况
(quotient 0 5)      ;; → 0
(modulo 0 5)        ;; → 0

;; 一些常用例子
(quotient 7 3)      ;; → 2
(modulo 7 3)        ;; → 1

(quotient -7 3)     ;; → -2
(modulo -7 3)       ;; → 2 (与除数同号)

(exit)
