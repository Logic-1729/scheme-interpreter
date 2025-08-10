# 测试用例功能分类索引

本文档按功能对所有测试文件夹中的测试用例进行分类整理，帮助快速定位特定功能的测试。

## 测试文件夹概览

### 文件夹结构
- **2394/**: 包含1-17号编号测试用例（.in/.out格式）
- **score/data/**: 包含1-118号完整测试集（.in/.out格式）
- **score/more-tests/**: 包含1-7号额外测试用例
- **testcase/**: 包含96个具名功能测试文件（.scm格式）

## 按功能分类的测试用例

### 1. 基础数据类型和字面量

#### 整数和数值
**testcase目录:**
- `basic_test.scm` - 基础数值运算测试
- `check_math.scm` - 数学性质验证
- `quotient_modulo_test.scm` - 除法和取模运算
- `simple_quotient_modulo_test.scm` - 简化除法取模测试
- `final_quotient_modulo_test.scm` - 最终除法取模测试
- `expt_test.scm` - 指数运算测试
- `debug_modulo.scm` - 模运算调试

**score/data目录 (按内容推测):**
- `1.in-9.in` - 基础整数字面量
- `20.in, 21.in` - 数值类型检查

#### 布尔值
**testcase目录:**
- `and_or_test.scm` - 逻辑与或运算
- `comprehensive_type_test.scm` - 综合类型测试
- `clean_type_test.scm` - 清洁类型测试

#### 字符串
**testcase目录:**
- `string_test.scm` - 字符串类型检查
- `string_escape_test.scm` - 字符串转义测试
- `string_integration_test.scm` - 字符串集成测试
- `simple_string_test.scm` - 简单字符串测试
- `test_string.scm` - 字符串基础测试
- `test_string2.scm` - 字符串扩展测试
- `test_string_predicate.scm` - 字符串谓词测试
- `test_string_predicate_advanced.scm` - 高级字符串谓词
- `test_quote_string.scm` - 引用字符串测试

#### 符号和引用
**testcase目录:**
- `quote_test.scm` - 基础引用测试
- `quote_test2.scm` - 扩展引用测试
- `clean_quote_test.scm` - 清洁引用测试
- `comprehensive_quote_test.scm` - 综合引用测试
- `test_quote.scm` - 引用功能测试
- `test_quote_comprehensive.scm` - 全面引用测试
- `test_quote_fix.scm` - 引用修复测试
- `test_quote_simple.scm` - 简单引用测试

### 2. 列表和对偶操作

#### 基础列表操作
**testcase目录:**
- `simple_list_test.scm` - 简单列表测试
- `list_function_test.scm` - 列表函数测试
- `list_processing.scm` - 列表处理
- `commented_list_processing.scm` - 带注释列表处理
- `debug_basic_list.scm` - 基础列表调试
- `debug_list.scm` - 列表调试

**score/data目录:**
- `10.in-19.in` - cons, car, cdr基础操作
- `70.in-89.in` - 复杂列表构造和操作

#### 列表修改操作
**testcase目录:**
- `test_setcar_setcdr.scm` - set-car!/set-cdr!测试
- `test_simple_setcar.scm` - 简单set-car!测试

### 3. 控制流结构

#### 条件表达式
**testcase目录:**
- `simple_cond_test.scm` - 简单cond测试
- `cond_test2.scm` - cond测试2
- `cond_test3.scm` - cond测试3
- `complete_cond_test.scm` - 完整cond测试
- `final_cond_test.scm` - 最终cond测试
- `minimal_cond_test.scm` - 最小cond测试
- `test_cond.scm` - cond功能测试
- `test_if.scm` - if条件测试

**score/data目录:**
- `45.in-55.in` - if表达式和条件判断

### 4. 函数定义和调用

#### Lambda表达式
**testcase目录:**
- `simple_lambda_test.scm` - 简单lambda测试
- `lambda_set_test.scm` - lambda中set!测试
- `no_set_lambda_test.scm` - 不带set!的lambda

#### 函数定义语法糖
**testcase目录:**
- `test_syntax_sugar.scm` - 语法糖测试
- `syntax_sugar_showcase.scm` - 语法糖展示
- `recursive_syntax_sugar.scm` - 递归语法糖

**score/data目录:**
- `90.in-105.in` - 函数定义和调用

### 5. 作用域和绑定

#### Let绑定
**testcase目录:**
- `simple_let_set_test.scm` - 简单let set测试
- `let_begin_set_test.scm` - let begin set测试

**score/data目录:**
- `80.in-89.in` - let绑定和嵌套作用域

#### 内部定义
**testcase目录:**
- `test_internal_define.scm` - 内部define测试
- `test_delayed_define.scm` - 延迟define测试
- `comprehensive_delayed_define_test.scm` - 综合延迟define

### 6. 递归和相互递归

#### 基础递归
**testcase目录:**
- `recursion_clean.scm` - 清洁递归测试
- `recursion_showcase.scm` - 递归展示
- `recursion_tests.scm` - 递归测试集
- `advanced_recursion.scm` - 高级递归
- `math_recursion.scm` - 数学递归

#### 相互递归
**testcase目录:**
- `mutual_recursion.scm` - 相互递归
- `mutual_recursion2.scm` - 相互递归2
- `global_mutual_recursion_test.scm` - 全局相互递归

**score/data目录:**
- `100.in-118.in` - 复杂递归和相互递归

### 7. 闭包和高级作用域

#### 闭包测试
**testcase目录:**
- `simple_closure_test.scm` - 简单闭包测试
- `closure_set_test.scm` - 闭包中set!测试
- `debug_closure_test.scm` - 闭包调试

### 8. 赋值和状态修改

#### Set!操作
**testcase目录:**
- `basic_set_test.scm` - 基础set!测试
- `test_assignment.scm` - 赋值测试

### 9. 类型检查和谓词

#### 类型谓词
**testcase目录:**
- `predicates_test.scm` - 谓词函数测试
- `type_system_test.scm` - 类型系统测试

**score/data目录:**
- `20.in-30.in` - 各种类型谓词测试

### 10. 输入输出和显示

#### Display输出
**testcase目录:**
- `test_display.scm` - display功能测试
- `test_display_comprehensive.scm` - 全面display测试
- `test_escape.scm` - 转义字符测试

#### 输出行为控制
**testcase目录:**
- `test_output_behavior.scm` - 输出行为测试
- `test_set_output.scm` - set输出测试
- `test_complete_output.scm` - 完整输出测试
- `test_void_output.scm` - void输出测试
- `test_void_comprehensive.scm` - 全面void测试
- `test_no_void.scm` - 无void测试
- `test_null_values.scm` - null值测试

### 11. 特殊功能和边界测试

#### 退出功能
**testcase目录:**
- `test_exit.scm` - 退出测试
- `test_exit_edge.scm` - 退出边界测试

#### 错误处理
**testcase目录:**
- `test_errors.scm` - 错误处理测试

#### 注释处理
**testcase目录:**
- `comment_test.scm` - 注释测试

### 12. 综合和集成测试

#### 完整功能测试
**testcase目录:**
- `complete_test.scm` - 完整测试
- `final_test.scm` - 最终测试
- `clean_test.scm` - 清洁测试
- `clean_showcase.scm` - 清洁展示
- `simple_test.scm` - 简单测试
- `test_simple.scm` - 基础简单测试
- `test1.scm` - 测试1
- `test_quick.scm` - 快速测试

### 13. 实际应用示例

#### 数学应用
**testcase目录:**
- `simple_quadric.scm` - 简单二次方程
- `debug_quadric.scm` - 二次方程调试
- `sqrt_test.scm` - 平方根测试
- `test_sum_gp.scm` - 几何级数求和

#### 数据结构应用
**testcase目录:**
- `queue_test.scm` - 队列测试
- `debug_queue.scm` - 队列调试
- `test_bank_account.scm` - 银行账户模拟

#### 比较运算
**testcase目录:**
- `test_comparison.scm` - 比较运算测试

#### 属性验证
**testcase目录:**
- `verify_property.scm` - 属性验证

#### 调试和开发工具
**testcase目录:**
- `debug_basic.scm` - 基础调试
- `fee_test.scm` - 费用测试
- `define_edge_test.scm` - 定义边界测试
- `define_test.scm` - 定义测试
- `define_test_clean.scm` - 清洁定义测试
- `advanced_define_test.scm` - 高级定义测试

## 测试用例数量统计

### 按文件夹统计
- **2394/**: 17个测试用例（数值计算专用）
- **score/data/**: 118个标准测试用例  
- **score/more-tests/**: 7个额外测试用例
- **testcase/**: 96个具名功能测试

### 按功能统计
- 基础数据类型: ~15个测试
- 列表操作: ~10个测试  
- 控制流: ~12个测试
- 函数定义: ~8个测试
- 作用域绑定: ~6个测试
- 递归: ~8个测试
- 闭包: ~5个测试
- 类型检查: ~8个测试
- I/O输出: ~12个测试
- 综合测试: ~15个测试
- 其他: ~20个测试

## 使用建议

### 1. 功能开发测试顺序
推荐按以下顺序进行测试：
1. 基础数据类型 → 列表操作 → 控制流
2. 函数定义 → 作用域绑定 → 递归
3. 闭包 → 类型检查 → I/O输出
4. 综合测试验证

### 2. 调试测试选择
- 快速验证: 使用`testcase/test_quick.scm`
- 功能验证: 选择对应功能分类的测试
- 回归测试: 运行`score/data/`中的完整测试集
- 压力测试: 使用`2394/`中的大量数值测试

### 3. 测试运行命令
```bash
# 运行单个testcase测试
./build/code < testcase/basic_test.scm

# 运行score测试
./build/code < score/data/1.in

# 批量测试（使用提供的脚本）
cd score && ./score.sh
```

### 4. 测试结果分析
- `.out`文件包含期望输出
- 对比实际输出与期望输出
- 关注错误处理和边界情况
- 验证内存管理和性能表现

这个测试分类索引帮助开发者快速定位特定功能的测试用例，支持有针对性的功能验证和调试。
