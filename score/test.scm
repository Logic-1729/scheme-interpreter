;; every (valid) pass runs multiple times, by the interpreter and your
;; compiler. so make sure each time they yield same results when adding your own
;; tests (especially set-car! and set-cdr!).

(define tests
  '(7
    '()
    #f
    '(1 2 3 4)
    '(#(#t #f 1) #(#f #t 2))
    (+ 4 5)
    (- 1 4)
    (* 7 9)
    (cons 1 '())
    (car '(1 2))
    (cdr '(1 2))
    (if #t 1 2)
    (pair? '(1 2))
    (pair? '())
    (boolean? #f)
    (boolean? 7)
    (null? '())
    (null? '(1 2))
    (fixnum? 1234)
    (fixnum? '())
    (procedure? (lambda (x) x))
    (procedure? 7)
    (<= 1 8)
    (<= 8 1)
    (<= 1 1)
    (< 8 1)
    (< 1 8)
    (= 1 1)
    (= 1 0)
    (>= 8 1)
    (>= 1 8)
    (>= 1 1)
    (> 8 1)
    (> 1 8)
    ;; value primitives in effect context
    (let ([x 5]) (* 3 x) x)
    (let ([x 5]) (+ 3 x) x)
    (let ([x 5]) (- 3 x) x)
    (let ([x (cons 1 5)]) (car x) x)
    (let ([x (cons 1 5)]) (cdr x) x)
    (let ([x 1] [y 5]) (cons x y) x)
    (begin (void) 5)
    ;; value primitives in pred
    (if (+ 3 5) '7 8)
    (if (- 3 5) '7 8)
    (if (cons 3 5) 7 8)
    (if (car (cons #t #f)) 7 8)
    (if (cdr (cons #t #f)) 7 8)
    (if (void) 7 8) 
    ;; pred prims in value
    (< 7 8)
    (let () (<= 7 8))
    (= 7 8)
    (letrec () (>= 7 8))
    (> 7 8)
    (let () (boolean? #f))
    (let ([x (cons 1 '())] [y (cons 1 '())]) (eq? x y))
    (fixnum? 7)
    (null? '())
    (letrec () (pair? (cons 1 '())))
    ;; pred prims in effect
    (letrec () (begin (< 7 8) 7))
    (begin (<= '7 '8) '7)
    (letrec () (= 7 8) 7)
    (begin (>= 7 8) 7)
    (letrec () (begin (> 7 8) 8))
    (letrec () (boolean? #f) 9)
    (letrec () 
      (let ([x (cons 1 '())] [y (cons 1 '())])
        (begin (eq? x y) 10)))
    (letrec () (begin (fixnum? 7) 10))
    (let () (null? '()) 15)
    (letrec () (pair? (cons 1 '())) 20)
    ;; effect prims in value
    ;; effect prims in pred

    (let ([x '(1 2)]) (eq? x x))
    (let ([x '(1 2)] [y '(1 2)]) (eq? x y))
    (+ (let ([x 7] [y 2])
         (if (if (= x 7) (< y 0) (<= 0 y)) 77 88))
       99)
    (if (= (+ 7 (* 2 4)) (- 20 (+ (+ 1 1) (+ (+ 1 1) 1))))
        (+ 1 (+ 1 (+ 1 (+ 1 (+ 1 10)))))
        0)
    (cons (let ([f (lambda (h v) (* h v))])
            (let ([k (lambda (x) (+ x 5))])
              (letrec ([x 15])
                (letrec ([g (lambda (x) (+ 1 x))])
                  (k (g (let ([g 3]) (f g x))))))))
          '())
    (let ([x (cons '1 '())])
      (let ([x (cons '2 x)])
        (let ([x (cons '3 x)])
          (let ([x (cons '4 x)])
            (let ([x (cons '5 x)])
              x)))))
    (let ([n 5])
      (let ([a 1])
        (let ([a (* a n)])
          (let ([n (- n 1)])
            (let ([a (* a n)])
              (let ([n (- n 1)])
                (let ([a (* a n)])
                  (let ([n (- n 1)])
                    (let ([a (* a n)])
                      a)))))))))
    (letrec ([a (lambda (u v w x) 
                  (if (= u 0) 
                      (b v w x)
                      (a (- u 1) v w x)))]
             [b (lambda (q r x)
                  (let ([p (* q r)])
                    (e (* q r) p x)))]
             [c (lambda (x) (* 5 x))]
             [e (lambda (n p x)
                  (if (= n '0) 
                      (c p)
                      (o (- n 1) p x)))]
             [o (lambda (n p x) 
                  (if (= 0 n)
                      (c x)
                      (e (- n 1) p x)))])
      (let ([x 5])
        (a 3 2 1 x)))
    ((letrec ([length (lambda (ptr)
                        (if (null? ptr) 0 (+ 1 (length (cdr ptr)))))])
       length)
     '(5 10 11 5 15))
    (letrec ([count-leaves (lambda (p)
                             (if (pair? p)
                                 (+ (count-leaves (car p))
                                    (count-leaves (cdr p)))
                                 1))])
      (count-leaves 
        (cons 
          (cons '0 (cons '0 '0))
          (cons 
            (cons (cons (cons '0 (cons '0 '0)) '0) '0)
            (cons 
              (cons (cons '0 '0) (cons '0 (cons '0 '0)))
              (cons (cons '0 '0) '0))))))
    (letrec ([add1 (lambda (n) (+ n 1))]
             [map (lambda (f ls)
                    (if (null? ls) '() (cons (f (car ls)) (map f (cdr ls)))))]
             [sum (lambda (ls)
                      (if (null? ls) 0 (+ (car ls) (sum (cdr ls)))))])
      (let ([ls '(5 4 3 2 1)])
        (let ([ls (cons '10 (cons '9 (cons '8 (cons '7 (cons '6 ls)))))])
          (sum (map add1 ls)))))
    (letrec ([list-ref (lambda (ls offset)
                           (if (= offset 0)
                               (car ls)
                               (list-ref (cdr ls) (- offset 1))))]
             [add (lambda (v w) (+ v w))]
             [sub (lambda (v w) (- v w))]
             [mult (lambda (v w) (* v w))]
             [expt (lambda (v w) (if (= w 0) 1 (* v (expt v (- w 1)))))]
             [selector (lambda (op* sel rand1 rand2)
                           (if (null? sel)
                               0
                               (cons ((list-ref op* (car sel))
                                      (car rand1) (car rand2))
                                     (selector op* (cdr sel) (cdr rand1)
                                               (cdr rand2)))))]
             [sum (lambda (ls) (if (pair? ls) (+ (car ls) (sum (cdr ls))) 0))])
      (sum (selector (cons add (cons sub (cons mult (cons expt '()))))
                     '(2 0 1 3 2) '(5 9 10 2 3) '(3 1 3 3 8))))
    (letrec ([thunk-num (lambda (n) (lambda () n))]
             [force (lambda (th) (th))]
             [add-ths (lambda (th1 th2 th3 th4)
                        (+ (+ (force th1) (force th2))
                           (+ (force th3) (force th4))))])
      (add-ths (thunk-num 5) (thunk-num 17) (thunk-num 7) (thunk-num 9)))
    (letrec ([x 7] [f (lambda () x)]) (f))
    ((lambda (y) ((lambda (f) (f (f y))) (lambda (y) y))) 4)
    (let ([double (lambda (a) (+ a a))]) (double 10))
    (let ([t #t] [f #f])
      (letrec ((even (lambda (x) (if (= x 0) t (odd (- x 1)))))
               (odd (lambda (x) (if (= x 0) f (even (- x 1))))))
        (odd 13)))
    (letrec ([remq (lambda (x ls)
                     (if (null? ls)
                         '()
                         (if (eq? (car ls) x)
                             (remq x (cdr ls))
                             (cons (car ls) (remq x (cdr ls))))))])
      (remq 3 '(3 1 3)))
    (letrec ([gcd (lambda (x y)
                    (if (= y 0) 
                        x 
                        (gcd (if (> x y) (- x y) x)
                             (if (> x y) y (- y x)))))])
      (gcd 1071 1029))
    (letrec ([sub1 (lambda (n) (- n 1))]
             [fib (lambda (n)
                    (if (= 0 n)
                        0
                        (if (= 1 n)
                            1
                            (+ (fib (sub1 n))
                               (fib (sub1 (sub1 n)))))))])
      (fib 10))
    (letrec ([ack (lambda (m n)
                    (if (= m 0)
                        (+ n 1)
                        (if (if (> m 0) (= n 0) #f)
                            (ack (- m 1) 1)
                            (ack (- m 1) (ack m (- n 1))))))])
      (ack 2 4))
    (letrec ([fib (lambda (n) 
                    (letrec ([fib (lambda (n a b)
                                    (if (= n 0)
                                        a
                                        (fib (- n 1) b (+ b a))))])
                      (fib n 0 1)))])
      (fib 5))
    ((((((lambda (x)
            (lambda (y)
              (lambda (z)
                (lambda (w)
                  (lambda (u)
                    (+ x (+ y (+ z (+ w u)))))))))
         5) 6) 7) 8) 9)
    (let ([t #t] [f #f])
      (let ([bools (cons t f)] [id (lambda (x) (if (not x) f t))])
        (letrec
          ([even (lambda (x) (if (= x 0) (id (car bools)) (odd (- x 1))))]
           [odd (lambda (y) (if (= y 0) (id (cdr bools)) (even (- y 1))))])
          (odd 5))))
    (let ([x 7] [y 4])
      (or (and (fixnum? x) (= x 4) (fixnum? y) (= y 7))
          (and (fixnum? x) (= x 7) (fixnum? y) (= y 4))))
    (letrec ([f (lambda (x) (+ 1 x))]
             [g (lambda (x) (- x 1))]
             [t (lambda (x) (- x 1))]
             [j (lambda (x) (- x 1))]
             [i (lambda (x) (- x 1))]
             [h (lambda (x) (- x 1))])
      (let ([x 80])
        (let ([a (f x)]
              [b (g x)]
              [c (h (i (j (t x))))])
          (* a (* b (+ c 0))))))
    (let ([f (lambda (x) (+ 1 x))] [g (lambda (x) (- x 1))])
      (let ([x 80])
        (let ([a (f x)]
              [b (g x)]
              [c (letrec ([h (lambda (x) (- x 1))])
                   (h (letrec ([i (lambda (x) (- x 1))])
                        (i
                          (letrec ([t (lambda (x) (- x 1))]
                                   [j (lambda (x) (- x 1))])
                            (j (t x)))))))])
          (* a (* b (+ c 0))))))
    (letrec ([fact (lambda (n)
                     (if (= n 0)
                         1
                         (let ([t (- n 1)])
                           (let ([t (fact t)])
                             (* n t)))))])
      (fact 10))
    (letrec ([fib (lambda (n k)
                    (if (or (= n 0) (= n 1))
                        (k 1)
                        (fib (- n 1) (lambda (w)
                                       (fib (- n 2) (lambda (v)
                                                      (k (+ w v))))))))])
      (fib 10 (lambda (x) x)))
    (letrec ([num-list? (lambda (ls)
                          (if (null? ls)
                              #t
                              (if (fixnum? (car ls))
                                  (num-list? (cdr ls))
                                  #f)))]
             [length (lambda (ls)
                       (if (null? ls)
                           0
                           (+ (length (cdr ls)) 1)))]
             [dot-prod (lambda (ls1 ls2)
                         (if (if (null? ls1) (null? ls2) #f)
                             0
                             (+ (* (car ls1) (car ls2))
                                (dot-prod (cdr ls1) (cdr ls2)))))])
      (let ([ls1 '(1 2 3 4 5)]
            [ls2 '(5 4 3 2 1)])
        (if (if (if (eq? (num-list? ls1) #f) #f #t)
                (if (if (eq? (num-list? ls2) #f) #f #t)
                    (= (length ls1) (length ls2))
                    #f)
                #f)
            (dot-prod ls1 ls2)
            #f)))
    (letrec ([num-list? (lambda (ls)
                          (if (null? ls)
                              #t
                              (if (fixnum? (car ls))
                                  (num-list? (cdr ls))
                                  #f)))]
             [list-product (lambda (ls)
                             (if (null? ls)
                                 1
                                 (* (car ls) (list-product (cdr ls)))))])
      (let ([ls '(1 2 3 4 5)])
        (if (num-list? ls) (list-product ls) #f)))
    (letrec ([f (lambda (x y)
                    (if x (h (+ x y)) (g (+ x 1) (+ y 1))))]
             [g (lambda (u v)
                  (let ([a (+ u v)] [b (* u v)])
                    (letrec ([e (lambda (d)
                                  (let ([p (cons a b)])
                                    (letrec ([q (lambda (m)
                                                  (if (< m u)
                                                      (f m d)
                                                      (h (car p))))])
                                      (q (f a b)))))])
                      (e u))))]
             [h (lambda (w) w)])
      (f 4 5))
    (letrec ([curry-list
               (lambda (x)
                 (lambda (y)
                   (lambda (z)
                     (lambda (w)
                       (cons x (cons y (cons z (cons w '()))))))))]
             [append (lambda (ls1 ls2)
                       (if (null? ls1)
                           ls2
                           (cons (car ls1)
                                 (append (cdr ls1) ls2))))])
      (append
        ((((curry-list 1) 2) 3) 4)
        ((((curry-list 5) 6) 7) 8)))

    ; test use of keywords/primitives as variables

   ; contributed by Ryan Newton

    ;; Francis Fernandez
    (and (+ ((if (not (cons '1 '(2))) 
                 '#t 
                 (letrec ([f.1 '3] [f.2 (lambda (x.3) (+ x.3 '4))])
                   f.2))
             '5) '6) '#f)

    ;; Thiago Rebello
    (let ([a 5]
          [b 4])
      (letrec ([c (lambda(d e) (* d e))]
               [f (lambda(g h) (cons g h))])
        (if (or (> (c a b) 15) (= (c a b) 20))
            (f a b))))

    ;; Yin Wang
    (let ([begin (lambda (x y) (+ x y))]
          [set! (lambda (x y) (* x y))])
      (let ([lambda (lambda (x) (begin 1 x))])
        (let ([lambda (lambda (set! 1 2))])
          (let ([let (set! lambda lambda)])
            (begin let (set! lambda (set! 4 (begin 2 3))))))))

    ;; Ben Peters
    (let ([x '(4 5 6)]
          [y '(7 8 9)])
      (cons 1 (cons 2 (cons 3 (cons (car x) (cons (car (cdr x)) (cons (car (cdr (cdr x))) y)))))))
    
    ;; Patrick Jensen
    (let ([a 1])
      (letrec ([add1 (lambda (b) (+ b 1))]
               [sub1 (lambda (b) (- b 1))])
        (let ([c (lambda (a)
                   (if (or (not (= a 1)) (and (> a 1) (< a 4)))
                       (add1 a)
                       (sub1 a)))])
          (let ([d (c a)] [e (c (add1 a))] [f (c (sub1 a))])
            (cons d (cons e (cons f '())))))))

    ;; Melanie Dybvig

    ;; Lindsey Kuper
    (let ([foo (lambda (lambda)
                 (lambda))])
      (let ([lambda foo]
            [bar (lambda () #t)])
        (foo bar)))
 
    ;; Yu-Shan Huang

    ;; Chabane Maidi
    (letrec ([merge (lambda (ls ls2)
                      (if (null? ls)
                          ls2
                          (if (null? ls2)
                              ls
                              (if (< (car ls) (car ls2))
                                  (cons (car ls) (merge (cdr ls) ls2))
                                  (cons (car ls2) (merge ls (cdr ls2)))))))]
             [sort (lambda (ls)
                     (if (null? ls)
                         ls
                         (if (null? (cdr ls))
                             ls
                             (let ([halves (halves ls '() '() #t)])
                               (let ([first (car halves)]
                                     [second (car (cdr halves))])
                                 (merge (sort first) (sort second)))))))]
             [halves (lambda (ls first second first?)
                       (if (null? ls)
                           (cons first (cons second '()))
                           (if first?
                               (halves (cdr ls) (cons (car ls) first) second #f)
                               (halves (cdr ls) first (cons (car ls) second) #t))))]
             [pend (lambda (ls ls2)
                     (if (null? ls)
                         ls2
                         (cons (car ls) (pend (cdr ls) ls2))))])
      (pend (sort '(1 5 5 8 2 3 9)) (sort '(5 9 5 7 7 8 7))))

    ;; Kewal Karavinkoppa
    (letrec ([depth (lambda (ls)
                      (if (null? ls)
                          1
                          (if (pair? (car ls))
                              (let ([l ((lambda (m)
                                          (+ m 1))
                                        (depth (car ls)))]
                                    [r (depth (cdr ls))])
                                (if (< l r) r l))
                              (depth (cdr ls)))))])
      (depth '(1 2 (3 (4 (5 (6 7)))))))

    ;; Brennon York
    ((lambda (x) (if (if (eq? x 5) x (and x 1 2 3 4 (or 6 7 8 9))) 3)) 4)

    ;; Nilesh Mahajan
    (letrec ([F (lambda (func-arg)
                  (lambda (n)
                    (if (= n 0)
                        1
                        (* n (func-arg (- n 1))))))])
      (letrec ([Y (lambda (X)
                    ((lambda (procedure)
                       (X (lambda (arg) ((procedure procedure) arg))))
                     (lambda (procedure)
                       (X (lambda (arg) ((procedure procedure) arg))))))])
        (letrec ([fact (Y F)])
          (fact 5))))

    ;; Joseph Knecht
    (letrec ([f (lambda () '(1 . 2))]) (eq? (f) (f)))

    ;; Emily Lyons
    (letrec ([extend (lambda (num alist)
                       (if (null? alist)
                           (cons (cons num 1) '())
                           (if (= num (car (car alist)))
                               (cons (cons num (+ 1 (cdr (car alist))))
                                     (cdr alist))
                               (cons (car alist)
                                     (extend num (cdr alist))))))]
             [loop (lambda (ls alist)
                     (if (null? ls)
                         alist
                         (loop (cdr ls) (extend (car ls) alist))))])
      (loop '(1 3 4 5 5 4 5 2 3 4 1) '()))
  ))

(define default-open-file-output-port
  (lambda (path)
    (open-file-output-port path
      (file-options no-fail)
      (buffer-mode none)
      (make-transcoder (utf-8-codec) 'lf))))

(define generate-test-file
  (lambda (dir)
    (letrec
      ([gen (lambda (test* ord)
              (unless (null? test*)
                (let ([in-file-path (string-append dir (format "/~s.in" ord))]
                      [out-file-path (string-append dir (format "/~s.out" ord))]
                      [test (car test*)])
                  (call-with-port (default-open-file-output-port in-file-path)
                    (lambda (op)
                      (pretty-print (car test*) op)))
                  (call-with-port (default-open-file-output-port out-file-path)
                    (lambda (op)
                      (pretty-print (eval (car test*)) op)))
                  (gen (cdr test*) (add1 ord)))))])
      (gen tests 1))))
