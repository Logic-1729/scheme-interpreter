# Scheme解释器功能文档

## 概述

这是一个用C++实现的Scheme解释器，支持Scheme语言的核心功能。解释器采用递归下降解析器和树遍历求值器的设计。

## 支持的数据类型

### 基本数据类型
- **整数** (Fixnum): 32位有符号整数，范围约为 -2^31 到 2^31-1
- **布尔值** (Boolean): `#t` (真) 和 `#f` (假)
- **字符串** (String): 支持双引号字符串，如 `"hello world"`
- **符号** (Symbol): 标识符，如 `x`, `foo`, `hello-world`
- **空值** (Null): 空列表 `()`
- **对偶** (Pair): 使用cons构造的数据结构 `(a . b)`
- **列表** (List): 嵌套的对偶结构，如 `(1 2 3)`
- **过程** (Procedure): Lambda表达式和函数
- **空值** (Void): 表达式的空返回值

### 复合数据类型
- **列表**: 支持任意嵌套的列表结构
- **点对**: cons单元格，支持非正规列表

## 核心语法结构

### 1. 变量定义和赋值

#### 变量定义
```scheme
;; 简单变量定义
(define x 42)
(define name "Alice")

;; 函数定义语法糖
(define (square x) (* x x))
(define (add a b) (+ a b))

;; 多表达式函数体
(define (complex-function x y)
  (define temp (* x y))
  (+ temp 1)
  (- temp 2))
```

#### 变量赋值
```scheme
;; 修改全局变量
(define x 10)
(set! x 20)

;; 修改闭包中的变量
(define counter
  (let ((count 0))
    (lambda ()
      (set! count (+ count 1))
      count)))
```

### 2. 控制流结构

#### 条件表达式
```scheme
;; if表达式
(if (> x 0) "positive" "non-positive")

;; cond表达式
(cond
  ((< x 0) "negative")
  ((= x 0) "zero")
  ((> x 0) "positive")
  (else "impossible"))
```

#### 逻辑运算
```scheme
;; 短路求值的and
(and #t #f (error "not reached"))  ; 返回 #f

;; 短路求值的or
(or #f #t (error "not reached"))   ; 返回 #t

;; 逻辑非
(not #f)  ; 返回 #t
```

### 3. 函数和闭包

#### Lambda表达式
```scheme
;; 简单lambda
(lambda (x) (* x x))

;; 多参数lambda
(lambda (x y z) (+ x y z))

;; 多表达式body
(lambda (x)
  (define temp (* x 2))
  (+ temp 1))

;; 无参数lambda
(lambda () 42)
```

#### 函数调用
```scheme
;; 直接调用
((lambda (x) (* x x)) 5)

;; 定义后调用
(define f (lambda (x) (* x x)))
(f 5)

;; 语法糖形式
(define (f x) (* x x))
(f 5)
```

### 4. 作用域和绑定

#### Let绑定
```scheme
;; let表达式
(let ((x 1) (y 2))
  (+ x y))

;; 嵌套let
(let ((x 1))
  (let ((y (+ x 1)))
    (* x y)))
```

#### Letrec递归绑定
```scheme
;; 相互递归函数
(letrec ((even? (lambda (n)
                  (if (= n 0) #t (odd? (- n 1)))))
         (odd? (lambda (n)
                 (if (= n 0) #f (even? (- n 1))))))
  (even? 10))
```

#### Begin序列
```scheme
;; begin支持内部定义
(begin
  (define x 10)
  (define y 20)
  (+ x y))
```

### 5. 列表操作

#### 基本列表函数
```scheme
;; 构造列表
(cons 1 2)        ; (1 . 2)
(cons 1 '())      ; (1)
(list 1 2 3)      ; (1 2 3)

;; 访问列表元素
(car '(1 2 3))    ; 1
(cdr '(1 2 3))    ; (2 3)

;; 修改列表
(define lst (list 1 2 3))
(set-car! lst 10) ; lst变为(10 2 3)
(set-cdr! lst '(20 30)) ; lst变为(10 20 30)
```

