# 🎯 Scheme 解释器支持的所有语法形式

本文档详细列出了当前 Scheme 解释器实现中支持的所有语法形式、原语函数和语言特性。

## 1. 特殊形式 (Special Forms)

这些是具有特殊语法和求值规则的保留字，不能作为普通函数使用：

### 1.1 绑定构造

#### `let` - 局部变量绑定
创建局部作用域，绑定变量到值。
```scheme
;; 基本用法
(let ((x 1) (y 2)) 
  (+ x y))                    ; 结果: 3

;; 嵌套绑定
(let ((x 5))
  (let ((y (* x 2)))
    (+ x y)))                 ; 结果: 15
```

#### `letrec` - 递归绑定
支持相互递归的变量绑定，用于定义递归函数。
```scheme
;; 简单递归
(letrec ((factorial 
           (lambda (n) 
             (if (<= n 1) 1 (* n (factorial (- n 1)))))))
  (factorial 5))              ; 结果: 120

;; 相互递归
(letrec ((even? (lambda (n) (if (= n 0) #t (odd? (- n 1)))))
         (odd?  (lambda (n) (if (= n 0) #f (even? (- n 1))))))
  (even? 10))                 ; 结果: #t
```

#### `define` - 变量/函数定义
定义全局变量或函数，支持语法糖。
```scheme
;; 变量定义
(define pi 3.14159)
(define greeting "Hello, World!")

;; 函数定义（完整形式）
(define square (lambda (x) (* x x)))

;; 函数定义（语法糖）
(define (square x) (* x x))

;; 多参数函数
(define (add3 x y z) (+ x y z))
```

### 1.2 控制流

#### `if` - 条件表达式
二路分支条件判断。
```scheme
;; 基本条件判断
(if (> x 0) "positive" "non-positive")

;; 嵌套条件
(if (> x 0) 
    (if (< x 10) "single digit positive" "multi digit positive")
    "non-positive")
```

#### `cond` - 多路条件表达式
多路分支条件判断，类似于 switch-case。
```scheme
;; 基本用法
(cond 
  ((< x 0) "negative")
  ((= x 0) "zero")
  ((> x 0) "positive")
  (else "impossible"))

;; 复杂条件
(cond 
  ((and (>= score 90) (<= score 100)) "A")
  ((and (>= score 80) (< score 90))   "B") 
  ((and (>= score 70) (< score 80))   "C")
  (else "F"))
```

#### `begin` - 顺序执行
按顺序执行多个表达式，返回最后一个表达式的值，支持内部定义。
```scheme
;; 基本序列执行
(begin 
  (display "Starting calculation...")
  (define result (* 6 7))
  (display "Calculation complete")
  result)                     ; 结果: 42

;; 内部定义
(begin
  (define (helper x) (* x 2))
  (define y 10)
  (helper y))                 ; 结果: 20
```

#### `and` - 逻辑与
短路求值的逻辑与运算。
```scheme
;; 基本用法
(and #t #t #f)                ; 结果: #f
(and 1 2 3)                   ; 结果: 3 (最后一个值)

;; 短路求值
(and #f (error "not reached")) ; 结果: #f (不会执行error)
```

#### `or` - 逻辑或
短路求值的逻辑或运算。
```scheme
;; 基本用法
(or #f #f #t)                 ; 结果: #t
(or #f 42 #f)                 ; 结果: 42 (第一个真值)

;; 短路求值
(or #t (error "not reached"))  ; 结果: #t (不会执行error)
```

### 1.3 函数定义

#### `lambda` - 匿名函数表达式
创建匿名函数，支持闭包。
```scheme
;; 基本lambda
(lambda (x) (* x x))

;; 多参数lambda  
(lambda (x y z) (+ x y z))

;; 无参数lambda
(lambda () 42)

;; 直接调用lambda
((lambda (x y) (+ x y)) 3 4)   ; 结果: 7

;; 多表达式函数体
(lambda (x)
  (define temp (* x 2))
  (display "Processing...")
  (+ temp 1))
```

### 1.4 数据构造

#### `quote` - 引用表达式
防止表达式被求值，返回字面数据。
```scheme
;; 引用符号
(quote hello)                 ; 结果: hello
'hello                        ; 语法糖形式

;; 引用列表
(quote (+ 1 2))              ; 结果: (+ 1 2) (不求值)
'(1 2 3)                     ; 结果: (1 2 3)

;; 引用字符串
'"quoted string"             ; 结果: "quoted string"
```

### 1.5 变量赋值

#### `set!` - 修改变量值
修改已存在变量的值。
```scheme
;; 修改全局变量
(define x 10)
(set! x 20)                   ; x 现在是 20

;; 修改闭包变量
(define counter
  (let ((count 0))
    (lambda ()
      (set! count (+ count 1))
      count)))
(counter)                     ; 结果: 1
(counter)                     ; 结果: 2
```

