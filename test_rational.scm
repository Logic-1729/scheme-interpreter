;; Test rational number arithmetic
(display "Testing rational arithmetic:")

;; Basic division (creates rationals)
(/ 1 2)
(/ 3 4)
(/ -5 6)

;; Rational + Integer
(+ (/ 1 2) 3)
(+ 2 (/ 1 4))

;; Rational + Rational
(+ (/ 1 2) (/ 1 3))
(+ (/ 2 3) (/ 1 6))

;; Rational * Integer
(* (/ 1 2) 4)
(* 3 (/ 2 5))

;; Rational * Rational
(* (/ 1 2) (/ 3 4))
(* (/ 2 3) (/ 4 5))

;; Rational - Integer
(- (/ 7 2) 2)
(- 5 (/ 1 3))

;; Rational - Rational
(- (/ 3 4) (/ 1 8))
(- (/ 5 6) (/ 1 2))

;; Rational / Integer
(/ (/ 1 2) 3)
(/ (/ 4 5) 2)

;; Rational / Rational
(/ (/ 1 2) (/ 1 3))
(/ (/ 2 3) (/ 4 5))

;; Comparison operations
(< (/ 1 2) 1)
(< 1 (/ 3 2))
(< (/ 1 3) (/ 1 2))

(= (/ 2 4) (/ 1 2))
(= (/ 3 2) 1)
(= 2 (/ 4 2))

(> (/ 3 2) 1)
(> 2 (/ 3 2))
(> (/ 2 3) (/ 1 2))

;; Multi-argument operations
(+ 1 (/ 1 2) (/ 1 3))
(* 2 (/ 1 2) (/ 3 4))
(- 5 (/ 1 2) (/ 1 4))

;; Variadic comparisons
(< 1 (/ 3 2) 2)
(= (/ 1 2) (/ 2 4) (/ 3 6))
(> 2 (/ 3 2) 1)

(exit)
