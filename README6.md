# 🔬 Scheme 解释器 eval 功能边界情况分析

本文档详细分析每个表达式类型在求值时的边界情况、语法形式和输出结果。

## 📋 目录
1. [字面量表达式](#字面量表达式)
2. [变量和引用](#变量和引用)
3. [算术运算表达式](#算术运算表达式)
4. [比较运算表达式](#比较运算表达式)
5. [逻辑运算表达式](#逻辑运算表达式)
6. [控制流表达式](#控制流表达式)
7. [绑定和定义表达式](#绑定和定义表达式)
8. [函数相关表达式](#函数相关表达式)
9. [列表操作表达式](#列表操作表达式)
10. [类型谓词表达式](#类型谓词表达式)
11. [系统控制表达式](#系统控制表达式)

---

## 字面量表达式

### 1. Fixnum::eval() - 整数字面量

#### 语法形式
```scheme
42      ; 正整数
-17     ; 负整数
0       ; 零
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **最小整数** | `INT_MIN` | `IntegerV(INT_MIN)` | 系统支持的最小整数 |
| **最大整数** | `INT_MAX` | `IntegerV(INT_MAX)` | 系统支持的最大整数 |
| **零值** | `0` | `IntegerV(0)` | 特殊数值零 |
| **正常整数** | `42` | `IntegerV(42)` | 普通正整数 |
| **负整数** | `-17` | `IntegerV(-17)` | 普通负整数 |

#### 实现代码
```cpp
Value Fixnum::eval(Assoc &e) {
    return IntegerV(n);  // 直接返回整数值对象
}
```

#### 不会失败的情况
- 整数字面量求值永远成功
- 不依赖环境状态
- 不进行任何计算或检查

---

### 2. StringExpr::eval() - 字符串字面量

#### 语法形式
```scheme
"hello world"    ; 基本字符串
""              ; 空字符串
"say \"hi\""    ; 包含转义字符
"line1\nline2"  ; 包含换行符
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **空字符串** | `""` | `StringV("")` | 长度为0的字符串 |
| **单字符** | `"a"` | `StringV("a")` | 最短非空字符串 |
| **长字符串** | `"very long..."` | `StringV("very long...")` | 任意长度字符串 |
| **转义字符** | `"hello\nworld"` | `StringV("hello\nworld")` | 包含特殊字符 |
| **引号字符串** | `"say \"hi\""` | `StringV("say \"hi\"")` | 包含内嵌引号 |

#### 实现代码
```cpp
Value StringExpr::eval(Assoc &e) {
    return StringV(s);  // 直接返回字符串值对象
}
```

---

### 3. True::eval() / False::eval() - 布尔字面量

#### 语法形式
```scheme
#t    ; 真值
#f    ; 假值
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **真值** | `#t` | `BooleanV(true)` | 布尔真值 |
| **假值** | `#f` | `BooleanV(false)` | 布尔假值 |

#### 实现代码
```cpp
Value True::eval(Assoc &e) {
    return BooleanV(true);
}

Value False::eval(Assoc &e) {
    return BooleanV(false);
}
```

---

## 变量和引用

### 4. Var::eval() - 变量求值

#### 语法形式
```scheme
x           ; 用户定义变量
+           ; 原语函数名
undefined   ; 未定义变量（错误）
```

#### 边界情况
| 情况 | 输入 | 输出 | 错误信息 |
|------|------|------|----------|
| **用户变量存在** | 环境中已定义的变量 | 变量的值 | - |
| **原语函数** | `+`, `-`, `*`, `/` 等 | 动态创建的`ProcedureV` | - |
| **变量未定义** | 环境中不存在的变量 | - | `"undefined variable"` |
| **非法变量名** | 以数字开头 | - | `"Wrong variable name"` |
| **包含#字符** | `x#y` | - | `"undefined variable"` |
| **空变量名** | `""` | - | `"Wrong variable name"` |

#### 实现代码
```cpp
Value Var::eval(Assoc &e) {
    // 验证变量名格式
    if ((x.empty()) || (std::isdigit(x[0]) || x[0] == '.' || x[0] == '@')) 
        throw RuntimeError("Wrong variable name");
    for (int i = 0; i < x.size(); i++) {
        if (x[i] == '#') {
            throw(RuntimeError("undefined variable"));
        }
    }

    // 在环境中查找变量
    Value matched_value = find(x, e);
    if (matched_value.get() == nullptr) {
        // 检查是否为原语函数
        if (primitives.count(x)) {
            // 动态创建原语函数的 Procedure 包装
            return create_primitive_procedure(x);
        } else {
            throw(RuntimeError("undefined variable"));
        }
    }
    return matched_value;
}
```

#### 原语函数动态包装
```cpp
// 支持的原语函数类型
case E_PLUS:    { exp = new Plus(new Var("parm1"), new Var("parm2")); break; }
case E_MINUS:   { exp = new Minus(new Var("parm1"), new Var("parm2")); break; }
case E_MUL:     { exp = new Mult(new Var("parm1"), new Var("parm2")); break; }
case E_DIV:     { exp = new Div(new Var("parm1"), new Var("parm2")); break; }
// ... 更多原语函数
```

---

### 5. Quote::eval() - 引用表达式

#### 语法形式
```scheme
'symbol           ; 引用符号
'(1 2 3)         ; 引用列表
'"hello"         ; 引用字符串
'#t              ; 引用布尔值
'()              ; 引用空列表
'(a . b)         ; 引用点对
```

#### 边界情况
| 情况 | 输入语法 | 输出 | 说明 |
|------|----------|------|------|
| **符号** | `'hello` | `SymbolV("hello")` | 符号值 |
| **数字** | `'42` | `IntegerV(42)` | 数字值 |
| **字符串** | `'"text"` | `StringV("text")` | 字符串值 |
| **布尔值** | `'#t` | `BooleanV(true)` | 布尔值 |
| **空列表** | `'()` | `NullV()` | 空列表 |
| **单元素列表** | `'(a)` | `PairV(SymbolV("a"), NullV())` | 单元素列表 |
| **多元素列表** | `'(1 2 3)` | 递归构建的对偶链 | 正常列表 |
| **点对** | `'(a . b)` | `PairV(SymbolV("a"), SymbolV("b"))` | 不规范列表 |
| **嵌套结构** | `'((a b) c)` | 嵌套的对偶结构 | 复杂嵌套 |

#### 实现代码
```cpp
Value Quote::eval(Assoc& e) {
    if (dynamic_cast<TrueSyntax*>(s.get())) 
        return BooleanV(true);
    else if (dynamic_cast<FalseSyntax*>(s.get())) 
        return BooleanV(false);
    else if (dynamic_cast<Number*>(s.get()))
        return IntegerV(dynamic_cast<Number*>(s.get())->n);
    else if (dynamic_cast<SymbolSyntax*>(s.get())) 
        return SymbolV(dynamic_cast<SymbolSyntax*>(s.get())->s);
    else if (dynamic_cast<StringSyntax*>(s.get())) 
        return StringV(dynamic_cast<StringSyntax*>(s.get())->s);
    else if (dynamic_cast<List*>(s.get())) {
        return convert_list_to_pairs(s);  // 复杂的列表转换逻辑
    } else 
        throw(RuntimeError("Unknown quoted typename"));
}
```

#### 点对语法验证
- 点符号只能出现一次
- 点符号必须在倒数第二个位置
- `(a . b . c)` 是非法的
- `(a .)` 是非法的

---

## 算术运算表达式

### 6. Plus::evalRator() - 二元加法

#### 语法形式
```scheme
(+ 1 2)          ; 整数加法
(+ 1/2 3)        ; 有理数与整数
(+ 1/3 2/5)      ; 有理数加法
```

#### 边界情况
| 情况 | 输入类型 | 输出 | 错误处理 |
|------|----------|------|----------|
| **整数+整数** | `IntegerV`, `IntegerV` | `IntegerV` | 可能溢出 |
| **有理数+整数** | `RationalV`, `IntegerV` | `RationalV` | 通分计算 |
| **整数+有理数** | `IntegerV`, `RationalV` | `RationalV` | 通分计算 |
| **有理数+有理数** | `RationalV`, `RationalV` | `RationalV` | 复杂通分 |
| **类型错误** | 其他类型组合 | - | `"Wrong typename"` |

#### 实现代码
```cpp
Value Plus::evalRator(const Value &rand1, const Value &rand2) {
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) + 
                       (dynamic_cast<Integer*>(rand2.get())->n));
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_INT) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        int n2 = dynamic_cast<Integer*>(rand2.get())->n;
        return RationalV(rat1->numerator + n2 * rat1->denominator, rat1->denominator);
    } 
    // ... 其他类型组合
    throw(RuntimeError("Wrong typename"));
}
```

---

### 7. PlusVar::evalRator() - 多参数加法

#### 语法形式
```scheme
(+)              ; 零参数，返回0
(+ 5)            ; 单参数，返回参数本身  
(+ 1 2 3 4)      ; 多参数求和
```

#### 边界情况
| 情况 | 参数个数 | 输入示例 | 输出 |
|------|----------|----------|------|
| **零参数** | 0 | `(+)` | `IntegerV(0)` |
| **单参数** | 1 | `(+ 5)` | `IntegerV(5)` |
| **两参数** | 2 | `(+ 1 2)` | `IntegerV(3)` |
| **多参数** | n≥3 | `(+ 1 2 3 4)` | `IntegerV(10)` |
| **溢出情况** | - | 极大数相加 | 可能整数溢出 |

#### 实现代码
```cpp
Value PlusVar::evalRator(const std::vector<Value> &args) {
    if (args.empty()) {
        return IntegerV(0); // 加法单位元
    }
    
    int result = 0;
    for (const auto& arg : args) {
        if (arg->v_type == V_INT) {
            result += dynamic_cast<Integer*>(arg.get())->n;
        } else {
            throw(RuntimeError("Wrong typename"));
        }
    }
    return IntegerV(result);
}
```

---

### 8. Minus / MinusVar - 减法运算

#### 语法形式
```scheme
(- 5 3)          ; 二元减法
(- 10)           ; 取负数
(- 10 3 2)       ; 多参数减法 (10-3-2=5)
```

#### 边界情况
| 情况 | 参数个数 | 输入 | 输出 | 错误处理 |
|------|----------|------|------|----------|
| **零参数** | 0 | `(-)` | - | `"Wrong number of arguments for -"` |
| **取负** | 1 | `(- 5)` | `IntegerV(-5)` | - |
| **二元减法** | 2 | `(- 8 3)` | `IntegerV(5)` | - |
| **多元减法** | n≥3 | `(- 10 3 2)` | `IntegerV(5)` | 从左到右依次相减 |
| **下溢** | - | `INT_MIN - 1` | 可能整数下溢 | - |

---

### 9. Div / DivVar - 除法运算

#### 语法形式
```scheme
(/ 6 2)          ; 二元除法，返回有理数
(/ 5)            ; 倒数，返回 1/5
(/ 12 3 2)       ; 多参数除法
```

#### 边界情况
| 情况 | 输入 | 输出 | 错误处理 |
|------|------|------|----------|
| **整数除法** | `(/ 6 3)` | `RationalV(2, 1)` | - |
| **产生分数** | `(/ 1 3)` | `RationalV(1, 3)` | - |
| **求倒数** | `(/ 5)` | `RationalV(1, 5)` | - |
| **除零错误** | `(/ 5 0)` | - | `"Division by zero"` |
| **零的倒数** | `(/ 0)` | - | `"Division by zero"` |
| **多参数** | `(/ 24 2 3)` | `RationalV(4, 1)` | - |

#### 实现代码
```cpp
Value Div::evalRator(const Value &rand1, const Value &rand2) {
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        int dividend = dynamic_cast<Integer*>(rand1.get())->n;
        int divisor = dynamic_cast<Integer*>(rand2.get())->n;
        if (divisor == 0) {
            throw(RuntimeError("Division by zero"));
        }
        return RationalV(dividend, divisor);  // 总是返回有理数
    }
    throw(RuntimeError("Wrong typename"));
}
```

---

## 比较运算表达式

### 10. Less / LessVar - 小于比较

#### 语法形式
```scheme
(< 1 2)          ; 二元比较
(< 1 2 3 4)      ; 多参数链式比较
(< 5 3)          ; 返回假的比较
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **真比较** | `(< 1 2)` | `BooleanV(true)` | 1 < 2 |
| **假比较** | `(< 3 2)` | `BooleanV(false)` | 3 不< 2 |
| **相等** | `(< 2 2)` | `BooleanV(false)` | 2 不< 2 |
| **链式真** | `(< 1 2 3 4)` | `BooleanV(true)` | 1<2<3<4 |
| **链式假** | `(< 1 3 2 4)` | `BooleanV(false)` | 3不<2，短路 |
| **参数不足** | `(< 1)` | - | `"< requires at least 2 arguments"` |

#### 实现代码
```cpp
Value LessVar::evalRator(const std::vector<Value> &args) {
    if (args.size() < 2) {
        throw(RuntimeError("< requires at least 2 arguments"));
    }
    
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (args[i]->v_type != V_INT || args[i+1]->v_type != V_INT) {
            throw(RuntimeError("Wrong typename"));
        }
        int n1 = dynamic_cast<Integer*>(args[i].get())->n;
        int n2 = dynamic_cast<Integer*>(args[i+1].get())->n;
        if (!(n1 < n2)) {
            return BooleanV(false);  // 短路求值
        }
    }
    return BooleanV(true);
}
```

---

### 11. Equal / EqualVar - 相等比较

#### 语法形式
```scheme
(= 2 2)          ; 二元相等
(= 1 1 1 1)      ; 多参数相等
(= 2 3)          ; 不相等
```

#### 边界情况
| 情况 | 输入 | 输出 | 支持类型 |
|------|------|------|----------|
| **整数相等** | `(= 5 5)` | `BooleanV(true)` | V_INT |
| **整数不等** | `(= 5 3)` | `BooleanV(false)` | V_INT |
| **多参数全等** | `(= 2 2 2)` | `BooleanV(true)` | 链式检查 |
| **多参数不等** | `(= 2 2 3)` | `BooleanV(false)` | 短路返回 |
| **有理数比较** | `(= 1/2 2/4)` | `BooleanV(true)` | 化简后比较 |

---

## 逻辑运算表达式

### 12. And::eval() - 逻辑与

#### 语法形式
```scheme
(and)            ; 空and，返回#t
(and #t #t #t)   ; 全真，返回最后值
(and #t #f #t)   ; 包含假值，短路返回#f
(and 1 2 3)      ; 非布尔值，返回最后值
```

#### 边界情况
| 情况 | 输入 | 输出 | 行为说明 |
|------|------|------|----------|
| **空参数** | `(and)` | `BooleanV(true)` | 逻辑与单位元 |
| **单参数真** | `(and #t)` | `BooleanV(true)` | 返回参数值 |
| **单参数假** | `(and #f)` | `BooleanV(false)` | 返回参数值 |
| **全真值** | `(and 1 2 3)` | `IntegerV(3)` | 返回最后值 |
| **包含假** | `(and 1 #f 3)` | `BooleanV(false)` | 短路求值 |
| **混合类型** | `(and "hello" 42)` | `IntegerV(42)` | 非#f都是真 |

#### 实现代码
```cpp
Value And::eval(Assoc &e) {
    if (es.size() == 0) return BooleanV(true);
    
    for (int i = 0; i < es.size(); i++) {
        Value val = es[i]->eval(e);
        // 在 Scheme 中，只有 #f 是假值
        if (val->v_type == V_BOOL) {
            Boolean* b = dynamic_cast<Boolean*>(val.get());
            if (!b->b) {  // 如果是 #f
                return BooleanV(false);  // 短路求值
            }
        }
        // 如果是最后一个参数，返回它的值
        if (i == es.size() - 1) {
            return val;
        }
    }
    return BooleanV(true);
}
```

---

### 13. Or::eval() - 逻辑或

#### 语法形式
```scheme
(or)             ; 空or，返回#f
(or #f #f #t)    ; 包含真值，返回首个真值
(or #f #f #f)    ; 全假，返回#f
(or 1 2 3)       ; 返回首个非#f值
```

#### 边界情况
| 情况 | 输入 | 输出 | 行为说明 |
|------|------|------|----------|
| **空参数** | `(or)` | `BooleanV(false)` | 逻辑或零元 |
| **全假值** | `(or #f #f)` | `BooleanV(false)` | 最后返回#f |
| **首个真** | `(or #t #f)` | `BooleanV(true)` | 短路返回首个真值 |
| **非布尔真** | `(or #f 42)` | `IntegerV(42)` | 返回首个非#f值 |
| **混合类型** | `(or #f "hello")` | `StringV("hello")` | 字符串是真值 |

---

### 14. Not::evalRator() - 逻辑非

#### 语法形式
```scheme
(not #t)         ; 对真值取非
(not #f)         ; 对假值取非
(not 42)         ; 对非布尔值取非
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **非真值** | `(not #t)` | `BooleanV(false)` | #t变#f |
| **非假值** | `(not #f)` | `BooleanV(true)` | 只有#f变#t |
| **非数字** | `(not 0)` | `BooleanV(false)` | 0不是假值 |
| **非字符串** | `(not "")` | `BooleanV(false)` | 空串不是假值 |
| **非空列表** | `(not '())` | `BooleanV(false)` | 空列表不是假值 |

#### 实现代码
```cpp
Value Not::evalRator(const Value &rand) {
    if (rand->v_type == V_BOOL and (dynamic_cast<Boolean*>(rand.get())->b == false))
        return BooleanV(true);
    else
        return BooleanV(false);
}
```

---

## 控制流表达式

### 15. If::eval() - 条件表达式

#### 语法形式
```scheme
(if #t 1 2)      ; 标准条件
(if #f 1 2)      ; 假条件
(if 0 1 2)       ; 非布尔条件
(if '() 1 2)     ; 空列表条件
```

#### 边界情况
| 情况 | 条件 | 输出 | 说明 |
|------|------|------|------|
| **真条件** | `#t` | 执行then分支 | 唯一的真布尔值 |
| **假条件** | `#f` | 执行else分支 | 唯一的假值 |
| **数字条件** | `0`, `42`, `-1` | 执行then分支 | 所有数字都是真值 |
| **字符串条件** | `""`, `"hello"` | 执行then分支 | 所有字符串都是真值 |
| **列表条件** | `'()`, `'(1 2)` | 执行then分支 | 所有列表都是真值 |
| **符号条件** | `'symbol` | 执行then分支 | 所有符号都是真值 |

#### 实现代码
```cpp
Value If::eval(Assoc &e) {
    Value valueof_condition = cond->eval(e);
    // 只有当条件是 Boolean 类型且值为 false 时，才返回 alter 分支
    if (valueof_condition->v_type == V_BOOL && 
        dynamic_cast<Boolean*>(valueof_condition.get())->b == false) {
        return alter->eval(e);
    } else {
        return conseq->eval(e);
    }
}
```

---

### 16. Cond::eval() - 多路条件

#### 语法形式
```scheme
(cond 
  ((< x 0) "negative")
  ((= x 0) "zero") 
  ((> x 0) "positive"))

(cond 
  (#f "never")
  (else "default"))
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **首分支匹配** | 第一个条件为真 | 执行首分支体 | 短路求值 |
| **中间分支匹配** | 中间条件为真 | 执行对应分支 | 跳过前面分支 |
| **else分支** | 前面都为假 | 执行else分支 | 默认分支 |
| **无匹配分支** | 所有条件都为假 | `VoidV()` | 返回void |
| **空else分支** | `(else)` | `VoidV()` | else无表达式 |
| **单条件分支** | `(cond (#t))` | 返回条件值`#t` | 无表达式体 |

#### 复杂示例
```scheme
;; 多表达式分支
(cond 
  ((> x 0) 
   (display "positive")
   (display "!")
   x))  ; 返回x的值

;; 嵌套cond
(cond 
  ((number? x) 
   (cond 
     ((> x 0) "positive number")
     (else "non-positive number")))
  (else "not a number"))
```

---

### 17. Begin::eval() - 顺序执行

#### 语法形式
```scheme
(begin)                    ; 空begin
(begin 1 2 3)             ; 多表达式
(begin 
  (define x 1)
  (define y 2)
  (+ x y))                ; 包含内部定义
```

#### 边界情况
| 情况 | 输入 | 输出 | 处理方式 |
|------|------|------|----------|
| **空begin** | `(begin)` | `VoidV()` | 无表达式执行 |
| **单表达式** | `(begin 42)` | `IntegerV(42)` | 返回唯一表达式值 |
| **多表达式** | `(begin 1 2 3)` | `IntegerV(3)` | 返回最后表达式值 |
| **纯内部定义** | 只含define | `VoidV()` | 类似letrec语义 |
| **混合定义** | define+表达式 | 最后表达式值 | 定义在前，表达式在后 |

#### 内部定义处理
```cpp
// 查找连续的内部定义
std::vector<std::pair<std::string, Expr>> internal_defs;
int first_non_define = 0;

for (int i = 0; i < es.size(); i++) {
    if (es[i]->e_type == E_DEFINE) {
        Define* def = dynamic_cast<Define*>(es[i].get());
        if (def) {
            internal_defs.push_back({def->var, def->e});
            first_non_define = i + 1;
        } else {
            break;
        }
    } else {
        break;
    }
}
```

---

## 绑定和定义表达式

### 18. Let::eval() - 局部绑定

#### 语法形式
```scheme
(let () 42)                        ; 空绑定
(let ((x 1)) x)                    ; 单绑定
(let ((x 1) (y 2)) (+ x y))       ; 多绑定
(let ((x 1) (x 2)) x)             ; 变量屏蔽
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **空绑定** | `(let () 42)` | `IntegerV(42)` | 无局部变量 |
| **单绑定** | `(let ((x 5)) x)` | `IntegerV(5)` | 基本绑定 |
| **变量屏蔽** | `(let ((x 1) (x 2)) x)` | `IntegerV(2)` | 后绑定覆盖前绑定 |
| **外层变量** | 引用外层同名变量 | 外层值 | let绑定不相互依赖 |
| **表达式绑定** | `(let ((x (+ 1 2))) x)` | `IntegerV(3)` | 绑定计算结果 |

#### 求值顺序
```cpp
Value Let::eval(Assoc &env) {
    // 第一阶段：在原环境中求值所有绑定表达式
    std::vector<std::pair<std::string, Value>> tobind;
    for (auto binded_pair : bind) {
        tobind.push_back({binded_pair.first, binded_pair.second->eval(env)});
    }
    
    // 第二阶段：扩展环境
    Assoc cur_env = env;
    for (auto binded_pair : tobind) {
        cur_env = extend(binded_pair.first, binded_pair.second, cur_env);
    }
    
    // 第三阶段：在新环境中求值body
    return body->eval(cur_env);
}
```

---

### 19. Letrec::eval() - 递归绑定

#### 语法形式
```scheme
(letrec ((fact (lambda (n) 
                 (if (<= n 1) 1 
                     (* n (fact (- n 1)))))))
  (fact 5))

(letrec ((even? (lambda (n) (if (= n 0) #t (odd? (- n 1)))))
         (odd?  (lambda (n) (if (= n 0) #f (even? (- n 1))))))
  (even? 10))
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **简单递归** | 阶乘函数 | 计算结果 | 自引用函数 |
| **相互递归** | even?/odd? | 计算结果 | 函数间相互调用 |
| **非递归使用** | 普通变量绑定 | 正常工作 | 兼容let语义 |
| **前向引用** | 后定义的函数 | 正确解析 | 环境预构建 |

#### 实现策略
```cpp
Value Letrec::eval(Assoc &env) {
    // 1. 创建新作用域 env1
    Assoc env1 = env;

    // 2. 将所有变量与 nullptr 绑定
    for (const auto &binding : bind) {
        env1 = extend(binding.first, Value(nullptr), env1);
    }

    // 3. 在 env1 下求值所有表达式
    std::vector<std::pair<std::string,Value>> bindings;
    for (const auto &binding : bind) {
        bindings.push_back(std::make_pair(binding.first, binding.second->eval(env1)));
    }

    // 4. 更新绑定为实际值
    Assoc env2 = env1;
    for (const auto &binding: bindings) {
        modify(binding.first, binding.second, env2);
    }

    // 5. 在最终环境中求值body
    return body->eval(env2);
}
```

---

### 20. Define::eval() - 变量/函数定义

#### 语法形式
```scheme
(define x 42)                    ; 变量定义
(define (square x) (* x x))      ; 函数定义语法糖
(define (fact n)                 ; 递归函数
  (if (<= n 1) 1 (* n (fact (- n 1)))))
```

#### 边界情况
| 情况 | 输入 | 输出 | 错误处理 |
|------|------|------|----------|
| **简单变量** | `(define x 5)` | `VoidV()` | 创建绑定 |
| **函数定义** | `(define (f x) x)` | `VoidV()` | 创建lambda |
| **重新定义** | 重定义已存在变量 | `VoidV()` | 更新绑定 |
| **递归函数** | 自引用函数 | `VoidV()` | 占位符机制 |
| **重定义原语** | `(define + -)` | - | `"Cannot redefine primitive"` |

#### 递归支持机制
```cpp
Value Define::eval(Assoc &env) {
    // 检查是否重新定义原语
    if (primitives.count(var) || reserved_words.count(var)) {
        throw RuntimeError("Cannot redefine primitive: " + var);
    }
    
    // 先创建占位符绑定支持递归
    env = extend(var, Value(nullptr), env);
    
    // 在包含占位符的环境中求值表达式
    Value val = e->eval(env);
    
    // 更新为实际值
    modify(var, val, env);
    
    return VoidV();
}
```

---

### 21. Set::eval() - 变量赋值

#### 语法形式
```scheme
(set! x 20)                    ; 修改已存在变量
(set! undefined-var 10)        ; 错误：变量未定义
```

#### 边界情况
| 情况 | 输入 | 输出 | 错误处理 |
|------|------|------|----------|
| **正常赋值** | 已存在变量 | `VoidV()` | 修改绑定 |
| **未定义变量** | 不存在的变量 | - | `"Undefined variable in set!"` |
| **修改闭包变量** | 闭包捕获的变量 | `VoidV()` | 修改外层绑定 |

---

## 函数相关表达式

### 22. Lambda::eval() - 函数创建

#### 语法形式
```scheme
(lambda () 42)                 ; 无参函数
(lambda (x) x)                 ; 单参函数
(lambda (x y z) (+ x y z))     ; 多参函数
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **无参lambda** | `(lambda () 42)` | `ProcedureV([], body, env)` | 空参数列表 |
| **单参lambda** | `(lambda (x) x)` | `ProcedureV(["x"], body, env)` | 标准情况 |
| **多参lambda** | `(lambda (x y) (+ x y))` | `ProcedureV(["x","y"], body, env)` | 多个参数 |
| **闭包捕获** | 引用外层变量的lambda | 捕获外层环境 | 词法作用域 |

#### 实现代码
```cpp
Value Lambda::eval(Assoc &env) {
    Assoc new_env = env;  // 捕获当前环境
    return ProcedureV(x, e, new_env);  // 创建闭包
}
```

---

### 23. Apply::eval() - 函数调用

#### 语法形式
```scheme
(func arg1 arg2)               ; 基本函数调用
((lambda (x) x) 42)           ; lambda直接调用
(+ 1 2 3)                     ; 原语函数调用
```

#### 边界情况
| 情况 | 输入 | 输出 | 错误处理 |
|------|------|------|----------|
| **正常调用** | 参数匹配的调用 | 函数执行结果 | - |
| **参数不匹配** | 参数个数错误 | - | `"Wrong number of arguments"` |
| **非函数调用** | 对非procedure调用 | - | `"Attempt to apply a non-procedure"` |
| **原语调用** | 调用+、-等原语 | 计算结果 | 动态包装 |
| **递归调用** | 函数自调用 | 递归结果或栈溢出 | - |

#### 五阶段执行过程
```cpp
Value Apply::eval(Assoc &e) {
    // 阶段1：求值函数表达式
    Value mid_fun = rator->eval(e);
    if (mid_fun->v_type != V_PROC) {
        throw RuntimeError("Attempt to apply a non-procedure");
    }

    Procedure* clos_ptr = dynamic_cast<Procedure*>(mid_fun.get());
    
    // 阶段2：求值参数表达式
    std::vector<Value> args;
    for (int i = 0; i < rand.size(); i++) {
        args.push_back(rand[i]->eval(e));
    }

    // 阶段3：参数数量验证
    if (args.size() != clos_ptr->parameters.size()) {
        throw RuntimeError("Wrong number of arguments");
    }

    // 阶段4：构建执行环境
    Assoc param_env = clos_ptr->env;
    for (int i = 0; i < clos_ptr->parameters.size(); i++) {
        param_env = extend(clos_ptr->parameters[i], args[i], param_env);
    }

    // 阶段5：执行函数体
    return clos_ptr->e->eval(param_env);
}
```

---

## 列表操作表达式

### 24. Cons::evalRator() - 构造对偶

#### 语法形式
```scheme
(cons 1 2)                     ; 基本对偶
(cons 1 '())                   ; 列表元素
(cons '(1 2) '(3 4))          ; 嵌套列表
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **基本对偶** | `(cons 1 2)` | `PairV(IntegerV(1), IntegerV(2))` | 点对 |
| **列表构造** | `(cons 1 '())` | `PairV(IntegerV(1), NullV())` | 单元素列表 |
| **嵌套结构** | `(cons '(1) '(2))` | 嵌套的对偶结构 | 复杂列表 |
| **混合类型** | `(cons #t "hello")` | `PairV(BooleanV(true), StringV("hello"))` | 不同类型 |

---

### 25. Car::evalRator() / Cdr::evalRator() - 列表访问

#### 语法形式
```scheme
(car (cons 1 2))               ; 获取第一个元素
(cdr (cons 1 2))               ; 获取第二个元素
(car '(1 2 3))                 ; 列表第一个元素
(cdr '(1 2 3))                 ; 列表剩余部分
```

#### 边界情况
| 情况 | 输入 | 输出 | 错误处理 |
|------|------|------|----------|
| **对偶car** | `(car (cons 1 2))` | `IntegerV(1)` | - |
| **对偶cdr** | `(cdr (cons 1 2))` | `IntegerV(2)` | - |
| **列表car** | `(car '(1 2 3))` | `IntegerV(1)` | - |
| **列表cdr** | `(cdr '(1 2 3))` | `PairV(2, PairV(3, NullV()))` | - |
| **非对偶错误** | `(car 42)` | - | `"Wrong typename"` |
| **空列表错误** | `(car '())` | - | `"Wrong typename"` |

---

### 26. SetCar::evalRator() / SetCdr::evalRator() - 破坏性修改

#### 语法形式
```scheme
(define p (cons 1 2))
(set-car! p 10)                ; 修改第一个元素
(set-cdr! p 20)                ; 修改第二个元素
```

#### 边界情况
| 情况 | 输入 | 输出 | 错误处理 |
|------|------|------|----------|
| **修改car** | `(set-car! pair new-val)` | `VoidV()` | 破坏性修改 |
| **修改cdr** | `(set-cdr! pair new-val)` | `VoidV()` | 破坏性修改 |
| **非对偶错误** | `(set-car! 42 1)` | - | `"argument must be a pair"` |
| **修改共享** | 修改被多处引用的对偶 | 影响所有引用 | 副作用 |

---

### 27. ListFunc::evalRator() - 列表构造

#### 语法形式
```scheme
(list)                         ; 空列表
(list 1)                       ; 单元素列表
(list 1 2 3)                   ; 多元素列表
(list 'a "b" 3)                ; 混合类型列表
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **空列表** | `(list)` | `NullV()` | 无参数 |
| **单元素** | `(list 42)` | `PairV(IntegerV(42), NullV())` | 基本列表 |
| **多元素** | `(list 1 2 3)` | 递归构建的对偶链 | 正常列表 |
| **混合类型** | `(list #t "hi" 5)` | 包含不同类型的列表 | 类型灵活 |

#### 实现代码
```cpp
Value ListFunc::evalRator(const std::vector<Value> &args) {
    if (args.empty()) {
        return NullV(); // 空列表
    }
    
    // 从右向左构建列表
    Value result = NullV();
    for (int i = args.size() - 1; i >= 0; i--) {
        result = PairV(args[i], result);
    }
    
    return result;
}
```

---

## 类型谓词表达式

### 28. 各种类型检查谓词

#### 语法形式和边界情况

| 谓词 | 语法 | 返回#t的情况 | 返回#f的情况 |
|------|------|-------------|-------------|
| **number?** | `(number? x)` | 整数、有理数 | 其他所有类型 |
| **boolean?** | `(boolean? x)` | `#t`, `#f` | 其他所有类型 |
| **string?** | `(string? x)` | `"hello"`, `""` | 其他所有类型 |
| **symbol?** | `(symbol? x)` | `'hello`, `'x` | 其他所有类型 |
| **null?** | `(null? x)` | `'()`, `(list)` | 其他所有类型 |
| **pair?** | `(pair? x)` | `(cons 1 2)`, `'(1 2)` | 原子值、`'()` |
| **procedure?** | `(procedure? x)` | lambda、原语函数 | 其他所有类型 |

#### List? 的复杂判断
```scheme
(list? '())                    ; #t - 空列表
(list? '(1 2 3))              ; #t - 正常列表  
(list? (cons 1 2))            ; #f - 不规范列表
(list? (cons 1 (cons 2 '()))) ; #t - 以null结尾
```

#### List? 实现细节
```cpp
Value IsList::evalRator(const Value &rand) {
    if (rand->v_type == V_NULL) {
        return BooleanV(true); // 空列表是列表
    }
    
    if (rand->v_type != V_PAIR) {
        return BooleanV(false); // 不是pair就不是列表
    }
    
    // 使用快慢指针检测环形列表
    Value slow = rand;
    Value fast = rand;
    
    while (true) {
        // 快指针前进两步
        if (fast->v_type != V_PAIR) break;
        fast = dynamic_cast<Pair*>(fast.get())->cdr;
        if (fast->v_type != V_PAIR) break;
        fast = dynamic_cast<Pair*>(fast.get())->cdr;
        
        // 慢指针前进一步
        slow = dynamic_cast<Pair*>(slow.get())->cdr;
        
        // 检测环形
        if (slow.get() == fast.get()) {
            return BooleanV(false); // 环形列表不是正常列表
        }
    }
    
    // 检查最后是否以null结尾
    return BooleanV(fast->v_type == V_NULL);
}
```

---

## 系统控制表达式

### 29. MakeVoid::eval() - 显式Void

#### 语法形式
```scheme
(void)                         ; 显式创建void值
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **显式void** | `(void)` | `VoidV()` | 明确返回void |

---

### 30. Exit::eval() - 程序退出

#### 语法形式
```scheme
(exit)                         ; 退出程序
```

#### 边界情况
| 情况 | 输入 | 输出 | 说明 |
|------|------|------|------|
| **正常退出** | `(exit)` | `TerminateV()` | 程序终止信号 |

---

### 31. Display::evalRator() - 输出函数

#### 语法形式
```scheme
(display "hello")              ; 输出字符串
(display 42)                   ; 输出数字
(display #t)                   ; 输出布尔值
(display '(1 2 3))            ; 输出列表
```

#### 边界情况
| 情况 | 输入类型 | 输出到屏幕 | 返回值 |
|------|----------|------------|--------|
| **字符串** | `StringV` | 不带引号的内容 | `VoidV()` |
| **整数** | `IntegerV` | 数字形式 | `VoidV()` |
| **布尔** | `BooleanV` | `#t` 或 `#f` | `VoidV()` |
| **列表** | `PairV` | `(1 2 3)` 形式 | `VoidV()` |
| **符号** | `SymbolV` | 符号名称 | `VoidV()` |

#### 实现代码
```cpp
Value Display::evalRator(const Value &rand) {
    if (rand->v_type == V_STRING) {
        // 字符串不显示引号
        String* str_ptr = dynamic_cast<String*>(rand.get());
        std::cout << str_ptr->s;
    } else {
        // 其他类型使用标准显示方法
        rand->show(std::cout);
    }
    
    return VoidV();
}
```

---

## 完整边界情况总结表

### 按错误类型分类

#### 类型错误
| 表达式 | 错误输入 | 错误信息 | 解决方法 |
|--------|----------|----------|----------|
| **算术运算** | 非数字类型 | `"Wrong typename"` | 确保操作数为数字 |
| **比较运算** | 非数字类型 | `"Wrong typename"` | 确保操作数为数字 |
| **Car/Cdr** | 非对偶类型 | `"Wrong typename"` | 确保操作数为对偶 |
| **Apply** | 非过程类型 | `"Attempt to apply a non-procedure"` | 确保第一个参数为函数 |

#### 参数错误
| 表达式 | 错误输入 | 错误信息 | 解决方法 |
|--------|----------|----------|----------|
| **Apply** | 参数个数不匹配 | `"Wrong number of arguments"` | 检查函数签名 |
| **比较运算** | 参数过少 | `"< requires at least 2 arguments"` | 提供足够参数 |
| **减法** | 无参数 | `"Wrong number of arguments for -"` | 至少提供一个参数 |

#### 数学错误
| 表达式 | 错误输入 | 错误信息 | 解决方法 |
|--------|----------|----------|----------|
| **除法** | 除零 | `"Division by zero"` | 检查除数非零 |
| **指数运算** | 负指数 | `"Negative exponent not supported"` | 使用非负指数 |
| **指数运算** | 0^0 | `"0^0 is undefined"` | 避免此情况 |

#### 变量错误
| 表达式 | 错误输入 | 错误信息 | 解决方法 |
|--------|----------|----------|----------|
| **Var** | 未定义变量 | `"undefined variable"` | 先定义变量 |
| **Var** | 非法变量名 | `"Wrong variable name"` | 使用合法标识符 |
| **Set!** | 未定义变量 | `"Undefined variable in set!"` | 先定义再赋值 |

### 按成功情况分类

#### 总是成功的操作
- 所有字面量求值（Fixnum, String, True, False）
- Quote表达式（语法正确的情况下）
- Lambda表达式创建
- MakeVoid、Exit

#### 可能失败的操作
- 变量引用（Var）
- 函数调用（Apply）
- 算术和比较运算
- 列表操作（car, cdr在非对偶上）
- 变量赋值（set!）

#### 复杂控制流
- If：条件求值可能失败，分支选择总是成功
- Cond：各子条件求值可能失败
- Begin：任何子表达式失败都会导致整体失败
- And/Or：短路求值，提前返回

---

## 性能和内存考虑

### 栈溢出风险
| 操作 | 风险场景 | 预防措施 |
|------|----------|----------|
| **递归调用** | 深度递归无终止条件 | 检查递归深度 |
| **相互递归** | A调用B，B调用A | 正确的终止条件 |
| **Quote嵌套** | 极深的嵌套列表 | 限制嵌套深度 |

### 内存泄漏风险
| 操作 | 风险场景 | 解决方案 |
|------|----------|----------|
| **闭包创建** | 大量闭包捕获大环境 | 智能指针自动管理 |
| **环境扩展** | 长期存在的大环境 | 垃圾回收机制 |
| **循环引用** | 环形数据结构 | 弱引用或手动打破 |

---

## 总结

这个分析涵盖了Scheme解释器中所有表达式类型的：

### ✅ 完整性
- **31种表达式类型**：从基础字面量到复杂控制流
- **边界情况覆盖**：成功情况、失败情况、特殊情况
- **错误处理**：详细的错误类型和处理策略

### ✅ 实用性
- **语法示例**：每种情况都有具体的代码示例
- **实现细节**：关键代码片段和算法逻辑
- **调试指导**：错误信息和解决方法

### ✅ 系统性
- **分类组织**：按功能领域系统分类
- **交叉引用**：相关功能之间的关联分析
- **性能考虑**：内存和栈溢出等实际问题

这个文档为理解和使用Scheme解释器提供了全面的技术参考，涵盖了从基础操作到高级特性的所有边界情况和实现细节。
