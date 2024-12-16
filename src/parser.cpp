#ifndef PARSER
#define PARSER

// parser of myscheme

#include "RE.hpp"
#include "Def.hpp"
#include "syntax.hpp"
#include "expr.hpp"
#include "value.hpp"
#include <map>
#include <cstring>
#include <iostream>
#define mp make_pair
using std ::pair;
using std ::string;
using std ::vector;

extern std ::map<std ::string, ExprType> primitives;
extern std ::map<std ::string, ExprType> reserved_words;

Expr Syntax ::parse(Assoc &env) { return ptr->parse(env); }

Expr Number ::parse(Assoc &env)
{
    ExprBase *pt = new Fixnum(n);
    Expr temp(pt);
    return temp;
}

Expr TrueSyntax ::parse(Assoc &env)
{
    ExprBase *pt = new True();
    Expr temp(pt);
    return temp;
}

Expr FalseSyntax ::parse(Assoc &env)
{
    ExprBase *pt = new False();
    Expr temp(pt);
    return temp;
}

Expr Identifier ::parse(Assoc &env)
{
    if (find(s, env).get() != nullptr)
    {
        ExprBase *pt = new Var(s);
        Expr temp(pt);
        return temp;
    }
    if (primitives.find(s) != primitives.end())
    {
        ExprBase *pt = new ExprBase(primitives[s]);
        Expr temp(pt);
        return temp;
    }
    if (reserved_words.find(s) != reserved_words.end())
    {
        ExprBase *pt = new ExprBase(reserved_words[s]);
        Expr temp(pt);
        return temp;
    }
    throw RuntimeError("Bad Identifier Syntax");
}

