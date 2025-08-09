/**
 * @file evaluation.cpp
 * @brief Expression evaluation implementation for the Scheme interpreter
 * @author luke36
 * 
 * This file implements evaluation methods for all expression types in the Scheme
 * interpreter. Functions are organized according to ExprType enumeration order
 * from Def.hpp for consistency and maintainability.
 */

#include "value.hpp"
#include "expr.hpp" 
#include "RE.hpp"
#include "syntax.hpp"
#include <cstring>
#include <vector>
#include <map>
#include <climits>

extern std::map<std::string, ExprType> primitives;
extern std::map<std::string, ExprType> reserved_words;

// ================================================================================
//                             CONTROL STRUCTURES
// ================================================================================

/**
 * @brief Evaluate Let expression
 * Creates new environment bindings and evaluates body in that context
 */
Value Let::eval(Assoc &env) {
    Assoc cur_env = env;
    std::vector<std::pair<std::string, Value>> tobind;
    for (auto binded_pair : bind) {
        tobind.push_back({binded_pair.first, binded_pair.second->eval(env)});
    }
    for (auto binded_pair : tobind) {
        cur_env = extend(binded_pair.first, binded_pair.second, cur_env);
    }
    return body->eval(cur_env);
}

/**
 * @brief Evaluate Set! expression  
 * Modifies the value of an existing variable
 */

/**
 * @brief Evaluate Lambda expression
 * Creates a closure capturing the current environment
 */
Value Lambda::eval(Assoc &env) { // lambda expression
    Assoc new_env = env;
    return ProcedureV(x, e, new_env);
}

/**
 * @brief Evaluate Apply expression
 * Evaluates function and arguments, then executes function body in extended environment
 */

Value Apply::eval(Assoc &e) {
    Value mid_fun = rator->eval(e);
    if (mid_fun->v_type != V_PROC) {throw RuntimeError("Attempt to apply a non-procedure");}

    Procedure* clos_ptr = dynamic_cast<Procedure*>(mid_fun.get());
    
    // 直接使用闭包的环境，然后为参数添加绑定
    // 关键修改：确保我们修改的是真正的闭包环境
    std::vector<Value> args;

    for (int i = 0; i < rand.size(); i++) {
        args.push_back(rand[i]->eval(e));
    }

    if (args.size() != clos_ptr->parameters.size()) {throw RuntimeError("Wrong number of arguments");}

    // 在闭包环境基础上添加参数绑定
    Assoc param_env = clos_ptr->env;
    for (int i = 0; i < clos_ptr->parameters.size(); i++) {
        param_env = extend(clos_ptr->parameters[i], args[i], param_env);
    }

    // 执行函数体
    return clos_ptr->e->eval(param_env);
}

// ================================================================================
//                           VARIABLE AND DEFINITION MANAGEMENT  
// ================================================================================

/**
 * @brief Evaluate Define expression - Delayed evaluation version supporting mutual recursion
 */
Value Define::eval(Assoc &env) {
    // 检查是否试图重新定义primitive函数
    if (primitives.count(var) || reserved_words.count(var)) {
        throw RuntimeError("Cannot redefine primitive: " + var);
    }
    
    // 为了支持递归函数，先在环境中创建一个占位符绑定
    env = extend(var, Value(nullptr), env);
    
    // 计算表达式的值（现在环境中已经有了该变量的绑定）
    Value val = e->eval(env);
    
    // 更新绑定为实际值
    modify(var, val, env);
    
    // define 返回 void
    return VoidV();
}

/**
 * @brief Evaluate Letrec expression - Batch processing of multiple define statements supporting mutual recursion
 */
Value evaluateDefineGroup(const std::vector<std::pair<std::string, Expr>>& defines, Assoc &env) {
    // 第一阶段：为所有变量创建占位符绑定
    for (const auto& def : defines) {
        if (primitives.count(def.first) || reserved_words.count(def.first)) {
            throw RuntimeError("Cannot redefine primitive: " + def.first);
        }
        env = extend(def.first, Value(nullptr), env);
    }
    
    // 第二阶段：求值所有表达式并更新绑定
    Value last_result = VoidV();
    for (const auto& def : defines) {
        Value val = def.second->eval(env);
        modify(def.first, val, env);
        last_result = VoidV(); // define 总是返回 void
    }
    
    return last_result;
}