#### 列表谓词
```scheme
(null? '())       ; #t
(pair? '(1 . 2))  ; #t
(list? '(1 2 3))  ; #t
```

### 6. 算术运算

#### 基本算术
```scheme
(+ 1 2 3 4)       ; 10 (可变参数)
(- 10 3)          ; 7
(- 10 3 2)        ; 5
(* 2 3 4)         ; 24
(/ 12 3)          ; 4
(quotient 17 5)   ; 3 (整数除法)
(modulo 17 5)     ; 2 (模运算)
(expt 2 10)       ; 1024 (指数运算)
```

#### 比较运算
```scheme
(= 1 1)           ; #t
(< 1 2)           ; #t
(<= 1 1)          ; #t
(> 2 1)           ; #t
(>= 2 1)          ; #t
```

### 7. 类型谓词

```scheme
;; 数值类型
(number? 42)      ; #t
(integer? 42)     ; #t

;; 布尔类型
(boolean? #t)     ; #t

;; 字符串类型
(string? "hello") ; #t

;; 符号类型
(symbol? 'foo)    ; #t

;; 过程类型
(procedure? +)    ; #t

;; 相等性测试
(eq? 'a 'a)       ; #t
```

### 8. 引用和求值

#### Quote表达式
```scheme
;; 引用符号
'hello            ; hello
(quote hello)     ; hello

;; 引用列表
'(1 2 3)          ; (1 2 3)
(quote (+ 1 2))   ; (+ 1 2) (不求值)

;; 引用字符串
'"quoted string"  ; "quoted string"
```

### 9. I/O操作

```scheme
;; 显示输出
(display "Hello, World!")
(display 42)

;; 退出解释器
(exit)
```

## 语法糖支持

### 函数定义语法糖
```scheme
;; 标准形式
(define square (lambda (x) (* x x)))

;; 语法糖形式
(define (square x) (* x x))

;; 多参数语法糖
(define (add3 x y z) (+ x y z))

;; 多表达式body语法糖
(define (complex-func x)
  (define temp (* x 2))
  (display temp)
  (+ temp 1))
```

### 引用语法糖
```scheme
;; 完整形式
(quote symbol)
(quote (1 2 3))

;; 简写形式
'symbol
'(1 2 3)
```

## 高级特性

### 闭包和词法作用域
```scheme
;; 闭包捕获外层变量
(define make-counter
  (lambda (initial)
    (let ((count initial))
      (lambda ()
        (set! count (+ count 1))
        count))))

(define counter1 (make-counter 0))
(define counter2 (make-counter 100))
(counter1) ; 1
(counter2) ; 101
```

### 递归和相互递归
```scheme
;; 简单递归
(define (factorial n)
  (if (<= n 1)
      1
      (* n (factorial (- n 1)))))

;; 尾递归
(define (factorial-tail n acc)
  (if (<= n 1)
      acc
      (factorial-tail (- n 1) (* n acc))))

;; 相互递归
(begin
  (define (even? n)
    (if (= n 0) #t (odd? (- n 1))))
  (define (odd? n)
    (if (= n 0) #f (even? (- n 1))))
  (even? 10))
```

### 内部定义
```scheme
;; 在函数体中定义局部函数
(define (outer-func x)
  (define (helper y) (* y y))
  (define (another z) (+ z 1))
  (+ (helper x) (another x)))

;; 在begin块中的内部定义
(begin
  (define local-var 42)
  (define (local-func) local-var)
  (local-func))
```

## 实现限制

### 数值精度限制
- 整数使用32位int类型，范围约为-2,147,483,648到2,147,483,647
- 大数运算会发生溢出（如：`(factorial 20)`会溢出，`(factorial 50)`返回0）
- 不支持浮点数运算
- 不支持复数和有理数

### 性能限制
- 不支持尾递归优化，深度递归会导致栈溢出
- 不支持垃圾回收优化
- 大规模数据处理性能有限

### 未实现的标准特性
- 字符类型 (`#\a`, `#\newline`)
- 向量类型 (`#(1 2 3)`)
- 端口和文件I/O (仅支持`display`)
- 宏系统 (`define-syntax`, `syntax-rules`)
- 延续 (`call/cc`)
- 多值返回 (`values`, `call-with-values`)
- 模块系统
- 异常处理
- 字符串处理函数
- 更多的数学函数（sin, cos, log等）