Expr List ::parse(Assoc &env)
{
    if (stxs.empty())
    {
        SyntaxBase *temp = new List();
        ExprBase *t = new Quote(Syntax(temp));
        return Expr(t);
    }
    Expr t = stxs.begin()->parse(env);
    if (t->e_type == E_VAR || typeid(*stxs[0].get()) == typeid(List))
    {
        std::vector<Expr> t1;
        auto p = stxs.begin();
        ++p;
        for (; p != stxs.end(); ++p)
        {
            t1.push_back(p->parse(env));
        }
        ExprBase *temp = new Apply(t, t1);
        return Expr(temp);
    }
    else if (t->e_type == E_VOID)
    {
        if (stxs.size() != 1)
            throw RuntimeError("Bad parameters number");
        ExprBase *pt = new MakeVoid();
        Expr temp(pt);
        return temp;
    }
    else if (t->e_type == E_LET)
    {
        // 备份环境变量
        Assoc env2 = env;
        // 存储使用过的变量名
        std::map<std::string, bool> m;

        // 检查参数是否符合要求
        if (stxs.size() != 3)
            throw RuntimeError("Inadequate parameter numbers");
        auto p = stxs.begin();
        ++p;
        if (typeid(*p->get()) != typeid(List))
            throw RuntimeError("Inadequate parameter format");

        // 获取赋值列表
        SyntaxBase *BindListt = p->get();
        List *BindList = dynamic_cast<List *>(BindListt);
        std ::vector<Syntax> Bindvct = BindList->stxs;

        // 遍历复制列表，继续检查格式并修改环境变量
        auto q = Bindvct.begin();
        std::vector<std::pair<std::string, Expr>> bind;
        for (; q != Bindvct.end(); ++q)
        {
            if (typeid(*q->get()) != typeid(List))
                throw RuntimeError("Inadequate parameter format");
            List *BindArray = dynamic_cast<List *>(q->get());

            // 检查内部每一个赋值列表类型以及元素个数
            if (BindArray->stxs.size() != 2)
                throw RuntimeError("Inadequate parameter format");
            if (typeid(*BindArray->stxs.begin()->get()) != typeid(Identifier))
                throw RuntimeError("Bad variable name");

            // 将新增加的变量加入环境变量并且放入 let 的 expr 中
            auto pt = BindArray->stxs.begin();
            Identifier *ipt = dynamic_cast<Identifier *>(pt->get());
            ++pt;
            Expr val = pt->parse(env2);

            // 加入环境变量中
            Value v = TerminateV();
            env = extend(ipt->s, v, env);

            bind.push_back({ipt->s, val});
            if (m.find(ipt->s) != m.end())
                throw RuntimeError("non-unique bindings");
            m[ipt->s] = 1;
        }
        ++p;
        Expr body = p->parse(env);

        ExprBase *temp = new Let(bind, body);
        env = env2;
        return Expr(temp);
    }
    else if (t->e_type == E_QUOTE)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter numbers");
        auto p = stxs.begin();
        ++p;
        ExprBase *temp = new Quote(*p);
        return Expr(temp);
    }
    else if (t->e_type == E_IF)
    {
        if (stxs.size() != 4)
            throw RuntimeError("Bad parameters number");
        Expr cond = stxs[1]->parse(env);
        Expr conseq = stxs[2]->parse(env);
        Expr alter = stxs[3]->parse(env);
        ExprBase *temp = new If(cond, conseq, alter);
        return Expr(temp);
    }
    else if (t->e_type == E_BEGIN)
    {
        std::vector<Expr> v;
        for (int i = 1; i < stxs.size(); ++i)
        {
            v.push_back(stxs[i]->parse(env));
        }
        ExprBase *temp = new Begin(v);
        return Expr(temp);
    }
    else if (t->e_type == E_LAMBDA)
    {
        // 备份环境变量
        Assoc env2 = env;
        std::map<std::string, bool> m;
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");

        // 检查参数类型
        if (typeid(*stxs[1].get()) != typeid(List))
            throw RuntimeError("Bad parameter format");
        SyntaxBase *lamdapt = stxs[1].get();
        List *LamdaList = dynamic_cast<List *>(lamdapt);

        std::vector<std::string> x;
        for (int i = 0; i < LamdaList->stxs.size(); ++i)
        {
            // 检查参数类型
            if (typeid(*LamdaList->stxs[i].get()) != typeid(Identifier))
                throw RuntimeError("Bad parameter format");
            SyntaxBase *varpt = LamdaList->stxs[i].get();
            Identifier *varname = dynamic_cast<Identifier *>(varpt);
            x.push_back(varname->s);
            if (m.find(varname->s) != m.end())
                throw RuntimeError("non-unique bindings");
            env = extend(varname->s, TerminateV(), env);
            m[varname->s] = 1;
        }
        Expr body = stxs[2]->parse(env);
        ExprBase *temp = new Lambda(x, body);
        env = env2;
        return Expr(temp);
    }
    else if (t->e_type == E_LETREC)
    {
        // 备份环境变量
        Assoc env2 = env;
        std::map<std::string, bool> m;

        // 检查参数是否符合要求
        if (stxs.size() != 3)
            throw RuntimeError("Inadequate parameter numbers");
        auto p = stxs.begin();
        ++p;
        if (typeid(*p->get()) != typeid(List))
            throw RuntimeError("Inadequate parameter format");

        // 获取赋值列表
        SyntaxBase *BindListt = p->get();
        List *BindList = dynamic_cast<List *>(BindListt);
        std ::vector<Syntax> Bindvct = BindList->stxs;

        // 遍历复制列表，继续检查格式并修改环境变量
        auto q = Bindvct.begin();
        std::vector<std::pair<std::string, Expr>> bind;
        for (; q != Bindvct.end(); ++q)
        {
            if (typeid(*q->get()) != typeid(List))
                throw RuntimeError("Inadequate parameter format");
            List *BindArray = dynamic_cast<List *>(q->get());

            // 检查内部每一个赋值列表类型以及元素个数
            if (BindArray->stxs.size() != 2)
                throw RuntimeError("Inadequate parameter format");
            if (typeid(*BindArray->stxs.begin()->get()) != typeid(Identifier))
                throw RuntimeError("Bad variable name");

            // 将新增加的变量加入环境变量
            auto pt = BindArray->stxs.begin();
            Identifier *ipt = dynamic_cast<Identifier *>(pt->get());
            if (m.find(ipt->s) != m.end())
                throw RuntimeError("non-unique bindings");
            env = extend(ipt->s, TerminateV(), env);
            m[ipt->s] = 1;
        }

        q = Bindvct.begin();

        for (; q != Bindvct.end(); ++q)
        {
            if (typeid(*q->get()) != typeid(List))
                throw RuntimeError("Inadequate parameter format");
            List *BindArray = dynamic_cast<List *>(q->get());

            // 检查内部每一个赋值列表类型以及元素个数
            if (BindArray->stxs.size() != 2)
                throw RuntimeError("Inadequate parameter format");
            if (typeid(*BindArray->stxs.begin()->get()) != typeid(Identifier))
                throw RuntimeError("Bad variable name");

            // 将变量放入 letrec 的 expr 中
            auto pt = BindArray->stxs.begin();
            Identifier *ipt = dynamic_cast<Identifier *>(pt->get());
            ++pt;
            bind.push_back({ipt->s, pt->parse(env)});
        }

        ++p;
        Expr body = p->parse(env);

        ExprBase *temp = new Letrec(bind, body);
        env = env2;
        return Expr(temp);
    }
    else if (t->e_type == E_PLUS)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new Plus(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_MINUS)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new Minus(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_MUL)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new Mult(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_LT)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new Less(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_LE)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new LessEq(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_EQ)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new Equal(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_GE)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new GreaterEq(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_GT)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new Greater(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_EXIT)
    {
        if (stxs.size() != 1)
            throw RuntimeError("Bad parameter number");
        ExprBase *temp = new Exit();
        return Expr(temp);
    }
    else if (t->e_type == E_CONS)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new Cons(rand1, rand2);
        return Expr(temp);
    }
    else if (t->e_type == E_CAR)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new Car(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_CDR)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new Cdr(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_NOT)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new Not(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_BOOLQ)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new IsBoolean(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_INTQ)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new IsFixnum(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_NULLQ)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new IsNull(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_PAIRQ)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new IsPair(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_PROCQ)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new IsProcedure(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_SYMBOLQ)
    {
        if (stxs.size() != 2)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        ExprBase *temp = new IsSymbol(rand1);
        return Expr(temp);
    }
    else if (t->e_type == E_EQQ)
    {
        if (stxs.size() != 3)
            throw RuntimeError("Bad parameter number");
        Expr rand1 = stxs[1].parse(env);
        Expr rand2 = stxs[2].parse(env);
        ExprBase *temp = new IsEq(rand1, rand2);
        return Expr(temp);
    }
    else
    {
        throw RuntimeError("Bad format");
    }
}

#endif