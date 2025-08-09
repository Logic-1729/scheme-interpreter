;; 测试宏系统
(define-syntax when
  (syntax-rules ()
    ((when test expr1 expr2 ...)
     (if test (begin expr1 expr2 ...)))))

(when #t (display "hello"))
(exit)