Value Letrec::eval(Assoc &env) {
    // 1. 在当前作用域的基础上创建一个新作用域 env1
    Assoc env1 = env;

    // 2. 将 var* 与 Value(nullptr) 绑定并引入 env1
    for (const auto &binding : bind) {
        env1 = extend(binding.first, Value(nullptr), env1);
    }

    std::vector<std::pair<std::string,Value>> bindings;

    // 3. 在 env1 下对 expr* 求值
    for (const auto &binding : bind) {
        bindings.push_back(std::make_pair(binding.first, binding.second->eval(env1)));
    }

    // 4. 在 env1 的基础上创建一个新作用域 env2
    Assoc env2 = env1;

    // 5. 将 var* 与其对应的值绑定并引入 env2
    for (const auto &binding: bindings) {
        modify(binding.first, binding.second, env2);
    }

    // 6. 最后在 env2 下对 body 求值
    return body->eval(env2);
}

Value Set::eval(Assoc &env) {
    // 检查变量是否存在
    Value var_value = find(var, env);
    if (var_value.get() == nullptr) {
        throw RuntimeError("Undefined variable in set!: " + var);
    }
    
    // 计算新值
    Value new_val = e->eval(env);
    
    // 修改环境中的变量值
    modify(var, new_val, env);
    
    // set! 返回 void
    return VoidV();
}

// ================================================================================
// 基本类型和字面量
// ================================================================================

/**
 * 变量求值
 * 在环境中查找变量的值
 */
Value Var::eval(Assoc &e) { // evaluation of variable
    if ((x.empty()) || (std::isdigit(x[0]) || x[0] == '.' || x[0] == '@')) 
        throw RuntimeError("Wrong variable name");
    for (int i = 0; i < x.size(); i++) {
        if (x[i] == '#') {
            throw(RuntimeError("undefined variable"));
        }
    }

    Value matched_value = find(x, e);
    if (matched_value.get() == nullptr) {
        if (primitives.count(x)) {
            Expr exp = nullptr;
            int type_name = primitives[x];
            switch (type_name) {
                case E_MUL: { exp = (new Mult(new Var("parm1"), new Var("parm2"))); break; }
                case E_MINUS: { exp = (new Minus(new Var("parm1"), new Var("parm2"))); break; }
                case E_PLUS: { exp = (new Plus(new Var("parm1"), new Var("parm2"))); break; }
                case E_DIV: { exp = (new Div(new Var("parm1"), new Var("parm2"))); break; }
                case E_LT: { exp = (new Less(new Var("parm1"), new Var("parm2"))); break; }
                case E_LE: { exp = (new LessEq(new Var("parm1"), new Var("parm2"))); break; }
                case E_EQ: { exp = (new Equal(new Var("parm1"), new Var("parm2"))); break; }
                case E_GE: { exp = (new GreaterEq(new Var("parm1"), new Var("parm2"))); break; }
                case E_GT: { exp = (new Greater(new Var("parm1"), new Var("parm2"))); break; }
                case E_VOID: { exp = (new MakeVoid()); break; }
                case E_EQQ: { exp = (new IsEq(new Var("parm1"), new Var("parm2"))); break; }
                case E_BOOLQ: { exp = (new IsBoolean(new Var("parm"))); break; }
                case E_INTQ: { exp = (new IsFixnum(new Var("parm"))); break; }
                case E_NULLQ: { exp = (new IsNull(new Var("parm"))); break; }
                case E_PAIRQ: { exp = (new IsPair(new Var("parm"))); break; }
                case E_PROCQ: { exp = (new IsProcedure(new Var("parm"))); break; }
                case E_LISTQ: { exp = (new IsList(new Var("parm"))); break; }
                case E_SYMBOLQ: { exp = (new IsSymbol(new Var("parm"))); break; }
                case E_STRINGQ: { exp = (new IsString(new Var("parm"))); break; }
                case E_CONS: { exp = (new Cons(new Var("parm1"), new Var("parm2"))); break; }
                case E_QUOTIENT: { exp = (new Quotient(new Var("parm1"), new Var("parm2"))); break; }
                case E_MODULO: { exp = (new Modulo(new Var("parm1"), new Var("parm2"))); break; }
                case E_EXPT: { exp = (new Expt(new Var("parm1"), new Var("parm2"))); break; }
                case E_NOT: { exp = (new Not(new Var("parm"))); break; }
                case E_CAR: { exp = (new Car(new Var("parm"))); break; }
                case E_CDR: { exp = (new Cdr(new Var("parm"))); break; }
                case E_SETCAR: { exp = (new SetCar(new Var("parm1"), new Var("parm2"))); break; }
                case E_SETCDR: { exp = (new SetCdr(new Var("parm1"), new Var("parm2"))); break; }
                case E_DISPLAY: { exp = (new Display(new Var("parm"))); break; }
                case E_EXIT: { exp = (new Exit()); break; }
            }
            std::vector<std::string> parameters_;
            if (dynamic_cast<Binary*>(exp.get())) {
                parameters_.push_back("parm1");
                parameters_.push_back("parm2");
            } else if (dynamic_cast<Unary*>(exp.get())) {
                parameters_.push_back("parm");
            }
            return ProcedureV(parameters_, exp, e);
        } else {
            throw(RuntimeError("undefined variable"));
        }
    }
    return matched_value;
}

