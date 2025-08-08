#ifndef PARSER
#define PARSER
#include "RE.hpp"
#include "Def.hpp"
#include "syntax.hpp"
#include "value.hpp"
#include "expr.hpp"
#include <map>
#include <string>
#include <iostream>
#define mp make_pair
using std::string;
using std::vector;
using std::pair;

extern std::map<std::string, ExprType> primitives;
extern std::map<std::string, ExprType> reserved_words;

Expr Syntax::parse(Assoc &env) {
    throw RuntimeError("Unimplemented parse method");
}

Expr Number::parse(Assoc &env) {
    return Expr(new Fixnum(n));
}

Expr Identifier::parse(Assoc &env) {
    return Expr(new Var(s));
}

Expr TrueSyntax::parse(Assoc &env) {
    return Expr(new True());
}

Expr FalseSyntax::parse(Assoc &env) {
    return Expr(new False());
}

Expr List::parse(Assoc &env) {
    if (stxs.empty()) {
        // 空列表 () 应该解析为一个引用的空列表，求值为 null
        return Expr(new Quote(Syntax(new List())));
    }

    // 检查第一个元素是否为 Identifier
    Identifier *id = dynamic_cast<Identifier*>(stxs[0].get());
    if (id == nullptr) {
        // 如果不是 Identifier，则将其解析为表达式并构造 Apply 表达式
        vector<Expr> parameters;
        for (size_t i = 1; i < stxs.size(); i++) {
            parameters.push_back(stxs[i]->parse(env));
        }
        return Expr(new Apply(stxs[0]->parse(env), parameters));
    }else{
    string op = id->s;
    if (find(op, env).get() != nullptr) {
         vector<Expr> parameters;
        for (size_t i = 1; i < stxs.size(); i++) {
            parameters.push_back(stxs[i].get()->parse(env));
        }
        return Expr(new Apply(stxs[0].get()->parse(env), parameters));
    }
    // 检查是否为库函数
    if (primitives.count(op) != 0) {
        vector<Expr> parameters;
        for (int i = 1; i < stxs.size(); i++) {
            parameters.push_back(stxs[i].get()->parse(env));
        }
        
        // 特殊处理多参数算术运算符
        ExprType op_type = primitives[op];
        if (op_type == E_PLUS) {
            if (parameters.size() == 0) {
                return Expr(new PlusVar(parameters)); // (+ ) → 0
            } else if (parameters.size() == 1) {
                return parameters[0]; // (+ x) → x
            } else if (parameters.size() == 2) {
                return Expr(new Plus(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new PlusVar(parameters)); // 多参数
            }
        } else if (op_type == E_MUL) {
            if (parameters.size() == 0) {
                return Expr(new MultVar(parameters)); // (* ) → 1
            } else if (parameters.size() == 1) {
                return parameters[0]; // (* x) → x
            } else if (parameters.size() == 2) {
                return Expr(new Mult(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new MultVar(parameters)); // 多参数
            }
        } else if (op_type == E_MINUS) {
            if (parameters.size() == 0) {
                throw RuntimeError("Wrong number of arguments for -");
            } else if (parameters.size() == 1) {
                return Expr(new MinusVar(parameters)); // (- x) → -x
            } else if (parameters.size() == 2) {
                return Expr(new Minus(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new MinusVar(parameters)); // 多参数
            }
        } else if (op_type == E_DIV) {
            if (parameters.size() == 0) {
                throw RuntimeError("Wrong number of arguments for /");
            } else if (parameters.size() == 1) {
                return Expr(new DivVar(parameters)); // (/ x) → 1/x
            } else if (parameters.size() == 2) {
                return Expr(new Div(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new DivVar(parameters)); // 多参数
            }
        } else if (op_type == E_LIST) {
            // list 函数：接受任意数量的参数
            return Expr(new ListFunc(parameters));
        } else if (op_type == E_LT) {
            // < 比较操作符
            if (parameters.size() < 2) {
                throw RuntimeError("Wrong number of arguments for <");
            } else if (parameters.size() == 2) {
                return Expr(new Less(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new LessVar(parameters)); // 多参数
            }
        } else if (op_type == E_LE) {
            // <= 比较操作符
            if (parameters.size() < 2) {
                throw RuntimeError("Wrong number of arguments for <=");
            } else if (parameters.size() == 2) {
                return Expr(new LessEq(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new LessEqVar(parameters)); // 多参数
            }
        } else if (op_type == E_EQ) {
            // = 比较操作符
            if (parameters.size() < 2) {
                throw RuntimeError("Wrong number of arguments for =");
            } else if (parameters.size() == 2) {
                return Expr(new Equal(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new EqualVar(parameters)); // 多参数
            }
        } else if (op_type == E_GE) {
            // >= 比较操作符
            if (parameters.size() < 2) {
                throw RuntimeError("Wrong number of arguments for >=");
            } else if (parameters.size() == 2) {
                return Expr(new GreaterEq(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new GreaterEqVar(parameters)); // 多参数
            }
        } else if (op_type == E_GT) {
            // > 比较操作符
            if (parameters.size() < 2) {
                throw RuntimeError("Wrong number of arguments for >");
            } else if (parameters.size() == 2) {
                return Expr(new Greater(parameters[0], parameters[1])); // 保持二元兼容
            } else {
                return Expr(new GreaterVar(parameters)); // 多参数
            }
        } else {
            // 其他原语保持原来的处理方式
            return new Apply(stxs[0].get()->parse(env), parameters);
        }
    }
    // 检查是否为保留字
    if (reserved_words.count(op) != 0) {
    	switch (reserved_words[op]) {
        	case E_LET:{
            		if (stxs.size() != 3) throw RuntimeError("wrong parameter number for let");
        		vector<pair<string, Expr>> binded_vector;
            		List *binder_list_ptr = dynamic_cast<List*>(stxs[1].get());
            		if (binder_list_ptr == nullptr) {throw RuntimeError("Invalid let binding list");}

            		Assoc local_env = env; // 创建新的环境
                	for (int i = 0; i < binder_list_ptr->stxs.size(); i++) {
                     		auto pair_it = dynamic_cast<List*>(binder_list_ptr->stxs[i].get());
                     		if ((pair_it == nullptr)||(pair_it->stxs.size() != 2)) {throw RuntimeError("Invalid let binding list");}
                     		auto Identifiers = dynamic_cast<Identifier*>(pair_it->stxs.front().get());
                     		if (Identifiers == nullptr) {throw RuntimeError("Invalid input of identifier");}
                      		Expr temp_expr = pair_it->stxs.back().get()->parse(env);
                      		local_env = extend(Identifiers->s, NullV(), local_env);
                      		pair<string, Expr> tmp_pair = std::make_pair(Identifiers->s, temp_expr);
                      		binded_vector.push_back(tmp_pair);
                	}
             		return Expr(new Let(binded_vector, stxs[2]->parse(local_env))); // 使用 local_env
        	}
        	case E_IF:{if (stxs.size() != 4) throw RuntimeError("wrong parameter number for if");return Expr(new If(stxs[1]->parse(env), stxs[2]->parse(env), stxs[3]->parse(env)));}
        	case E_BEGIN:{
             		vector<Expr> passed_exprs;
    		      	for (size_t i = 1; i < stxs.size(); i++) {
        		        passed_exprs.push_back(stxs[i]->parse(env));
    			}
             		return Expr(new Begin(passed_exprs));
        	}
        	case E_AND:{
             		vector<Expr> passed_exprs;
    		      	for (size_t i = 1; i < stxs.size(); i++) {
        		        passed_exprs.push_back(stxs[i]->parse(env));
    			}
             		return Expr(new And(passed_exprs));
        	}
        	case E_OR:{
             		vector<Expr> passed_exprs;
    		      	for (size_t i = 1; i < stxs.size(); i++) {
        		        passed_exprs.push_back(stxs[i]->parse(env));
    			}
             		return Expr(new Or(passed_exprs));
        	}
        	case E_QUOTE:{if (stxs.size() != 2) throw RuntimeError("wrong parameter number for quote");return Expr(new Quote(stxs[1]));}
        	case E_LAMBDA:{
            		if (stxs.size() != 3) throw RuntimeError("wrong parameter number for lambda");
            		Assoc New_env = env;
                	std::vector<std::string> vars;
                	List* paras_ptr = dynamic_cast<List*>(stxs[1].get());
                	if (paras_ptr == nullptr) {throw RuntimeError("Invalid lambda parameter list");}
            		for (int i = 0; i < paras_ptr->stxs.size(); i++) {
                     		if (auto tmp_var = dynamic_cast<Var*>(paras_ptr->stxs[i].get()->parse(env).get())) {
                         		vars.push_back(tmp_var->x);
                         		New_env = extend(tmp_var->x, NullV(), New_env);
                     		} else {
                         		throw RuntimeError("Invalid input of variable");
                     		}	
                	}
                	return Expr(new Lambda(vars, stxs[2].get()->parse(New_env)));break;
        	}
        	case E_LETREC:{
    			if (stxs.size() != 3) throw RuntimeError("wrong parameter number for letrec");
    			vector<pair<string, Expr>> binded_vector;
    			List *binder_list_ptr = dynamic_cast<List*>(stxs[1].get());
    			if (binder_list_ptr == nullptr) {throw RuntimeError("Invalid letrec binding list");}

    			// 创建新的环境用于解析
    			Assoc temp_env = env;

    			// 第一次遍历：收集所有变量名并在临时环境中绑定为 null
    			for (auto &stx_tobind_raw : binder_list_ptr->stxs) {
        			List *stx_tobind = dynamic_cast<List*>(stx_tobind_raw.get());
        			if (stx_tobind == nullptr || stx_tobind->stxs.size() != 2) {throw RuntimeError("Invalid letrec binding");}

        			Identifier *temp_id = dynamic_cast<Identifier*>(stx_tobind->stxs[0].get());
        			if (temp_id == nullptr) {throw RuntimeError("Invalid letrec binding variable");}
				
        			// 在临时环境中绑定变量，初始值为 null
        			temp_env = extend(temp_id->s, NullV(), temp_env);
    			}

    			// 第二次遍历：使用包含所有变量的环境解析表达式
    			for (auto &stx_tobind_raw : binder_list_ptr->stxs) {
        			List *stx_tobind = dynamic_cast<List*>(stx_tobind_raw.get());
        			Identifier *temp_id = dynamic_cast<Identifier*>(stx_tobind->stxs[0].get());

        			// 在包含所有变量的环境中解析表达式
        			Expr temp_store = stx_tobind->stxs[1]->parse(temp_env);
        			binded_vector.push_back(mp(temp_id->s, temp_store));
    			}

    			// 使用同样的环境解析 body
    			return Expr(new Letrec(binded_vector, stxs[2]->parse(temp_env)));
		}
		case E_DEFINE:{
			if (stxs.size() != 3) throw RuntimeError("wrong parameter number for define");
			
			// 检查第二个元素是否为List（函数定义语法糖）
			List *func_def_list = dynamic_cast<List*>(stxs[1].get());
			if (func_def_list != nullptr) {
				// 语法糖: (define (func-name param1 param2 ...) body)
				if (func_def_list->stxs.empty()) {
					throw RuntimeError("Invalid function definition: empty parameter list");
				}
				
				// 第一个元素应该是函数名
				Identifier *func_name = dynamic_cast<Identifier*>(func_def_list->stxs[0].get());
				if (func_name == nullptr) {
					throw RuntimeError("Invalid function name in define");
				}
				
				// 提取参数列表
				vector<string> param_names;
				for (size_t i = 1; i < func_def_list->stxs.size(); i++) {
					Identifier *param = dynamic_cast<Identifier*>(func_def_list->stxs[i].get());
					if (param == nullptr) {
						throw RuntimeError("Invalid parameter in function definition");
					}
					param_names.push_back(param->s);
				}
				
				// 创建lambda表达式
				Expr lambda_expr = Expr(new Lambda(param_names, stxs[2]->parse(env)));
				return Expr(new Define(func_name->s, lambda_expr));
			} else {
				// 原有语法: (define var-name expression)
				Identifier *var_id = dynamic_cast<Identifier*>(stxs[1].get());
				if (var_id == nullptr) {throw RuntimeError("Invalid define variable");}
				return Expr(new Define(var_id->s, stxs[2]->parse(env)));
			}
		}
        	default:
            	throw RuntimeError("Unknown reserved word: " + op);
    	}
    }

    // 默认情况：构造 Apply 表达式
    Expr opexpr = stxs[0]->parse(env);
    vector<Expr> to_expr;
    for (size_t i = 1; i < stxs.size(); i++) {
        to_expr.push_back(stxs[i]->parse(env));
    }
    return Expr(new Apply(opexpr, to_expr));
    }
}

#endif
