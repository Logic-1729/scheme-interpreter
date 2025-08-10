# 📝 Scheme 解释器变量名命名规范

本文档详细说明了在当前 Scheme 解释器实现中变量名的命名规则、限制条件和最佳实践。

## 🔍 变量名合法性检测机制

### 检测流程

变量名的合法性检测分为两个阶段：

1. **语法分析阶段**（`syntax.cpp`）：确定哪些字符可以组成标识符 token
2. **求值阶段**（`evaluation.cpp`）：验证变量名的具体规则

## ✅ 变量名合法性规则

### 1. 基本字符组成规则

```cpp
// 来源：syntax.cpp - readItem() 函数
// 标识符可以包含除以下分隔符外的任何字符：
❌ 左括号 '('
❌ 右括号 ')'  
❌ 左方括号 '['
❌ 右方括号 ']'
❌ 分号 ';' (注释开始符)
❌ 空格字符 (isspace())
❌ 文件结束符 EOF
```

### 2. 首字符限制

```cpp
// 来源：evaluation.cpp - Var::eval() 方法
// 变量名的第一个字符不能是：
❌ 数字 (0-9)        // 避免与数字字面量冲突
❌ 点号 (.)          // 避免与浮点数冲突  
❌ @ 符号            // 保留符号
```

### 3. 内容字符限制

```cpp
// 来源：evaluation.cpp - Var::eval() 方法
❌ 不能包含 '#' 字符  // 避免与特殊字面量冲突
❌ 不能为空字符串     // 必须有实际内容
```

## ❌ 禁止使用的标识符

### 1. 特殊形式（保留字）

这些是 Scheme 的核心语法构造，具有特殊的语法和求值规则：

```scheme
;; 绑定构造
let letrec define

;; 控制流
if begin cond and or

;; 函数定义  
lambda

;; 数据构造
quote

;; 赋值
set!
```

### 2. 原语函数（内置函数）

虽然技术上可以重新绑定，但不推荐覆盖这些内置函数：

```scheme
;; 算术运算
+ - * / quotient modulo expt

;; 比较运算
< <= = >= >

;; 类型谓词
eq? boolean? number? null? pair? procedure? symbol? list? string?

;; 列表操作
cons car cdr list set-car! set-cdr!

;; 逻辑运算
not

;; 输入输出
display

;; 系统控制
void exit
```

### 3. 特殊字面量

```scheme
#t  ;; 布尔真值
#f  ;; 布尔假值
```

## ✅ 有效变量名示例

### 基本命名

```scheme
;; 简单字母变量
x y z
var temp result
hello world

;; 多单词组合
my-variable your_variable 
first-name last-name
is-valid? set-value!
```

### 包含数字

```scheme
;; 数字在非首位（合法）
var1 temp2 x123 
counter42 list-item-3
version2-0 test-case-1

;; 数字开头（非法）
❌ 123var 42test 9point5
```

### 特殊字符组合

```scheme
;; 连字符（kebab-case）
my-great-variable
compute-factorial
is-prime-number?

;; 下划线（snake_case）  
my_awesome_var
compute_result
temp_storage_1

;; 问号结尾（谓词命名约定）
empty? null? positive?
is-list? is-procedure?
valid-input? has-value?

;; 感叹号结尾（修改操作约定）
set-car! set-cdr!
my-setter! update-value!
clear-cache! reset-state!

;; 混合符号
<counter> *global-var*
++increment-- **special**
my->transformed->value
```

### 单字符变量

```scheme
;; 常用单字符
a b c d e f g h i j k l m n o p q r s t u v w x y z
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z

;; 特殊单字符符号
_ $ % ^ & * < > + - | \ / ~ `
```

### 创意命名

```scheme
;; 数学符号风格
α β γ δ ε       ;; Unicode 支持（如果终端支持）
∑ ∏ ∫ ∂ ∇      ;; 数学运算符

;; 编程风格
CamelCase PascalCase
kebab-case snake_case
CONSTANT_VALUE