Value Fixnum::eval(Assoc &e) { // evaluation of a fixnum
    return IntegerV(n);
}

Value StringExpr::eval(Assoc &e) { // evaluation of a string
    return StringV(s);
}

Value If::eval(Assoc &e) {
    // if expression (Scheme: 只有 #f 为假，其余都为真)
    Value valueof_condition = cond->eval(e);
    // 只有当条件是 Boolean 类型且值为 false 时，才返回 alter 分支
    // 其他所有情况（包括 null、数字、符号等）都返回 conseq 分支
    if (valueof_condition->v_type == V_BOOL && 
        dynamic_cast<Boolean*>(valueof_condition.get())->b == false) {
        return alter->eval(e);
    } else {
        return conseq->eval(e);
    }
}

Value True::eval(Assoc &e) { // evaluation of #t
    return BooleanV(true);
}

Value False::eval(Assoc &e) { // evaluation of #f
    return BooleanV(false);
}

Value Begin::eval(Assoc &e) {
    if (es.size() == 0) return VoidV();
    
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
                break; // 不是连续的定义了
            }
        } else {
            break; // 不是连续的定义了
        }
    }
    
    // 如果有内部定义，使用类似 letrec 的语义
    if (!internal_defs.empty()) {
        // 创建新环境，先绑定所有变量为 nullptr
        Assoc new_env = e;
        for (const auto &def : internal_defs) {
            new_env = extend(def.first, Value(nullptr), new_env);
        }
        
        // 在新环境中求值所有定义的表达式
        for (const auto &def : internal_defs) {
            Value val = def.second->eval(new_env);
            modify(def.first, val, new_env);
        }
        
        // 在新环境中执行剩余的表达式
        if (first_non_define >= es.size()) {
            return VoidV(); // 只有定义，没有其他表达式
        }
        
        for (int i = first_non_define; i < es.size() - 1; i++) {
            es[i]->eval(new_env);
        }
        return es[es.size() - 1]->eval(new_env);
    }
    
    // 没有内部定义，使用原来的逻辑
    for (int i = 0; i < es.size() - 1; i++) {
        es[i]->eval(e);
    }
    return es[es.size() - 1]->eval(e);
}

Value And::eval(Assoc &e) {
    // (and) → #t
    if (es.size() == 0) return BooleanV(true);
    
    // 从左到右求值，遇到 #f 就返回 #f
    for (int i = 0; i < es.size(); i++) {
        Value val = es[i]->eval(e);
        // 在 Scheme 中，只有 #f 是假值，其他都是真值
        if (val->v_type == V_BOOL) {
            Boolean* b = dynamic_cast<Boolean*>(val.get());
            if (!b->b) {  // 如果是 #f
                return BooleanV(false);
            }
        }
        // 如果是最后一个参数，返回它的值
        if (i == es.size() - 1) {
            return val;
        }
    }
    return BooleanV(true); // 不应该到达这里
}

Value Or::eval(Assoc &e) {
    // (or) → #f
    if (es.size() == 0) return BooleanV(false);
    
    // 从左到右求值，遇到非 #f 就返回该值
    for (int i = 0; i < es.size(); i++) {
        Value val = es[i]->eval(e);
        // 检查是否为 #f
        bool is_false = false;
        if (val->v_type == V_BOOL) {
            Boolean* b = dynamic_cast<Boolean*>(val.get());
            is_false = !b->b;
        }
        
        if (!is_false) {  // 如果不是 #f，返回这个值
            return val;
        }
        
        // 如果是最后一个参数且为 #f，返回 #f
        if (i == es.size() - 1) {
            return BooleanV(false);
        }
    }
    return BooleanV(false); // 不应该到达这里
}

