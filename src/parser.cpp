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
using std :: string;
using std :: vector;
using std :: pair;

extern std :: map<std :: string, ExprType> primitives;
extern std :: map<std :: string, ExprType> reserved_words;

Expr Syntax::parse(Assoc &env) {
    throw RuntimeError("Unimplemented parse method");
}

Expr Number :: parse(Assoc &env) {
    return Expr(new Fixnum(n));
}

Expr Identifier :: parse(Assoc &env) {
    return Expr(new Var(s));
}

Expr TrueSyntax :: parse(Assoc &env) {
    return Expr(new True());
}

Expr FalseSyntax :: parse(Assoc &env) {
    return Expr(new False());
}

Expr List::parse(Assoc &env) {
    if (stxs.empty()) {
        throw RuntimeError("Empty list is not a valid expression");
    }

    // 检查第一个元素是否为 Identifier
    Identifier *id = dynamic_cast<Identifier*>(stxs[0].get());
    if (id == nullptr) {
        // 如果不是 Identifier，则将其解析为表达式并构造 Apply 表达式
        Expr opexpr = stxs[0]->parse(env);
        vector<Expr> to_expr;
        for (size_t i = 1; i < stxs.size(); i++) {
            to_expr.push_back(stxs[i]->parse(env));
        }
        return Expr(new Apply(opexpr, to_expr));
    }else{
    string op = id->s;
    if (find(op, env).get() != nullptr) {
         vector<Expr> parameters;
         for (int i = 1; i < stxs.size(); i++) {
             parameters.push_back(stxs[i].get()->parse(env));
         }
         return new Apply(stxs[0].get()->parse(env), parameters);
    }
    // 检查是否为库函数
    if (primitives.count(op) != 0) {
        vector<Expr> parameters;
         for (int i = 1; i < stxs.size(); i++) {
             parameters.push_back(stxs[i].get()->parse(env));
         }
         return new Apply(stxs[0].get()->parse(env), parameters);
    }
    // 检查是否为保留字
    if (reserved_words.count(op) != 0) {
    	switch (reserved_words[op]) {
        	case E_LET:{
            	if (stxs.size() != 3) throw RuntimeError("wrong parameter number for let");
        		vector<pair<string, Expr>> binded_vector;
            	List *binder_list_ptr = dynamic_cast<List*>(stxs[1].get());
            	if (binder_list_ptr == nullptr) {
                  	throw RuntimeError("Invalid let binding list");
            	}

            	Assoc local_env = env; // 创建新的环境
            	for (auto &stx_tobind_raw : binder_list_ptr->stxs) {
                	List *stx_tobind = dynamic_cast<List*>(stx_tobind_raw.get());
                	if (stx_tobind == nullptr || stx_tobind->stxs.size() != 2) {
                    	throw RuntimeError("Invalid let binding");
                	}

                	Identifier *temp_id = dynamic_cast<Identifier*>(stx_tobind->stxs[0].get());
                	if (temp_id == nullptr) {
                    	throw RuntimeError("Invalid let binding variable");
                	}

                 	string var_name = temp_id->s;
                 	Expr temp_store = stx_tobind->stxs[1]->parse(local_env); // 使用 local_env
                 	binded_vector.push_back(mp(var_name, temp_store));
                 	local_env = extend(var_name, temp_store->eval(local_env), local_env); // 更新 local_env
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
        	case E_QUOTE:{if (stxs.size() != 2) throw RuntimeError("wrong parameter number for quote");return Expr(new Quote(stxs[1]));}
        	case E_LAMBDA:{
            	if (stxs.size() != 3) throw RuntimeError("wrong parameter number for lambda");
            	vector<string> paras;
            	List *paras_ptr = dynamic_cast<List*>(stxs[1].get());
            	if (paras_ptr == nullptr) {
                	throw RuntimeError("Invalid lambda parameter list");
            	}
            	for (auto &one_para : paras_ptr->stxs) {
                 	Identifier *id = dynamic_cast<Identifier*>(one_para.get());
                 	if (id == nullptr) {
                     	throw RuntimeError("Invalid lambda parameter");
                 	}
                 	paras.push_back(id->s);
            	}
            	Expr pass_expr = stxs[2]->parse(env);
            	return Expr(new Lambda(paras, pass_expr));
        	}
        	case E_LETREC:{
            	if (stxs.size() != 3) throw RuntimeError("wrong parameter number for letrec");
            	vector<pair<string, Expr>> binded_vector;
            	List *binder_list_ptr = dynamic_cast<List*>(stxs[1].get());
            	if (binder_list_ptr == nullptr) {
                 	throw RuntimeError("Invalid letrec binding list");
            	}

            	// 创建一个临时环境，用于存储未完全解析的绑定
            	Assoc temp_env = env;

            	// 首先将所有绑定的变量添加到临时环境中，初始值为 NullV()
            	for (auto &stx_tobind_raw : binder_list_ptr->stxs) {
                 	List *stx_tobind = dynamic_cast<List*>(stx_tobind_raw.get());
                 	if (stx_tobind == nullptr || stx_tobind->stxs.size() != 2) {
                      	throw RuntimeError("Invalid letrec binding");
                 	}

                 	Identifier *temp_id = dynamic_cast<Identifier*>(stx_tobind->stxs[0].get());
                 	if (temp_id == nullptr) {
                      	throw RuntimeError("Invalid letrec binding variable");
                 	}

                 	string var_name = temp_id->s;
                 	temp_env = extend(var_name, NullV(), temp_env); // 将变量添加到临时环境中，初始值为 NullV()
            	}

            	// 解析每个绑定的表达式
            	for (auto &stx_tobind_raw : binder_list_ptr->stxs) {
                 	List *stx_tobind = dynamic_cast<List*>(stx_tobind_raw.get());
                 	if (stx_tobind == nullptr || stx_tobind->stxs.size() != 2) {
                      	throw RuntimeError("Invalid letrec binding");
                 	}

                 	Identifier *temp_id = dynamic_cast<Identifier*>(stx_tobind->stxs[0].get());
                 	if (temp_id == nullptr) {
                      	throw RuntimeError("Invalid letrec binding variable");
                 	}

                 	string var_name = temp_id->s;
                 	Expr temp_store = stx_tobind->stxs[1]->parse(temp_env); // 使用临时环境解析表达式
                 	binded_vector.push_back(mp(var_name, temp_store));
            	}

            	// 返回 Letrec 表达式
            	return Expr(new Letrec(binded_vector, stxs[2]->parse(temp_env)));
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