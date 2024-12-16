#include "value.hpp"
#include "expr.hpp"
#include "RE.hpp"
#include "syntax.hpp"
#include <cstring>
#include <vector>
#include <map>

extern std :: map<std :: string, ExprType> primitives;
extern std :: map<std :: string, ExprType> reserved_words;

Value Let::eval(Assoc &env) {
    Assoc cur_env = env;
    std::vector< std::pair<std::string, Value> > tobind;
    for(auto binded_pair : bind){
        tobind.push_back({binded_pair.first, binded_pair.second->eval(env)});
    }
    for(auto binded_pair: tobind){
        cur_env = extend(binded_pair.first, binded_pair.second, cur_env);
    }
    return body->eval(cur_env);
}

Value Lambda::eval(Assoc &env) { // lambda expression
    Assoc new_env=env;
    return ClosureV(x,e,new_env);
}

Value Apply::eval(Assoc &e) {
    Value mid_fun = rator->eval(e);
    if (mid_fun->v_type != V_PROC) {
        throw RuntimeError("Attempt to apply a non-procedure");
    }

    Closure* clos_ptr = dynamic_cast<Closure*>(mid_fun.get());
    Assoc cur_env = clos_ptr->env;
    std::vector<Value> args;

    for (int i = 0; i < rand.size(); i++) {
        args.push_back(rand[i]->eval(e));
    }

    if (args.size() != clos_ptr->parameters.size()) {
        throw RuntimeError("Wrong number of arguments");
    }

    for (int i = 0; i < clos_ptr->parameters.size(); i++) {
        cur_env = extend(clos_ptr->parameters[i], args[i], cur_env);
    }

    return clos_ptr->e->eval(cur_env);
}

Value Letrec::eval(Assoc &env) {
    Assoc new_env = env;

    // 1. 先将所有变量绑定到 null
    for (const auto &binding : bind) {
        new_env = extend(binding.first, NullV(), new_env);
    }

    // 2. 逐个求值并更新绑定
    for (const auto &binding : bind) {
        Value val = binding.second->eval(new_env);
        modify(binding.first, val, new_env);
    }

    // 3. 求值 body
    return body->eval(new_env);
}

Value Var::eval(Assoc &e) { // evaluation of variable
    Value matched_value = find(x, e);
    if(matched_value.get()==nullptr){
        // std::cout<<"failure when looking for var:"<<x<<std::endl;
        throw(RuntimeError("undefined variable"));
    }
    return matched_value;
}

Value Fixnum::eval(Assoc &e) { // evaluation of a fixnum
    return IntegerV(n);
}

Value If::eval(Assoc &e) {
    // if expression
    Value valueof_condition = cond->eval(e);
    if(valueof_condition->v_type == V_BOOL and dynamic_cast<Boolean*>(valueof_condition.get())->b == false)
        return alter->eval(e);
    else return conseq->eval(e);
}

Value True::eval(Assoc &e) { // evaluation of #t
    return BooleanV(true);
}

Value False::eval(Assoc &e) { // evaluation of #f
    return BooleanV(false);
}

Value Begin::eval(Assoc &e) {
    if (es.size() == 0) return VoidV();
    for (int i = 0; i < es.size() - 1; i++) {
        es[i]->eval(e);
    }
    return es[es.size() - 1]->eval(e);
}

