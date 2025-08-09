;; 测试带条件的 cond 表达式
(cond
 (#f "false branch")
 (#t "true branch")
 (else "else branch"))

(exit)