;; 描述性命名
input-stream output-buffer
current-position next-element
error-message status-code
```

## 🚫 无效变量名示例

### 违反首字符规则

```scheme
❌ 123abc          ;; 数字开头
❌ 456-variable    ;; 数字开头
❌ .hidden-var     ;; 点号开头
❌ @special-name   ;; @ 符号开头
❌ 9.5-ratio       ;; 数字+点号开头
```

### 违反内容规则

```scheme
❌ hash#tag        ;; 包含 # 字符
❌ my#variable     ;; 包含 # 字符  
❌ var#42          ;; 包含 # 字符
❌ ""              ;; 空字符串
```

### 包含分隔符

```scheme
❌ "my variable"   ;; 包含空格（实际被解析为字符串）
❌ (my-var)        ;; 包含括号（实际被解析为列表）
❌ [array-var]     ;; 包含方括号（实际被解析为列表）
❌ my;comment      ;; 包含分号（后半部分被当作注释）
```

### 保留字冲突

```scheme
❌ let             ;; 特殊形式
❌ define          ;; 特殊形式  
❌ lambda          ;; 特殊形式
❌ if              ;; 特殊形式
```

## 💡 命名最佳实践

### 1. 命名约定

```scheme
;; 谓词函数：以 ? 结尾
(define (positive? x) (> x 0))
(define (empty? lst) (null? lst))
(define (valid-input? input) ...)

;; 修改函数：以 ! 结尾  
(define (set-value! var val) (set! var val))
(define (append! lst item) ...)
(define (clear! container) ...)

;; 常量：使用 * 包围或全大写
(define *pi* 3.14159)
(define *global-counter* 0)
(define MAX-SIZE 1000)

;; 私有/内部变量：使用 _ 开头
(define _internal-helper ...)
(define _temp-storage ...)
```

### 2. 多单词连接

```scheme
;; 推荐：kebab-case（Lisp 传统）
first-name last-name
compute-fibonacci-number
parse-input-string

;; 可用：snake_case
first_name last_name  
compute_fibonacci_number
parse_input_string

;; 避免：CamelCase（不符合 Lisp 习惯）
firstName lastName  ;; 不推荐但语法上合法
computeFibonacci    ;; 不推荐但语法上合法
```

### 3. 语义化命名

```scheme
;; 好的命名：清晰表达意图
input-validation-result
user-authentication-status  
file-processing-complete?

;; 避免的命名：意义不明
x1 temp2 var3
a b c result
thing stuff data
```

## 🔧 代码实现细节

### 语法分析阶段检测

```cpp
// syntax.cpp - readItem() 函数
std::string s;
do {
    int c = is.peek();
    if (c == '(' || c == ')' ||
        c == '[' || c == ']' || 
        c == ';' ||
        isspace(c) ||
        c == EOF)
      break;
    is.get();
    s.push_back(c);
} while (true);
```

### 求值阶段验证

```cpp
// evaluation.cpp - Var::eval() 方法
Value Var::eval(Assoc &e) {
    // 检查空字符串和首字符限制
    if ((x.empty()) || (std::isdigit(x[0]) || x[0] == '.' || x[0] == '@')) 
        throw RuntimeError("Wrong variable name");
    
    // 检查禁止字符
    for (int i = 0; i < x.size(); i++) {
        if (x[i] == '#') {
            throw(RuntimeError("undefined variable"));
        }
    }
    
    // 其他求值逻辑...
}
```

## 🌟 高级特性

### 1. Unicode 支持

理论上支持 UTF-8 编码的 Unicode 字符（取决于终端和系统支持）：

```scheme
;; 数学符号
λ α β γ Δ ∑ ∏

;; 其他语言字符  
变量名 переменная متغير
```

### 2. 变量遮蔽

局部变量可以遮蔽全局变量和内置函数：

```scheme
;; 全局变量遮蔽
(define x 10)
(let ((x 20)) x)  ;; 返回 20，遮蔽全局 x

;; 内置函数遮蔽（技术上可行但不推荐）
(let ((+ -)) (+ 3 2))  ;; 返回 1，+ 被重新绑定为 -
```

### 3. 作用域规则

```scheme
;; 词法作用域
(define x 1)
(define (outer)
  (define x 2)
  (define (inner) x)
  (inner))  ;; 返回 2，使用内层作用域的 x

;; 动态绑定（在 let、letrec、lambda 等中）
(let ((var1 value1)
      (var2 value2))
  ;; var1 和 var2 在此作用域内有效
  ...)
```

## 📚 相关文档

- `README4.md` - 完整语法形式文档
- `Def.cpp` - 原语函数和保留字定义
- `syntax.cpp` - 语法分析实现
- `evaluation.cpp` - 求值和变量解析实现

## ⚠️ 重要提醒

1. **保留字检查**：解释器会在解析时检查保留字冲突
2. **大小写敏感**：`Var` 和 `var` 是不同的变量
3. **作用域遮蔽**：内层作用域的变量会遮蔽外层同名变量
4. **内置函数重绑定**：虽然可以重新定义 `+` 等，但会失去原有功能
5. **错误信息**：违反规则的变量名会抛出 `RuntimeError`

遵循这些命名规范，可以编写出清晰、可维护的 Scheme 代码，避免与解释器的内部机制产生冲突。
