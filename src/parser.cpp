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

Expr List::parsePrimitive(const string &op, Assoc &env) {
    switch (primitives[op]) {
        case E_MUL:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for *");
            return Expr(new Mult(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_MINUS:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for -");
            return Expr(new Minus(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_PLUS:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for +");
            return Expr(new Plus(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_LT:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for <");
            return Expr(new Less(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_LE:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for <=");
            return Expr(new LessEq(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_EQ:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for =");
            return Expr(new Equal(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_GE:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for >=");
            return Expr(new GreaterEq(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_GT:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for >");
            return Expr(new Greater(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_VOID:
            if (stxs.size() != 1) throw RuntimeError("wrong parameter number for void");
            return Expr(new MakeVoid());
        case E_EQQ:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for eq?");
            return Expr(new IsEq(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_BOOLQ:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for boolean?");
            return Expr(new IsBoolean(stxs[1]->parse(env)));
        case E_INTQ:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for fixnum?");
            return Expr(new IsFixnum(stxs[1]->parse(env)));
        case E_NULLQ:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for null?");
            return Expr(new IsNull(stxs[1]->parse(env)));
        case E_PAIRQ:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for pair?");
            return Expr(new IsPair(stxs[1]->parse(env)));
        case E_PROCQ:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for procedure?");
            return Expr(new IsProcedure(stxs[1]->parse(env)));
        case E_SYMBOLQ:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for symbol?");
            return Expr(new IsSymbol(stxs[1]->parse(env)));
        case E_CONS:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for cons");
            return Expr(new Cons(stxs[1]->parse(env), stxs[2]->parse(env)));
        case E_NOT:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for not");
            return Expr(new Not(stxs[1]->parse(env)));
        case E_CAR:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for car");
            return Expr(new Car(stxs[1]->parse(env)));
        case E_CDR:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for cdr");
            return Expr(new Cdr(stxs[1]->parse(env)));
        case E_EXIT:
            if (stxs.size() != 1) throw RuntimeError("wrong parameter number for exit");
            return Expr(new Exit());
        default:
            throw RuntimeError("Unknown primitive: " + op);
    }
}

Expr List::parseLet(Assoc &env) {
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

Expr List::parseBegin(Assoc &env) {
    vector<Expr> passed_exprs;
    for (size_t i = 1; i < stxs.size(); i++) {
        passed_exprs.push_back(stxs[i]->parse(env));
    }
    return Expr(new Begin(passed_exprs));
}

Expr List::parseLambda(Assoc &env) {
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

Expr List::parseLetrec(Assoc &env) {
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

Expr List::parseReservedWord(const string &op, Assoc &env) {
    switch (reserved_words[op]) {
        case E_LET:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for let");
        return parseLet(env);
        case E_IF:
            if (stxs.size() != 4) throw RuntimeError("wrong parameter number for if");
        return Expr(new If(stxs[1]->parse(env), stxs[2]->parse(env), stxs[3]->parse(env)));
        case E_BEGIN:
            return parseBegin(env);
        case E_QUOTE:
            if (stxs.size() != 2) throw RuntimeError("wrong parameter number for quote");
        return Expr(new Quote(stxs[1]));
        case E_LAMBDA:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for lambda");
        return parseLambda(env);
        case E_LETREC:
            if (stxs.size() != 3) throw RuntimeError("wrong parameter number for letrec");
        return parseLetrec(env);
        default:
            throw RuntimeError("Unknown reserved word: " + op);
    }
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
    }

    string op = id->s;

    // 检查是否为库函数
    if (primitives.count(op) != 0) {
        return parsePrimitive(op, env);
    }

    // 检查是否为保留字
    if (reserved_words.count(op) != 0) {
        return parseReservedWord(op, env);
    }

    // 默认情况：构造 Apply 表达式
    Expr opexpr = stxs[0]->parse(env);
    vector<Expr> to_expr;
    for (size_t i = 1; i < stxs.size(); i++) {
        to_expr.push_back(stxs[i]->parse(env));
    }
    return Expr(new Apply(opexpr, to_expr));
}

#endif