Value Quote::eval(Assoc& e) {
    if (dynamic_cast<TrueSyntax*>(s.get())) return BooleanV(true);
    else if (dynamic_cast<FalseSyntax*>(s.get())) return BooleanV(false);
    else if (dynamic_cast<Number*>(s.get())) return IntegerV(dynamic_cast<Number*>(s.get())->n);
    else if (dynamic_cast<Identifier*>(s.get())) return SymbolV(dynamic_cast<Identifier*>(s.get())->s);
    else if (dynamic_cast<List*>(s.get())) {
        auto stxs_got=dynamic_cast<List*>(s.get())->stxs; List* temp = new List;
        if (dynamic_cast<List*>(s.get())->stxs.empty()) {
            return NullV();
        } else if (stxs_got.size() == 1) {
            return PairV(Value(Quote(stxs_got[0]).eval(e)), NullV());
        } else {
            int pos = -1, cnt = 0 , len=stxs_got.size();
            for (int i = 0; i < len; i++) {
                pos = (((dynamic_cast<Identifier*>(stxs_got[i].get()))&&(dynamic_cast<Identifier*>(stxs_got[i].get())->s == "."))? (i) :(pos));
                cnt = (((dynamic_cast<Identifier*>(stxs_got[i].get()))&&(dynamic_cast<Identifier*>(stxs_got[i].get())->s == ".")) ? (cnt+1): (cnt));
            }
            if ((cnt > 1 || ((pos != len - 2) && (cnt)))||(cnt == 1 && (len < 3))) {
                throw RuntimeError("Parm isn't fit");
            }
            if (len == 3) {
                if ((dynamic_cast<Identifier*>(stxs_got[1].get()))&&(dynamic_cast<Identifier*>(stxs_got[1].get())->s == ".")) {
                    return PairV(Quote(stxs_got[0]).eval(e), Quote(stxs_got[2]).eval(e));
                }
            }
            (*temp).stxs = std::vector<Syntax>(stxs_got.begin() + 1, stxs_got.end());
            return PairV(Value(Quote(stxs_got.front()).eval(e)), Value(Quote(Syntax(temp)).eval(e)));
        }
    } else throw(RuntimeError("Unknown quoted typename"));
}

Value MakeVoid::eval(Assoc &e) { // (void)
    return VoidV();
}

Value Exit::eval(Assoc &e) { // (exit)
    return TerminateV();
}

Value Binary::eval(Assoc &e) { // evaluation of two-operators primitive
    return evalRator(rand1->eval(e),rand2->eval(e));
}

Value Unary::eval(Assoc &e) { // evaluation of single-operator primitive
    return evalRator(rand->eval(e));
}

Value Mult::evalRator(const Value &rand1, const Value &rand2) { // *
    // std::cout<<rand1->v_type<<' '<<rand2->v_type<<std::endl;
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) * (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Plus::evalRator(const Value &rand1, const Value &rand2) { // +
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) + (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Minus::evalRator(const Value &rand1, const Value &rand2) { // -
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return IntegerV((dynamic_cast<Integer*>(rand1.get())->n) - (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Less::evalRator(const Value &rand1, const Value &rand2) { // <
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) < (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value LessEq::evalRator(const Value &rand1, const Value &rand2) { // <=
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) <= (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Equal::evalRator(const Value &rand1, const Value &rand2) { // =
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) == (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value GreaterEq::evalRator(const Value &rand1, const Value &rand2) { // >=
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) >= (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
}

Value Greater::evalRator(const Value &rand1, const Value &rand2) { // >
    if(rand1->v_type==V_INT and rand2->v_type==V_INT){
        return BooleanV((dynamic_cast<Integer*>(rand1.get())->n) > (dynamic_cast<Integer*>(rand2.get())->n));
    }
    throw(RuntimeError("Wrong typename"));
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
    }else {
        return BooleanV(rand1.get() == rand2.get());
    }
}

Value Cons::evalRator(const Value &rand1, const Value &rand2) { // cons
    return PairV(rand1,rand2);
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

Value IsNull::evalRator(const Value &rand) { // null?
    return BooleanV(rand->v_type == V_NULL);
}

Value IsPair::evalRator(const Value &rand) { // pair?
    return BooleanV(rand->v_type == V_PAIR);
}

Value IsProcedure::evalRator(const Value &rand) { // procedure?
    return BooleanV(rand->v_type == V_PROC);
}

Value Not::evalRator(const Value &rand) { // not
    if(rand->v_type == V_BOOL and (dynamic_cast<Boolean*>(rand.get())->b == false))
        return BooleanV(true);
    else return BooleanV(false);
}

Value Car::evalRator(const Value &rand) { // car
    if(rand->v_type == V_PAIR)
        return dynamic_cast<Pair*>(rand.get())->car;
    else throw(RuntimeError("Wrong typename"));
}

Value Cdr::evalRator(const Value &rand) { // cdr
    if(rand->v_type == V_PAIR)
        return dynamic_cast<Pair*>(rand.get())->cdr;
    else throw(RuntimeError("Wrong typename"));
}