#include "Def.hpp"
#include "value.hpp"
#include "expr.hpp"
#include "RE.hpp"
#include "syntax.hpp"
#include <cstring>
#include <vector>
#include <map>

extern std ::map<std ::string, ExprType> primitives;
extern std ::map<std ::string, ExprType> reserved_words;

Value ExprBase::eval(Assoc &env) { throw RuntimeError("Bad keyword format"); }

Value Let::eval(Assoc &env)
{
    Assoc env2 = env;
    for (auto p : bind)
    {
        env = extend(p.first, p.second->eval(env2), env);
    }
    Value v = body->eval(env);
    env = env2;
    return v;
} // let expression

Value Lambda::eval(Assoc &env)
{
    return ClosureV(x, e, env);
} // lambda expression

Value Apply::eval(Assoc &e)
{
    Value clos = rator->eval(e);
    if (typeid(*clos.get()) != typeid(Closure))
        throw RuntimeError("Value type is not a closure in Apply");
    Closure *proc = dynamic_cast<Closure *>(clos.get());
    Assoc env2 = proc->env;
    if (proc->parameters.size() != rand.size())
        throw RuntimeError("The number of the parameter list and the actual operands doesn't match in Apply");
    for (int i = 0; i < rand.size(); ++i)
    {
        env2 = extend(proc->parameters[i], rand[i]->eval(e), env2);
    }
    return proc->e->eval(env2);
} // for function calling

Value Letrec::eval(Assoc &env)
{
    Assoc env2 = env;
    for (auto p : bind)
    {
        env = extend(p.first, real_voidV(), env);
    }
    std::vector<Value> vct; // 用于记录所有求得的值
    for (auto p : bind)
    {
        Value v = p.second->eval(env);
        vct.push_back(v);
    }
    // 修改 Closure 中的作用域
    for (int i = 0; i < bind.size(); ++i)
    {
        modify(bind[i].first, vct[i], env);
    }
    Value v = body->eval(env);
    env = env2;
    return v;
} // letrec expression

Value Var::eval(Assoc &e)
{
    Value v = find(x, e);
    if (v.get() == nullptr)
        throw RuntimeError("Variant hasn't been declared for " + x);
    if (v->v_type == V_RVOID)
        throw RuntimeError("Variant has been declared but not been binded for " + x);
    return v;
} // evaluation of variable

Value Fixnum::eval(Assoc &e)
{
    ValueBase *pt = new Integer(n);
    Value temp(pt);
    return temp;
} // evaluation of a fixnum

Value If::eval(Assoc &e)
{
    Value temp1 = cond->eval(e);
    if (temp1->v_type == V_BOOL)
    {
        Boolean *temp = dynamic_cast<Boolean *>(temp1.get());
        if (temp->b == false)
            return alter->eval(e);
    }
    return conseq->eval(e);
} // if expression

Value True::eval(Assoc &e)
{
    ValueBase *pt = new Boolean(true);
    Value temp(pt);
    return temp;
} // evaluation of #t

Value False::eval(Assoc &e)
{
    ValueBase *pt = new Boolean(false);
    Value temp(pt);
    return temp;
} // evaluation of #f

Value Begin::eval(Assoc &e)
{
    if (es.size() == 0)
        return NullV();
    for (int i = 0; i < es.size() - 1; ++i)
    {
        es[i]->eval(e);
    }
    return es[es.size() - 1]->eval(e);
} // begin expression

Value Quote::eval(Assoc &e)
{
    if (typeid(*s.get()) == typeid(List))
    {
        List *list = dynamic_cast<List *>(s.get());
        if (list->stxs.size() == 0)
            return NullV();

        // 处理第一个元素
        Syntax now = list->stxs[0];
        Quote fstq(now);
        Value v1 = fstq.eval(e);

        // 递归处理剩下元素
        List *nxt = new List();
        auto it = list->stxs.begin() + 1;
        nxt->stxs.assign(it, list->stxs.end());
        Syntax tt(nxt);
        Expr ttt(new Quote(tt));
        return Value(new Pair(v1, ttt->eval(e)));
    }
    else if (typeid(*s.get()) == typeid(Identifier))
    {
        Identifier *temp = dynamic_cast<Identifier *>(s.get());
        ValueBase *t = new Symbol(temp->s);
        return Value(t);
    }
    else
    {
        return s->parse(e)->eval(e);
    }
} // quote expression

Value MakeVoid::eval(Assoc &e)
{
    return VoidV();
} // (void)

Value Exit::eval(Assoc &e)
{
    ValueBase *temp = new Terminate();
    return Value(temp);
} // (exit)

