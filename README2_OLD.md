# Scheme 解释器功能说明

本文档详细描述了当前 Scheme 解释器的实现功能，包括所有支持的基本类型、表达式类型、内置函数和语法特性。

## 目录
- [值类型 (Value Types)](#值类型-value-types)
- [表达式类型 (Expression Types)](#表达式类型-expression-types)
- [内置函数 (Primitives)](#内置函数-primitives)
- [保留字和特殊形式 (Reserved Words)](#保留字和特殊形式-reserved-words)
- [语法糖 (Syntactic Sugar)](#语法糖-syntactic-sugar)
- [高级功能 (Advanced Features)](#高级功能-advanced-features)

## 值类型 (Value Types)

解释器支持以下基本值类型：

### 数值类型
- **V_INT**: 整数类型 (Integer)
  - 例如: `42`, `-17`, `0`
- **V_RATIONAL**: 有理数类型 (Rational)  
  - 支持分数表示

### 基本类型
- **V_BOOL**: 布尔类型 (Boolean)
  - `#t` (真), `#f` (假)
- **V_STRING**: 字符串类型 (String)
  - 例如: `"hello"`, `"world"`
  - 支持转义字符
- **V_SYM**: 符号类型 (Symbol)
  - 例如: `'x`, `'hello`, `'my-var`
- **V_NULL**: 空值类型 (Null)
  - 表示空列表 `'()`

### 复合类型
- **V_PAIR**: 序对类型 (Pair)
  - cons 单元, 例如: `(cons 1 2)` → `(1 . 2)`
- **V_PROC**: 过程类型 (Procedure)
  - 函数/lambda 表达式
- **V_PRIMITIVE**: 原语类型
  - 内置函数的表示

### 系统类型
- **V_VOID**: 空返回类型
  - 用于没有返回值的操作 (但在 REPL 中被隐藏)
- **V_TERMINATE**: 终止类型
  - 用于 `exit` 命令

## 表达式类型 (Expression Types)

### 基本表达式
- **E_VAR**: 变量引用
- **E_FIXNUM**: 整数字面量
- **E_STRING**: 字符串字面量
- **E_TRUE**: 布尔真值 `#t`
- **E_FALSE**: 布尔假值 `#f`
- **E_VOID**: 空值表达式

### 控制结构
- **E_IF**: 条件表达式
  - `(if condition then-expr else-expr)`
- **E_COND**: 多路条件表达式
  - `(cond (test1 result1) (test2 result2) ... (else default))`
- **E_AND**: 逻辑与 (短路求值)
  - `(and expr1 expr2 ...)`
- **E_OR**: 逻辑或 (短路求值)  
  - `(or expr1 expr2 ...)`
- **E_BEGIN**: 顺序执行
  - `(begin expr1 expr2 ... exprN)`

### 函数相关
- **E_LAMBDA**: Lambda 表达式
  - `(lambda (param1 param2 ...) body...)`
  - 支持多表达式函数体
- **E_APPLY**: 函数调用
  - `(func arg1 arg2 ...)`

### 绑定和作用域
- **E_LET**: 局部绑定
  - `(let ((var1 val1) (var2 val2) ...) body)`
- **E_LETREC**: 递归绑定
  - `(letrec ((var1 val1) (var2 val2) ...) body)`
- **E_DEFINE**: 变量/函数定义
- **E_SET**: 变量赋值
  - `(set! var value)`

### 其他
- **E_QUOTE**: 引用
  - `(quote expr)` 或 `'expr`
- **E_EXIT**: 退出解释器

## 内置函数 (Primitives)

### 算术运算
- **+**: 加法 `(+ a b ...)` - 支持多参数
- **-**: 减法 `(- a b ...)` - 支持多参数
- **\***: 乘法 `(* a b ...)` - 支持多参数  
- **/**: 除法 `(/ a b ...)` - 支持多参数
- **quotient**: 整数除法 `(quotient a b)`
- **modulo**: 取模运算 `(modulo a b)`
- **expt**: 指数运算 `(expt base power)`

### 比较运算
- **=**: 数值相等 `(= a b ...)`
- **<**: 小于 `(< a b ...)`
- **<=**: 小于等于 `(<= a b ...)`
- **>**: 大于 `(> a b ...)`
- **>=**: 大于等于 `(>= a b ...)`
- **eq?**: 对象相等性 `(eq? obj1 obj2)`

### 类型检查
- **number?**: 检查是否为数值 `(number? obj)`
- **boolean?**: 检查是否为布尔值 `(boolean? obj)`
- **symbol?**: 检查是否为符号 `(symbol? obj)`
- **string?**: 检查是否为字符串 `(string? obj)`
- **null?**: 检查是否为空值 `(null? obj)`
- **pair?**: 检查是否为序对 `(pair? obj)`
- **list?**: 检查是否为列表 `(list? obj)`
- **procedure?**: 检查是否为过程 `(procedure? obj)`

### 列表操作
- **cons**: 构造序对 `(cons a b)`
- **car**: 获取序对的第一个元素 `(car pair)`
- **cdr**: 获取序对的第二个元素 `(cdr pair)`
- **list**: 构造列表 `(list a b c ...)`

### 变异操作
- **set-car!**: 修改序对的第一个元素 `(set-car! pair value)`
- **set-cdr!**: 修改序对的第二个元素 `(set-cdr! pair value)`

### 逻辑运算
- **not**: 逻辑非 `(not expr)`

### 输入输出
- **display**: 显示值 `(display obj)`
  - 输出对象的字符串表示，字符串不显示引号
  - 支持转义字符：`\n` (换行), `\t` (制表符), `\"` (引号), `\\` (反斜杠)

### 系统函数
- **void**: 返回空值 `(void)`
- **exit**: 退出解释器 `(exit)`

## 保留字和特殊形式 (Reserved Words)

以下关键字具有特殊语义，不能作为变量名使用：

- **let**: 局部绑定
- **lambda**: 函数定义
- **letrec**: 递归绑定
- **if**: 条件判断
- **begin**: 顺序执行
- **quote**: 引用 (也可写作 `'`)
- **define**: 变量/函数定义
- **set!**: 变量赋值
- **and**: 逻辑与
- **or**: 逻辑或
- **cond**: 多路条件

## 语法糖 (Syntactic Sugar)

### 函数定义简写
传统写法：
```scheme
(define factorial (lambda (n) (if (= n 0) 1 (* n (factorial (- n 1))))))
```

语法糖写法：
```scheme
(define (factorial n) (if (= n 0) 1 (* n (factorial (- n 1)))))
```

### 多表达式函数体
支持在函数定义中使用多个表达式：
```scheme
(define (print-and-add x y)
  (display "Adding numbers")
  (+ x y))
```
等价于：
```scheme
(define print-and-add 
  (lambda (x y) 
    (begin 
      (display "Adding numbers")
      (+ x y))))
```

### 引用简写
- `'expr` 等价于 `(quote expr)`
- `'(1 2 3)` 等价于 `(quote (1 2 3))`

## 高级功能 (Advanced Features)

### 词法作用域和闭包
解释器支持完整的词法作用域，函数可以捕获定义时的环境：

```scheme
(define (make-counter)
  (let ((count 0))
    (lambda ()
      (set! count (+ count 1))
      count)))

(define counter (make-counter))
(counter) ; => 1
(counter) ; => 2
```

### 相互递归支持
通过延迟求值机制支持相互递归函数定义：

```scheme
(define (even? n)
  (if (= n 0) #t (odd? (- n 1))))

(define (odd? n)
  (if (= n 0) #f (even? (- n 1))))
```

### 内部定义
支持在函数体内使用 `define`：

```scheme
(define (outer x)
  (define (inner y) (+ x y))
  (inner 10))
```

### REPL 输出控制
- 大多数表达式会显示其求值结果
- `define`、`set!`、`set-car!`、`set-cdr!`、`display` 不显示返回值，只显示空行
- 错误会显示 "RuntimeError"

### 字符串转义支持
字符串字面量支持转义字符：
```scheme
"Hello\nWorld"  ; 包含换行符
"Say \"Hello\"" ; 包含引号
"Tab\there"     ; 包含制表符
"Path\\file"    ; 包含反斜杠
```

### 输入输出功能
```scheme
;; 显示不同类型的值
(display "Hello, World!")  ; 输出: Hello, World!
(display 42)               ; 输出: 42
(display '(1 2 3))         ; 输出: (1 2 3)

;; 组合使用进行格式化输出
(begin
  (display "The answer is: ")
  (display 42)
  (display "\n"))
```

### 动态数据结构
支持创建和操作复杂数据结构，如队列、栈等：

```scheme
;; 队列实现示例
(define (make-queue) (cons '() '()))
(define (enqueue! queue obj) ...)
(define (dequeue! queue) ...)
```

## 实现特点

1. **内存管理**: 使用 C++ 智能指针自动管理内存
2. **错误处理**: 完整的运行时错误检查和报告
3. **环境模型**: 基于关联列表的环境实现，支持嵌套作用域
4. **求值策略**: 采用标准的 Scheme 求值模型
5. **可扩展性**: 模块化设计，易于添加新功能

## 限制和注意事项

1. 不支持尾递归优化
2. 不支持宏系统
3. 不支持多值返回
4. 不支持 continuation
5. 字符类型尚未实现
6. I/O 操作有限 (目前支持 `display` 输出)

---

*本文档基于解释器当前实现状态，可能会随功能更新而变化。*
