(define (make-queue) (cons '() '()))
(define q (make-queue))

;; 先测试 enqueue
(define (enqueue! queue obj)
  (let ((lobj (cons obj '())))
    (if (null? (car queue))
        (begin 
          (set-car! queue lobj)
          (set-cdr! queue lobj))
        (begin 
          (set-cdr! (cdr queue) lobj)
          (set-cdr! queue lobj))))
  (car queue))

(enqueue! q 'a)
q

;; 现在测试 dequeue
(define (dequeue! queue)
  (define obj (car (car queue)))
  (set-car! queue (cdr (car queue)))
  obj)

(dequeue! q)
q
(exit)
