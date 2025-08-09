;; 完整的 cond 表达式测试 - 带字符串支持
(cond
 (#f "false branch")
 (#t "true branch")
 (else "else branch"))

;; 测试成绩等级函数 - 使用字符串
(define (grade score)
  (cond
   ((>= score 90) "A")
   ((>= score 80) "B")
   ((>= score 70) "C")
   ((>= score 60) "D")
   (else "F")))

(grade 95)
(grade 85)
(grade 75)
(grade 65)
(grade 50)

;; 测试年龄分组
(define (age-group age)
  (cond
   ((<= age 12) "child")
   ((<= age 19) "teenager")
   ((<= age 59) "adult")
   (else "senior")))

(age-group 8)
(age-group 16)
(age-group 35)
(age-group 70)

;; 测试只有 else 的情况
(cond (else "only else"))

;; 测试复杂条件
(define (weather-advice temp)
  (cond
   ((< temp 0) "Stay inside!")
   ((< temp 10) "Wear a coat")
   ((< temp 25) "Nice weather")
   (else "It's hot!")))

(weather-advice -5)
(weather-advice 5)
(weather-advice 20)
(weather-advice 30)

(exit)