Value Cond::eval(Assoc &env) {
    // cond 表达式求值
    for (const auto &clause : clauses) {
        if (clause.empty()) continue;
        
        // 检查是否为 else 分支
        if (clause[0]->e_type == E_VAR) {
            Var* var_expr = dynamic_cast<Var*>(clause[0].get());
            if (var_expr && var_expr->x == "else") {
                // else 分支：求值所有表达式，返回最后一个
                if (clause.size() == 1) {
                    return VoidV();  // 如果 else 分支没有表达式，返回 void
                }
                Value result = VoidV();  // 初始化为 void
                for (size_t i = 1; i < clause.size(); i++) {
                    result = clause[i]->eval(env);
                }
                return result;
            }
        }
        
        // 普通分支：先求值谓词
        Value pred_value = clause[0]->eval(env);
        
        // 在 Scheme 中，只有 #f 是假值
        bool is_true = true;
        if (pred_value->v_type == V_BOOL) {
            Boolean* b = dynamic_cast<Boolean*>(pred_value.get());
            is_true = b->b;
        }
        
        if (is_true) {
            // 谓词为真，求值该分支的所有表达式，返回最后一个
            if (clause.size() == 1) {
                return pred_value;  // 如果只有谓词，返回谓词的值
            }
            Value result = VoidV();  // 初始化为 void
            for (size_t i = 1; i < clause.size(); i++) {
                result = clause[i]->eval(env);
            }
            return result;
        }
    }
    
    // 没有分支匹配，返回 void
    return VoidV();
}

Value Quote::eval(Assoc& e) {
    if (dynamic_cast<TrueSyntax*>(s.get())) 
        return BooleanV(true);
    else if (dynamic_cast<FalseSyntax*>(s.get())) 
        return BooleanV(false);
    else if (dynamic_cast<Number*>(s.get()))  // 修正：使用Number而不是Integer
        return IntegerV(dynamic_cast<Number*>(s.get())->n);
    else if (dynamic_cast<SymbolSyntax*>(s.get())) 
        return SymbolV(dynamic_cast<SymbolSyntax*>(s.get())->s);
    else if (dynamic_cast<StringSyntax*>(s.get())) 
        return StringV(dynamic_cast<StringSyntax*>(s.get())->s);
    else if (dynamic_cast<List*>(s.get())) {
        auto stxs_got = dynamic_cast<List*>(s.get())->stxs; 
        List* temp = new List;
        if (dynamic_cast<List*>(s.get())->stxs.empty()) {
            return NullV();
        } else if (stxs_got.size() == 1) {
            return PairV(Value(Quote(stxs_got[0]).eval(e)), NullV());
        } else {
            int pos = -1, cnt = 0, len = stxs_got.size();
            for (int i = 0; i < len; i++) {
                pos = (((dynamic_cast<SymbolSyntax*>(stxs_got[i].get())) && (dynamic_cast<SymbolSyntax*>(stxs_got[i].get())->s == ".")) ? (i) : (pos));
                cnt = (((dynamic_cast<SymbolSyntax*>(stxs_got[i].get())) && (dynamic_cast<SymbolSyntax*>(stxs_got[i].get())->s == ".")) ? (cnt + 1) : (cnt));
            }
            if ((cnt > 1 || ((pos != len - 2) && (cnt))) || (cnt == 1 && (len < 3))) {
                throw RuntimeError("Parm isn't fit");
            }
            if (len == 3) {
                if ((dynamic_cast<SymbolSyntax*>(stxs_got[1].get())) && (dynamic_cast<SymbolSyntax*>(stxs_got[1].get())->s == ".")) {
                    return PairV(Quote(stxs_got[0]).eval(e), Quote(stxs_got[2]).eval(e));
                }
            }
            (*temp).stxs = std::vector<Syntax>(stxs_got.begin() + 1, stxs_got.end());
            return PairV(Value(Quote(stxs_got.front()).eval(e)), Value(Quote(Syntax(temp)).eval(e)));
        }
    } else 
        throw(RuntimeError("Unknown quoted typename"));
}

Value MakeVoid::eval(Assoc &e) { // (void)
    return VoidV();
}

Value Exit::eval(Assoc &e) { // (exit)
    return TerminateV();
}

Value Binary::eval(Assoc &e) { // evaluation of two-operators primitive
    return evalRator(rand1->eval(e), rand2->eval(e));
}

Value Unary::eval(Assoc &e) { // evaluation of single-operator primitive
    return evalRator(rand->eval(e));
}

Value Variadic::eval(Assoc &e) { // evaluation of multi-operator primitive
    std::vector<Value> args;
    for (const auto& r : rands) {
        args.push_back(r->eval(e));
    }
    return evalRator(args);
}

Value Mult::evalRator(const Value &rand1, const Value &rand2) { // *
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) * (dynamic_cast<Integer*>(rand2.get())->n));
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_INT) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        int n2 = dynamic_cast<Integer*>(rand2.get())->n;
        return RationalV(rat1->numerator * n2, rat1->denominator);
    } else if (rand1->v_type == V_INT and rand2->v_type == V_RATIONAL) {
        int n1 = dynamic_cast<Integer*>(rand1.get())->n;
        auto rat2 = dynamic_cast<Rational*>(rand2.get());
        return RationalV(n1 * rat2->numerator, rat2->denominator);
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_RATIONAL) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        auto rat2 = dynamic_cast<Rational*>(rand2.get());
        return RationalV(rat1->numerator * rat2->numerator, rat1->denominator * rat2->denominator);
    }
    throw(RuntimeError("Wrong typename"));
}

