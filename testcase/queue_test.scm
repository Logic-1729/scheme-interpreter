;; Queue implementation test
;; This test demonstrates a queue data structure using cons pairs

;; Define queue constructor - creates empty queue with front and rear pointers
(define (make-queue) 
  (cons '() '()))

;; Enqueue operation - adds element to rear of queue
(define (enqueue! queue obj)
  (let ((lobj (cons obj '())))
    (if (null? (car queue))
        ;; Queue is empty - set both front and rear to new element
        (begin 
          (set-car! queue lobj)
          (set-cdr! queue lobj))
        ;; Queue not empty - add to rear and update rear pointer
        (begin 
          (set-cdr! (cdr queue) lobj)
          (set-cdr! queue lobj))))
  ;; Return the front of queue (for display purposes)
  (car queue))

;; Dequeue operation - removes element from front of queue
(define (dequeue! queue)
  (define obj (car (car queue)))
  ;; Move front pointer to next element
  (set-car! queue (cdr (car queue)))
  ;; Return the dequeued object
  obj)

;; Create a new queue
(define q (make-queue))

;; Test enqueue operations
(enqueue! q 'a)
(enqueue! q 'b) 
(enqueue! q 'c)

;; Test dequeue operation
(dequeue! q)

;; Display final queue state
q

;; Additional test cases
(define q2 (make-queue))

;; Test single element
(enqueue! q2 'x)
(dequeue! q2)
q2

;; Test multiple enqueue/dequeue cycles
(define q3 (make-queue))
(enqueue! q3 1)
(enqueue! q3 2)
(dequeue! q3)
(enqueue! q3 3)
(dequeue! q3)
(enqueue! q3 4)
q3

(exit)