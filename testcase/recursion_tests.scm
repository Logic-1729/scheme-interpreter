;; 递归函数测试集合

;; 1. 阶乘函数 (已经测试过的)
(define factorial (lambda (n) 
  (if (= n 0) 1 (* n (factorial (- n 1))))))
(factorial 5)
(factorial 6)

;; 2. 斐波那契数列
(define fibonacci (lambda (n)
  (if (<= n 1) 
      n 
      (+ (fibonacci (- n 1)) (fibonacci (- n 2))))))
(fibonacci 0)
(fibonacci 1)
(fibonacci 5)
(fibonacci 8)

;; 3. 计算数字的和 (从1到n)
(define sum-to-n (lambda (n)
  (if (= n 0) 
      0 
      (+ n (sum-to-n (- n 1))))))
(sum-to-n 10)
(sum-to-n 15)

;; 4. 幂运算的递归实现 (与内置expt对比)
(define power (lambda (base exp)
  (if (= exp 0)
      1
      (* base (power base (- exp 1))))))
(power 2 3)
(power 3 4)
(power 5 2)

;; 5. 最大公约数 (欧几里德算法)
(define gcd (lambda (a b)
  (if (= b 0)
      a
      (gcd b (modulo a b)))))
(gcd 48 18)
(gcd 21 14)
(gcd 17 13)

;; 6. 计算数字位数
(define count-digits (lambda (n)
  (if (< n 10)
      1
      (+ 1 (count-digits (quotient n 10))))))
(count-digits 5)
(count-digits 123)
(count-digits 9876)

;; 7. 阿克曼函数 (双递归)
(define ackermann (lambda (m n)
  (if (= m 0)
      (+ n 1)
      (if (= n 0)
          (ackermann (- m 1) 1)
          (ackermann (- m 1) (ackermann m (- n 1)))))))
(ackermann 0 5)
(ackermann 1 3)
(ackermann 2 3)

;; 8. 数字反转
(define reverse-number (lambda (n acc)
  (if (= n 0)
      acc
      (reverse-number (quotient n 10) (+ (* acc 10) (modulo n 10))))))
(define reverse-num (lambda (n) (reverse-number n 0)))
(reverse-num 1234)
(reverse-num 5678)

;; 9. 互相递归函数示例 - 判断奇偶性
(define is-even (lambda (n)
  (if (= n 0)
      #t
      (is-odd (- n 1)))))
(define is-odd (lambda (n)
  (if (= n 0)
      #f
      (is-even (- n 1)))))
(is-even 4)
(is-odd 4)
(is-even 7)
(is-odd 7)

(exit)