Value Plus::evalRator(const Value &rand1, const Value &rand2) { // +
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) + (dynamic_cast<Integer*>(rand2.get())->n));
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_INT) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        int n2 = dynamic_cast<Integer*>(rand2.get())->n;
        return RationalV(rat1->numerator + n2 * rat1->denominator, rat1->denominator);
    } else if (rand1->v_type == V_INT and rand2->v_type == V_RATIONAL) {
        int n1 = dynamic_cast<Integer*>(rand1.get())->n;
        auto rat2 = dynamic_cast<Rational*>(rand2.get());
        return RationalV(n1 * rat2->denominator + rat2->numerator, rat2->denominator);
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_RATIONAL) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        auto rat2 = dynamic_cast<Rational*>(rand2.get());
        return RationalV(rat1->numerator * rat2->denominator + rat2->numerator * rat1->denominator, 
                        rat1->denominator * rat2->denominator);
    }
    throw(RuntimeError("Wrong typename"));
}

Value Minus::evalRator(const Value &rand1, const Value &rand2) { // -
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) - (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Div::evalRator(const Value &rand1, const Value &rand2) { // /
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        int dividend = dynamic_cast<Integer*>(rand1.get())->n;
        int divisor = dynamic_cast<Integer*>(rand2.get())->n;
        if (divisor == 0) {
            throw(RuntimeError("Division by zero"));
        }
        return RationalV(dividend, divisor);
    }
    throw(RuntimeError("Wrong typename"));
}

Value Less::evalRator(const Value &rand1, const Value &rand2) { // <
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) < (dynamic_cast<Integer*>(rand2.get())->n));
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_INT) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        int n2 = dynamic_cast<Integer*>(rand2.get())->n;
        return BooleanV((rat1->numerator) < (n2 * rat1->denominator));
    } else if (rand1->v_type == V_INT and rand2->v_type == V_RATIONAL) {
        int n1 = dynamic_cast<Integer*>(rand1.get())->n;
        auto rat2 = dynamic_cast<Rational*>(rand2.get());
        return BooleanV((n1 * rat2->denominator) < (rat2->numerator));
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_RATIONAL) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        auto rat2 = dynamic_cast<Rational*>(rand2.get());
        return BooleanV((rat1->numerator * rat2->denominator) < (rat2->numerator * rat1->denominator));
    }
    throw(RuntimeError("Wrong typename"));
}

Value LessEq::evalRator(const Value &rand1, const Value &rand2) { // <=
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) <= (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Equal::evalRator(const Value &rand1, const Value &rand2) { // =
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) == (dynamic_cast<Integer*>(rand2.get())->n));
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_INT) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        int n2 = dynamic_cast<Integer*>(rand2.get())->n;
        return BooleanV((rat1->numerator) == (n2 * rat1->denominator));
    } else if (rand1->v_type == V_INT and rand2->v_type == V_RATIONAL) {
        int n1 = dynamic_cast<Integer*>(rand1.get())->n;
        auto rat2 = dynamic_cast<Rational*>(rand2.get());
        return BooleanV((n1 * rat2->denominator) == (rat2->numerator));
    } else if (rand1->v_type == V_RATIONAL and rand2->v_type == V_RATIONAL) {
        auto rat1 = dynamic_cast<Rational*>(rand1.get());
        auto rat2 = dynamic_cast<Rational*>(rand2.get());
        return BooleanV((rat1->numerator == rat2->numerator) && (rat1->denominator == rat2->denominator));
    }
    throw(RuntimeError("Wrong typename"));
}

Value GreaterEq::evalRator(const Value &rand1, const Value &rand2) { // >=
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) >= (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Greater::evalRator(const Value &rand1, const Value &rand2) { // >
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) > (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value LessVar::evalRator(const std::vector<Value> &args) { // < with multiple args
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
            return BooleanV(false);
        }
    }
    return BooleanV(true);
}

Value LessEqVar::evalRator(const std::vector<Value> &args) { // <= with multiple args
    if (args.size() < 2) {
        throw(RuntimeError("<= requires at least 2 arguments"));
    }
    
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (args[i]->v_type != V_INT || args[i+1]->v_type != V_INT) {
            throw(RuntimeError("Wrong typename"));
        }
        int n1 = dynamic_cast<Integer*>(args[i].get())->n;
        int n2 = dynamic_cast<Integer*>(args[i+1].get())->n;
        if (!(n1 <= n2)) {
            return BooleanV(false);
        }
    }
    return BooleanV(true);
}

