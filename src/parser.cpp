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
    if (stxs.empty()) {throw RuntimeError("Empty list is not a valid expression");}

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
         return new Apply(stxs[0].get()->parse(env), parameters);
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