## 2. 原语函数 (Primitive Functions)

这些是内置函数，可以像普通函数一样调用和传递：

### 2.1 算术运算

#### `+` - 加法（支持多参数）
```scheme
(+ 1 2)                       ; 结果: 3
(+ 1 2 3 4)                   ; 结果: 10
(+)                           ; 结果: 0 (加法单位元)
```

#### `-` - 减法（支持多参数）
```scheme
(- 10 3)                      ; 结果: 7
(- 10 3 2)                    ; 结果: 5
(- 5)                         ; 结果: -5 (取负)
```

#### `*` - 乘法（支持多参数）
```scheme
(* 2 3)                       ; 结果: 6
(* 2 3 4)                     ; 结果: 24
(*)                           ; 结果: 1 (乘法单位元)
```

#### `/` - 除法（支持多参数，返回有理数）
```scheme
(/ 6 2)                       ; 结果: 3/1
(/ 1 3)                       ; 结果: 1/3
(/ 12 2 3)                    ; 结果: 2/1
(/ 5)                         ; 结果: 1/5 (倒数)
```

#### `quotient` - 整数除法
```scheme
(quotient 17 5)               ; 结果: 3
(quotient -17 5)              ; 结果: -3
```

#### `modulo` - 模运算
```scheme
(modulo 17 5)                 ; 结果: 2
(modulo -17 5)                ; 结果: 3
```

#### `expt` - 指数运算
```scheme
(expt 2 10)                   ; 结果: 1024
(expt 3 4)                    ; 结果: 81
```

### 2.2 比较运算

#### `<` - 小于（支持多参数）
```scheme
(< 1 2)                       ; 结果: #t
(< 1 2 3 4)                   ; 结果: #t
(< 1 3 2)                     ; 结果: #f
```

#### `<=` - 小于等于（支持多参数）
```scheme
(<= 1 2)                      ; 结果: #t
(<= 1 1 2)                    ; 结果: #t
```

#### `=` - 数值相等（支持多参数）
```scheme
(= 1 1)                       ; 结果: #t
(= 1 1 1 1)                   ; 结果: #t
(= 1 2)                       ; 结果: #f
```

#### `>=` - 大于等于（支持多参数）
```scheme
(>= 3 2)                      ; 结果: #t
(>= 3 3 2 1)                  ; 结果: #t
```

#### `>` - 大于（支持多参数）
```scheme
(> 3 2)                       ; 结果: #t
(> 4 3 2 1)                   ; 结果: #t
```

#### `eq?` - 对象相等性检查
```scheme
(eq? 'a 'a)                   ; 结果: #t
(eq? 1 1)                     ; 结果: #t
(eq? "hello" "hello")         ; 结果: #f (不同对象)
```

### 2.3 类型谓词

#### `boolean?` - 检查是否为布尔值
```scheme
(boolean? #t)                 ; 结果: #t
(boolean? #f)                 ; 结果: #t
(boolean? 42)                 ; 结果: #f
```

#### `number?` - 检查是否为数值
```scheme
(number? 42)                  ; 结果: #t
(number? -17)                 ; 结果: #t
(number? "42")                ; 结果: #f
```

#### `string?` - 检查是否为字符串
```scheme
(string? "hello")             ; 结果: #t
(string? 'hello)              ; 结果: #f
```

#### `symbol?` - 检查是否为符号
```scheme
(symbol? 'hello)              ; 结果: #t
(symbol? "hello")             ; 结果: #f
```

#### `null?` - 检查是否为空值
```scheme
(null? '())                   ; 结果: #t
(null? (list))                ; 结果: #t
(null? (list 1))              ; 结果: #f
```

#### `pair?` - 检查是否为对偶
```scheme
(pair? (cons 1 2))            ; 结果: #t
(pair? (list 1 2))            ; 结果: #t
(pair? '())                   ; 结果: #f
```

#### `list?` - 检查是否为列表
```scheme
(list? (list 1 2 3))          ; 结果: #t
(list? '())                   ; 结果: #t
(list? (cons 1 2))            ; 结果: #f (不规范列表)
```

#### `procedure?` - 检查是否为过程
```scheme
(procedure? +)                ; 结果: #t
(procedure? (lambda (x) x))   ; 结果: #t
(procedure? 42)               ; 结果: #f
```

### 2.4 列表操作

#### `cons` - 构造对偶
```scheme
(cons 1 2)                    ; 结果: (1 . 2)
(cons 1 '())                  ; 结果: (1)
(cons 1 (cons 2 '()))         ; 结果: (1 2)
```