Value EqualVar::evalRator(const std::vector<Value> &args) { // = with multiple args
    if (args.size() < 2) {
        throw(RuntimeError("= requires at least 2 arguments"));
    }
    
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (args[i]->v_type != V_INT || args[i+1]->v_type != V_INT) {
            throw(RuntimeError("Wrong typename"));
        }
        int n1 = dynamic_cast<Integer*>(args[i].get())->n;
        int n2 = dynamic_cast<Integer*>(args[i+1].get())->n;
        if (!(n1 == n2)) {
            return BooleanV(false);
        }
    }
    return BooleanV(true);
}

Value GreaterEqVar::evalRator(const std::vector<Value> &args) { // >= with multiple args
    if (args.size() < 2) {
        throw(RuntimeError(">= requires at least 2 arguments"));
    }
    
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (args[i]->v_type != V_INT || args[i+1]->v_type != V_INT) {
            throw(RuntimeError("Wrong typename"));
        }
        int n1 = dynamic_cast<Integer*>(args[i].get())->n;
        int n2 = dynamic_cast<Integer*>(args[i+1].get())->n;
        if (!(n1 >= n2)) {
            return BooleanV(false);
        }
    }
    return BooleanV(true);
}

Value GreaterVar::evalRator(const std::vector<Value> &args) { // > with multiple args
    if (args.size() < 2) {
        throw(RuntimeError("> requires at least 2 arguments"));
    }
    
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (args[i]->v_type != V_INT || args[i+1]->v_type != V_INT) {
            throw(RuntimeError("Wrong typename"));
        }
        int n1 = dynamic_cast<Integer*>(args[i].get())->n;
        int n2 = dynamic_cast<Integer*>(args[i+1].get())->n;
        if (!(n1 > n2)) {
            return BooleanV(false);
        }
    }
    return BooleanV(true);
}

Value IsEq::evalRator(const Value &rand1, const Value &rand2) { // eq?
    // 检查类型是否为 Integer
    if (rand1->v_type == V_INT && rand2->v_type == V_INT) {
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) == (dynamic_cast<Integer*>(rand2.get())->n));
    }
    // 检查类型是否为 Boolean
    else if (rand1->v_type == V_BOOL && rand2->v_type == V_BOOL) {
        return BooleanV((dynamic_cast<Boolean*>(rand1.get())->b) == (dynamic_cast<Boolean*>(rand2.get())->b));
    }
    // 检查类型是否为 Symbol
    else if (rand1->v_type == V_SYM && rand2->v_type == V_SYM) {
        return BooleanV((dynamic_cast<Symbol*>(rand1.get())->s) == (dynamic_cast<Symbol*>(rand2.get())->s));
    }
    // 检查类型是否为 Null 或 Void
    else if ((rand1->v_type == V_NULL && rand2->v_type == V_NULL) ||
             (rand1->v_type == V_VOID && rand2->v_type == V_VOID)) {
        return BooleanV(true);
    } else {
        return BooleanV(rand1.get() == rand2.get());
    }
}

Value Cons::evalRator(const Value &rand1, const Value &rand2) { // cons
    return PairV(rand1, rand2);
}

Value Quotient::evalRator(const Value &rand1, const Value &rand2) { // quotient
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        int dividend = dynamic_cast<Integer*>(rand1.get())->n;
        int divisor = dynamic_cast<Integer*>(rand2.get())->n;
        if (divisor == 0) {
            throw(RuntimeError("Division by zero"));
        }
        // 向零截断的除法（C++ 的默认行为）
        return IntegerV(dividend / divisor);
    }
    throw(RuntimeError("Wrong typename"));
}

Value Modulo::evalRator(const Value &rand1, const Value &rand2) { // modulo
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        int dividend = dynamic_cast<Integer*>(rand1.get())->n;
        int divisor = dynamic_cast<Integer*>(rand2.get())->n;
        if (divisor == 0) {
            throw(RuntimeError("Division by zero"));
        }
        
        int result = dividend % divisor;
        // 确保结果与除数同号（除非结果为0）
        if (result != 0 && ((result > 0) != (divisor > 0))) {
            result += divisor;
        }
        return IntegerV(result);
    }
    throw(RuntimeError("Wrong typename"));
}