Value Binary::eval(Assoc &e)
{
    Value v1 = rand1->eval(e);
    Value v2 = rand2->eval(e);
    return evalRator(v1, v2);
} // evaluation of two-operators primitive

Value Unary::eval(Assoc &e)
{
    Value v1 = rand->eval(e);
    return evalRator(v1);
} // evaluation of single-operator primitive

Value Mult::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type != V_INT || rand2->v_type != V_INT)
        throw RuntimeError("Value type error in *");
    Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
    Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
    return IntegerV(temp1->n * temp2->n);
} // *

Value Plus::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type != V_INT || rand2->v_type != V_INT)
        throw RuntimeError("Value type error in +");
    Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
    Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
    return IntegerV(temp1->n + temp2->n);
} // +

Value Minus::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type != V_INT || rand2->v_type != V_INT)
        throw RuntimeError("Value type error in -");
    Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
    Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
    return IntegerV(temp1->n - temp2->n);
} // -

Value Less::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type != V_INT || rand2->v_type != V_INT)
        throw RuntimeError("Value type error in <");
    Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
    Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
    return BooleanV(temp1->n < temp2->n);
} // <

Value LessEq::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type != V_INT || rand2->v_type != V_INT)
        throw RuntimeError("Value type error in <=");
    Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
    Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
    return BooleanV(temp1->n <= temp2->n);
} // <=

Value Equal::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type != V_INT || rand2->v_type != V_INT)
        throw RuntimeError("Value type error in =");
    Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
    Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
    return BooleanV(temp1->n == temp2->n);
} // =

Value GreaterEq::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type != V_INT || rand2->v_type != V_INT)
        throw RuntimeError("Value type error in >=");
    Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
    Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
    return BooleanV(temp1->n >= temp2->n);
} // >=

Value Greater::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type != V_INT || rand2->v_type != V_INT)
        throw RuntimeError("Value type error in >");
    Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
    Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
    return BooleanV(temp1->n > temp2->n);
} // >

Value IsEq::evalRator(const Value &rand1, const Value &rand2)
{
    if (rand1->v_type == V_INT && rand2->v_type == V_INT)
    {
        Integer *temp1 = dynamic_cast<Integer *>(rand1.get());
        Integer *temp2 = dynamic_cast<Integer *>(rand2.get());
        return BooleanV(temp1->n == temp2->n);
    }
    if (rand1->v_type == V_BOOL && rand2->v_type == V_BOOL)
    {
        Boolean *temp1 = dynamic_cast<Boolean *>(rand1.get());
        Boolean *temp2 = dynamic_cast<Boolean *>(rand2.get());
        return BooleanV(temp1->b == temp2->b);
    }
    if (rand1->v_type == V_SYM && rand2->v_type == V_SYM)
    {
        Symbol *temp1 = dynamic_cast<Symbol *>(rand1.get());
        Symbol *temp2 = dynamic_cast<Symbol *>(rand2.get());
        return BooleanV(temp1->s == temp2->s);
    }
    return BooleanV(rand1.get() == rand2.get());
} // eq?

Value Cons::evalRator(const Value &rand1, const Value &rand2)
{
    ValueBase *temp = new Pair(rand1, rand2);
    return Value(temp);
} // cons

Value IsBoolean::evalRator(const Value &rand)
{
    return BooleanV(rand->v_type == V_BOOL);
} // boolean?

Value IsFixnum::evalRator(const Value &rand)
{
    return BooleanV(rand->v_type == V_INT);
} // fixnum?

Value IsSymbol::evalRator(const Value &rand)
{
    return BooleanV(rand->v_type == V_SYM);
} // symbol?

Value IsNull::evalRator(const Value &rand)
{
    return BooleanV(rand->v_type == V_NULL);
} // null?

Value IsPair::evalRator(const Value &rand)
{
    return BooleanV(rand->v_type == V_PAIR);
} // pair?

Value IsProcedure::evalRator(const Value &rand)
{
    return BooleanV(rand->v_type == V_PROC);
} // procedure?

Value Not::evalRator(const Value &rand)
{
    if (rand->v_type == V_BOOL)
    {
        Boolean *temp = dynamic_cast<Boolean *>(rand.get());
        if (temp->b == false)
            return BooleanV(1);
    }
    return BooleanV(0);
} // not

Value Car::evalRator(const Value &rand)
{
    if (rand->v_type != V_PAIR)
        throw RuntimeError("Bad value type (should be pair)");
    Pair *temp = dynamic_cast<Pair *>(rand.get());
    return temp->car;
} // car

Value Cdr::evalRator(const Value &rand)
{
    if (rand->v_type != V_PAIR)
        throw RuntimeError("Bad value type (should be pair)");
    Pair *temp = dynamic_cast<Pair *>(rand.get());
    return temp->cdr;
} // cdr