#### `car` - 获取对偶的第一个元素
```scheme
(car (cons 1 2))              ; 结果: 1
(car (list 1 2 3))            ; 结果: 1
```

#### `cdr` - 获取对偶的第二个元素
```scheme
(cdr (cons 1 2))              ; 结果: 2
(cdr (list 1 2 3))            ; 结果: (2 3)
```

#### `list` - 构造列表
```scheme
(list)                        ; 结果: ()
(list 1)                      ; 结果: (1)
(list 1 2 3)                  ; 结果: (1 2 3)
```

#### `set-car!` - 修改对偶的第一个元素
```scheme
(define p (cons 1 2))
(set-car! p 10)
p                             ; 结果: (10 . 2)
```

#### `set-cdr!` - 修改对偶的第二个元素
```scheme
(define p (cons 1 2))
(set-cdr! p 20)
p                             ; 结果: (1 . 20)
```

### 2.5 逻辑运算

#### `not` - 逻辑非
```scheme
(not #t)                      ; 结果: #f
(not #f)                      ; 结果: #t
(not 0)                       ; 结果: #f (非#f即真)
```

### 2.6 输入输出

#### `display` - 显示值
输出值到屏幕，字符串不显示引号，支持转义字符。
```scheme
(display "Hello, World!")     ; 输出: Hello, World!
(display 42)                  ; 输出: 42
(display '(1 2 3))            ; 输出: (1 2 3)
```

### 2.7 系统控制

#### `void` - 返回空值
```scheme
(void)                        ; 结果: #<void>
```

#### `exit` - 退出解释器
```scheme
(exit)                        ; 退出程序
```

## 3. 语法糖 (Syntactic Sugar)

解释器支持的语法糖形式，会自动转换为标准形式：

### 3.1 函数定义语法糖

#### 基本函数定义
```scheme
;; 标准形式
(define factorial (lambda (n) (if (= n 0) 1 (* n (factorial (- n 1))))))

;; 语法糖形式（等价）
(define (factorial n) (if (= n 0) 1 (* n (factorial (- n 1)))))
```

#### 多参数函数
```scheme
;; 标准形式
(define add3 (lambda (x y z) (+ x y z)))

;; 语法糖形式
(define (add3 x y z) (+ x y z))
```

#### 多表达式函数体
```scheme
;; 标准形式
(define complex-func 
  (lambda (x) 
    (begin
      (define temp (* x 2))
      (display temp)
      (+ temp 1))))

;; 语法糖形式
(define (complex-func x)
  (define temp (* x 2))
  (display temp)
  (+ temp 1))
```

### 3.2 引用语法糖

#### 符号引用
```scheme
;; 标准形式
(quote symbol)

;; 语法糖形式
'symbol
```

#### 列表引用
```scheme
;; 标准形式
(quote (1 2 3))

;; 语法糖形式
'(1 2 3)
```

## 4. 基本数据类型字面量

### 4.1 数值字面量

#### 整数
```scheme
42                            ; 正整数
-17                           ; 负整数
0                             ; 零
```

#### 有理数
通过除法运算产生：
```scheme
(/ 1 3)                       ; 结果: 1/3
(/ 22 7)                      ; 结果: 22/7
```

### 4.2 布尔字面量

#### 布尔值
```scheme
#t                            ; 真值
#f                            ; 假值
```

### 4.3 字符串字面量

#### 基本字符串
```scheme
"hello world"                 ; 基本字符串
""                            ; 空字符串
```

#### 转义字符支持
```scheme
"hello\nworld"                ; 包含换行符
"say \"hello\""               ; 包含引号
"tab\there"                   ; 包含制表符
"path\\file"                  ; 包含反斜杠
```

### 4.4 符号

#### 引用符号
```scheme
'symbol                       ; 符号字面量
'hello-world                  ; 带连字符的符号
'?predicate                   ; 谓词符号
'set-var!                     ; 修改操作符号
```

### 4.5 列表

#### 基本列表
```scheme
'()                           ; 空列表
'(1 2 3)                      ; 数值列表
'(a b c)                      ; 符号列表
'("hello" "world")            ; 字符串列表
```

#### 嵌套列表
```scheme
'((1 2) (3 4))               ; 二维列表
'(1 (2 3) 4)                 ; 混合嵌套
```

#### 点对
```scheme
'(1 . 2)                     ; 基本点对
'(1 2 . 3)                   ; 不规范列表
```

## 5. 高级语法特性

### 5.1 闭包和词法作用域