Value Expt::evalRator(const Value &rand1, const Value &rand2) { // expt
    if (rand1->v_type == V_INT and rand2->v_type == V_INT) {
        int base = dynamic_cast<Integer*>(rand1.get())->n;
        int exponent = dynamic_cast<Integer*>(rand2.get())->n;
        
        // 处理特殊情况
        if (exponent < 0) {
            throw(RuntimeError("Negative exponent not supported for integers"));
        }
        if (base == 0 && exponent == 0) {
            throw(RuntimeError("0^0 is undefined"));
        }
        
        // 计算 base^exponent
        long long result = 1;
        long long b = base;
        int exp = exponent;
        
        // 使用快速幂算法
        while (exp > 0) {
            if (exp % 2 == 1) {
                result *= b;
                // 检查溢出
                if (result > INT_MAX || result < INT_MIN) {
                    throw(RuntimeError("Integer overflow in expt"));
                }
            }
            b *= b;
            if (b > INT_MAX || b < INT_MIN) {
                if (exp > 1) {
                    throw(RuntimeError("Integer overflow in expt"));
                }
            }
            exp /= 2;
        }
        
        return IntegerV((int)result);
    }
    throw(RuntimeError("Wrong typename"));
}

Value IsBoolean::evalRator(const Value &rand) { // boolean?
    return BooleanV(rand->v_type == V_BOOL);
}

Value IsFixnum::evalRator(const Value &rand) { // fixnum?
    return BooleanV(rand->v_type == V_INT);
}

Value IsSymbol::evalRator(const Value &rand) { // symbol?
    return BooleanV(rand->v_type == V_SYM);
}

Value IsString::evalRator(const Value &rand) { // string?
    return BooleanV(rand->v_type == V_STRING);
}

Value IsNull::evalRator(const Value &rand) { // null?
    return BooleanV(rand->v_type == V_NULL);
}

Value IsPair::evalRator(const Value &rand) { // pair?
    return BooleanV(rand->v_type == V_PAIR);
}

Value IsProcedure::evalRator(const Value &rand) { // procedure?
    return BooleanV(rand->v_type == V_PROC);
}

Value IsList::evalRator(const Value &rand) { // list?
    // 在 Scheme 中，list? 检查值是否为正常列表（包括空列表）
    // 正常列表是以 null 结尾的 pair 链，或者就是 null
    if (rand->v_type == V_NULL) {
        return BooleanV(true); // 空列表是列表
    }
    
    if (rand->v_type != V_PAIR) {
        return BooleanV(false); // 不是 pair 就不是列表
    }
    
    // 使用快慢指针检测环形列表并找到列表末尾
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
    
    // 检查最后是否以 null 结尾
    return BooleanV(fast->v_type == V_NULL);
}

Value Not::evalRator(const Value &rand) { // not
    if (rand->v_type == V_BOOL and (dynamic_cast<Boolean*>(rand.get())->b == false))
        return BooleanV(true);
    else
        return BooleanV(false);
}

Value Car::evalRator(const Value &rand) { // car
    if (rand->v_type == V_PAIR)
        return dynamic_cast<Pair*>(rand.get())->car;
    else
        throw(RuntimeError("Wrong typename"));
}

Value Cdr::evalRator(const Value &rand) { // cdr
    if (rand->v_type == V_PAIR)
        return dynamic_cast<Pair*>(rand.get())->cdr;
    else
        throw(RuntimeError("Wrong typename"));
}

// 多参数算术运算符实现
Value MultVar::evalRator(const std::vector<Value> &args) { // * with multiple args
    if (args.empty()) {
        return IntegerV(1); // (* ) → 1 (multiplicative identity)
    }
    
    int result = 1;
    for (const auto& arg : args) {
        if (arg->v_type == V_INT) {
            result *= dynamic_cast<Integer*>(arg.get())->n;
        } else {
            throw(RuntimeError("Wrong typename"));
        }
    }
    return IntegerV(result);
}