### 内存管理
- 使用C++智能指针进行内存管理
- 可能存在循环引用导致的内存泄漏
- 不支持垃圾回收

## 使用示例

### 基本使用
```bash
# 编译解释器
cd build && make

# 运行交互模式
./code

# 运行脚本文件
./code < script.scm
```

### 示例程序

#### 计算斐波那契数列
```scheme
(define (fibonacci n)
  (cond
    ((= n 0) 0)
    ((= n 1) 1)
    (else (+ (fibonacci (- n 1))
             (fibonacci (- n 2))))))

(fibonacci 10) ; 55
```

#### 列表处理
```scheme
(define (length lst)
  (if (null? lst)
      0
      (+ 1 (length (cdr lst)))))

(define (reverse lst)
  (define (rev-helper lst acc)
    (if (null? lst)
        acc
        (rev-helper (cdr lst) (cons (car lst) acc))))
  (rev-helper lst '()))

(length '(1 2 3 4 5))     ; 5
(reverse '(1 2 3 4 5))    ; (5 4 3 2 1)
```

#### 高阶函数
```scheme
(define (map f lst)
  (if (null? lst)
      '()
      (cons (f (car lst))
            (map f (cdr lst)))))

(define (filter pred lst)
  (cond
    ((null? lst) '())
    ((pred (car lst))
     (cons (car lst) (filter pred (cdr lst))))
    (else (filter pred (cdr lst)))))

(map (lambda (x) (* x x)) '(1 2 3 4)) ; (1 4 9 16)
(filter (lambda (x) (> x 2)) '(1 2 3 4 5)) ; (3 4 5)
```

## 与标准Scheme的主要差异

### 1. 数值系统
- **限制**: 仅支持32位整数，无大数支持
- **影响**: `(factorial 50)` 返回0而非正确的大数
- **标准Scheme**: 支持任意精度整数

### 2. 类型系统
- **限制**: 缺少字符、向量、端口等标准类型
- **影响**: 无法进行字符串处理、向量操作
- **标准Scheme**: 支持完整的类型体系

### 3. I/O系统
- **限制**: 仅支持`display`输出，无文件I/O
- **影响**: 无法读写文件、处理用户输入
- **标准Scheme**: 支持端口、文件、格式化输出等

### 4. 宏系统
- **限制**: 完全不支持宏
- **影响**: 无法扩展语言语法
- **标准Scheme**: 支持强大的宏系统

### 5. 性能优化
- **限制**: 无尾递归优化
- **影响**: 深度递归会栈溢出
- **标准Scheme**: 必须支持尾递归优化

### 6. 错误处理
- **限制**: 使用C++异常机制
- **影响**: 无法在Scheme代码中捕获和处理错误
- **标准Scheme**: 支持异常处理机制

## 技术实现

### 架构设计
- **词法分析**: 手写递归下降解析器
- **语法分析**: 构建抽象语法树(AST)
- **求值器**: 树遍历解释器
- **环境模型**: 链表实现的环境链
- **内存管理**: C++智能指针

### 核心组件
- `syntax.hpp/cpp`: 语法分析和token处理
- `parser.cpp`: 语法树构建
- `expr.hpp/cpp`: 表达式类型定义  
- `evaluation.cpp`: 表达式求值实现
- `value.hpp/cpp`: 值类型和环境管理
- `Def.hpp/cpp`: 基本定义和枚举

### 重构历史
- ✅ **命名标准化**: Identifier → SymbolSyntax, StringLiteral → StringSyntax
- ✅ **代码现代化**: 消除goto语句，使用现代C++结构
- ✅ **功能完善**: 修复闭包set!、函数体内部define、cond表达式解析
- ✅ **多表达式支持**: Lambda和函数定义支持多个body表达式

这个实现为学习Scheme语言和解释器设计提供了一个清晰的参考，涵盖了Scheme的核心特性，虽然有一些限制，但足以运行大部分基础的Scheme程序。