#### 闭包变量捕获
```scheme
(define (make-counter start)
  (let ((count start))
    (lambda ()
      (set! count (+ count 1))
      count)))

(define counter1 (make-counter 0))
(define counter2 (make-counter 100))
(counter1)                    ; 结果: 1
(counter1)                    ; 结果: 2
(counter2)                    ; 结果: 101
```

#### 词法作用域
```scheme
(define x 10)
(define (outer)
  (define x 20)
  (define (inner) x)
  (inner))
(outer)                       ; 结果: 20 (使用内层的x)
```

### 5.2 递归和相互递归

#### 简单递归
```scheme
(define (factorial n)
  (if (<= n 1)
      1
      (* n (factorial (- n 1)))))
```

#### 尾递归
```scheme
(define (factorial-tail n acc)
  (if (<= n 1)
      acc
      (factorial-tail (- n 1) (* n acc))))
```

#### 相互递归
```scheme
(define (even? n)
  (if (= n 0) #t (odd? (- n 1))))

(define (odd? n)
  (if (= n 0) #f (even? (- n 1))))
```

### 5.3 内部定义

#### 函数内部定义
```scheme
(define (outer-func x)
  (define (helper y) (* y y))        ; 内部函数
  (define temp (+ x 1))              ; 内部变量
  (helper temp))
```

#### Begin块内部定义
```scheme
(begin
  (define local-var 42)              ; 局部变量
  (define (local-func) local-var)    ; 局部函数
  (local-func))                      ; 结果: 42
```

### 5.4 高阶函数

#### 函数作为参数
```scheme
(define (apply-twice f x)
  (f (f x)))

(apply-twice (lambda (n) (* n 2)) 3)  ; 结果: 12
```

#### 函数作为返回值
```scheme
(define (make-adder n)
  (lambda (x) (+ x n)))

(define add10 (make-adder 10))
(add10 5)                             ; 结果: 15
```

## 6. 特殊语法约定

### 6.1 注释语法

#### 行注释
```scheme
;; 这是单行注释
(+ 1 2)  ; 行末注释
```

### 6.2 变量命名约定

#### 有效的标识符字符
- 字母：`a-z`, `A-Z`
- 数字：`0-9`（不能开头）
- 特殊字符：`-`, `?`, `!`, `+`, `*`, `/`, `<`, `>`, `=`

#### 命名约定
```scheme
;; 谓词函数以?结尾
(define (positive? x) (> x 0))

;; 修改函数以!结尾
(define (set-value! var val) (set! var val))

;; 多单词用连字符连接
(define my-special-function (lambda (x) x))
```

### 6.3 函数调用语法

#### 基本调用
```scheme
(function-name arg1 arg2 ...)
```

#### Lambda直接调用
```scheme
((lambda (x y) (+ x y)) 3 4)
```

#### 嵌套调用
```scheme
(+ (* 2 3) (/ 8 4))           ; 结果: 8
```

## 7. 语法形式分类统计

### 7.1 按类别统计
- **特殊形式**: 12 个（let, letrec, define, if, cond, begin, and, or, lambda, quote, set!）
- **算术运算**: 7 个（+, -, *, /, quotient, modulo, expt）
- **比较运算**: 6 个（<, <=, =, >=, >, eq?）
- **类型谓词**: 8 个（boolean?, number?, string?, symbol?, null?, pair?, list?, procedure?）
- **列表操作**: 6 个（cons, car, cdr, list, set-car!, set-cdr!）
- **逻辑运算**: 1 个（not）
- **系统函数**: 3 个（display, void, exit）
- **语法糖**: 2 种主要形式（函数定义、引用）

### 7.2 按求值特性统计
- **特殊求值**: 12 个（特殊形式）
- **标准求值**: 31 个（原语函数）
- **语法转换**: 2 个（语法糖）

### 7.3 按参数特性统计
- **可变参数**: 10 个（+, -, *, /, <, <=, =, >=, >, and, or, list）
- **固定参数**: 33 个（其他所有函数）
- **无参数**: 3 个（void, exit, +, *, and, or 的零参数情况）

## 8. 实现特色

### 8.1 完整的闭包支持
- 词法作用域
- 变量捕获
- 环境链管理

### 8.2 递归优化
- 支持直接递归
- 支持相互递归
- letrec 语义正确实现

### 8.3 语法糖自动展开
- 函数定义语法糖自动转换
- 引用语法糖处理
- 多表达式函数体支持

### 8.4 类型系统
- 动态类型检查
- 完整的类型谓词
- 运行时类型安全

### 8.5 错误处理
- 详细的错误信息
- 类型错误检查
- 参数数量验证

这个 Scheme 解释器实现了 Scheme 语言的核心语法特性，支持现代函数式编程的主要范式，包括高阶函数、闭包、递归等特性，为学习和实践函数式编程提供了完整的语言环境。