Value PlusVar::evalRator(const std::vector<Value> &args) { // + with multiple args
    if (args.empty()) {
        return IntegerV(0); // (+ ) → 0 (additive identity)
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

Value MinusVar::evalRator(const std::vector<Value> &args) { // - with multiple args
    if (args.empty()) {
        throw(RuntimeError("Wrong number of arguments for -"));
    }
    if (args.size() == 1) {
        // (- x) → -x (negation)
        if (args[0]->v_type == V_INT) {
            return IntegerV(-(dynamic_cast<Integer*>(args[0].get())->n));
        } else {
            throw(RuntimeError("Wrong typename"));
        }
    }
    
    // (- x y z ...) → x - y - z - ...
    if (args[0]->v_type != V_INT) {
        throw(RuntimeError("Wrong typename"));
    }
    int result = dynamic_cast<Integer*>(args[0].get())->n;
    for (size_t i = 1; i < args.size(); i++) {
        if (args[i]->v_type == V_INT) {
            result -= dynamic_cast<Integer*>(args[i].get())->n;
        } else {
            throw(RuntimeError("Wrong typename"));
        }
    }
    return IntegerV(result);
}

// GCD 辅助函数（用于有理数化简）
static int gcd_helper(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 有理数乘法
static Value multiply_rationals(int num1, int den1, int num2, int den2) {
    int new_num = num1 * num2;
    int new_den = den1 * den2;
    int g = gcd_helper(new_num, new_den);
    new_num /= g;
    new_den /= g;
    if (new_den < 0) {
        new_num = -new_num;
        new_den = -new_den;
    }
    return RationalV(new_num, new_den);
}

Value DivVar::evalRator(const std::vector<Value> &args) { // / with multiple args
    if (args.empty()) {
        throw(RuntimeError("Wrong number of arguments for /"));
    }
    if (args.size() == 1) {
        // (/ x) → 1/x (reciprocal)
        if (args[0]->v_type == V_INT) {
            int n = dynamic_cast<Integer*>(args[0].get())->n;
            if (n == 0) throw(RuntimeError("Division by zero"));
            return RationalV(1, n);
        } else if (args[0]->v_type == V_RATIONAL) {
            auto rat = dynamic_cast<Rational*>(args[0].get());
            if (rat->numerator == 0) throw(RuntimeError("Division by zero"));
            return RationalV(rat->denominator, rat->numerator);
        } else {
            throw(RuntimeError("Wrong typename"));
        }
    }
    
    // (/ x y z ...) → x / y / z / ...
    int num, den;
    if (args[0]->v_type == V_INT) {
        num = dynamic_cast<Integer*>(args[0].get())->n;
        den = 1;
    } else if (args[0]->v_type == V_RATIONAL) {
        auto rat = dynamic_cast<Rational*>(args[0].get());
        num = rat->numerator;
        den = rat->denominator;
    } else {
        throw(RuntimeError("Wrong typename"));
    }
    
    for (size_t i = 1; i < args.size(); i++) {
        if (args[i]->v_type == V_INT) {
            int divisor = dynamic_cast<Integer*>(args[i].get())->n;
            if (divisor == 0) throw(RuntimeError("Division by zero"));
            num *= 1;
            den *= divisor;
        } else if (args[i]->v_type == V_RATIONAL) {
            auto rat = dynamic_cast<Rational*>(args[i].get());
            if (rat->numerator == 0) throw(RuntimeError("Division by zero"));
            num *= rat->denominator;
            den *= rat->numerator;
        } else {
            throw(RuntimeError("Wrong typename"));
        }
    }
    
    int g = gcd_helper(num, den);
    num /= g;
    den /= g;
    if (den < 0) {
        num = -num;
        den = -den;
    }
    return RationalV(num, den);
}

Value SetCar::evalRator(const Value &rand1, const Value &rand2) { // set-car!
    if (rand1->v_type != V_PAIR) {
        throw RuntimeError("set-car!: argument must be a pair");
    }
    
    Pair* pair_ptr = dynamic_cast<Pair*>(rand1.get());
    pair_ptr->car = rand2;
    
    return VoidV();
}

Value SetCdr::evalRator(const Value &rand1, const Value &rand2) { // set-cdr!
    if (rand1->v_type != V_PAIR) {
        throw RuntimeError("set-cdr!: argument must be a pair");
    }
    
    Pair* pair_ptr = dynamic_cast<Pair*>(rand1.get());
    pair_ptr->cdr = rand2;
    
    return VoidV();
}

Value ListFunc::evalRator(const std::vector<Value> &args) { // list function
    // list 函数创建一个包含所有参数的列表
    // (list) → () (空列表)
    // (list 1) → (1)
    // (list 1 2 3) → (1 2 3)
    
    if (args.empty()) {
        return NullV(); // 空列表
    }
    
    // 从右向左构建列表 (cons 第一个参数 到 (递归构建剩余参数的列表))
    Value result = NullV();
    for (int i = args.size() - 1; i >= 0; i--) {
        result = PairV(args[i], result);
    }
    
    return result;
}

Value Display::evalRator(const Value &rand) { // display function
    // display 输出值但不换行，字符串不显示引号
    if (rand->v_type == V_STRING) {
        // 对于字符串，输出内容但不包括引号
        String* str_ptr = dynamic_cast<String*>(rand.get());
        std::cout << str_ptr->s;
    } else {
        // 对于其他类型，使用标准显示方法
        rand->show(std::cout);
    }
    
    return VoidV();
}
