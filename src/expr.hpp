#ifndef EXPRESSION
#define EXPRESSION

#include "Def.hpp"
#include "syntax.hpp"
#include "shared.hpp"
#include <memory>
#include <cstring>
#include <vector>

struct ExprBase
{
    ExprType e_type;
    ExprBase(ExprType);
    virtual Value eval(Assoc &) = 0;
    virtual ~ExprBase() = default;
};

struct Expr {
    SharedPtr<ExprBase> ptr;
    Expr(ExprBase *);
    ExprBase* operator -> () const;
    ExprBase& operator * ();
    ExprBase* get() const;
};

struct Let : ExprBase {
    std::vector<std::pair<std::string, Expr>> bind;
    Expr body;
    Let(const std :: vector<std :: pair<std :: string, Expr>> &, const Expr &);
    virtual Value eval(Assoc &) override;
};

struct Lambda : ExprBase {
    std::vector<std::string> x;
    Expr e;
    Lambda(const std :: vector<std :: string> &, const Expr &);
    virtual Value eval(Assoc &) override;
};

struct Apply : ExprBase {
    Expr rator;
    std::vector<Expr> rand;
    Apply(const Expr &, const std :: vector<Expr> &);
    virtual Value eval(Assoc &) override;
}; // this is used to handle function calling, where rator is the operator and rands are operands

struct Letrec : ExprBase {
  std::vector<std::pair<std::string, Expr>> bind;
  Expr body;
  Letrec(const std :: vector<std :: pair<std :: string, Expr>> &, const Expr &);
  virtual Value eval(Assoc &) override;
};

struct Var : ExprBase {
  std::string x;
  Var(const std :: string &);
  virtual Value eval(Assoc &) override;
};

struct Fixnum : ExprBase {
  int n;
  Fixnum(int);
  virtual Value eval(Assoc &) override;
};

struct If : ExprBase {
  Expr cond;
  Expr conseq;
  Expr alter;
  If(const Expr &, const Expr &, const Expr &);
  virtual Value eval(Assoc &) override;
};

struct True : ExprBase {
  True();
  virtual Value eval(Assoc &) override;
};

struct False : ExprBase {
  False();
  virtual Value eval(Assoc &) override;
};

struct Begin : ExprBase {
  std::vector<Expr> es;
  Begin(const std :: vector<Expr> &);
  virtual Value eval(Assoc &) override;
};

struct Quote : ExprBase {
  Syntax s;
  Quote(const Syntax &);
  virtual Value eval(Assoc &) override;
};

struct MakeVoid : ExprBase {
    MakeVoid();
    virtual Value eval(Assoc &) override;
};

struct Exit : ExprBase {
    Exit();
    virtual Value eval(Assoc &) override;
};

struct Binary : ExprBase {
    Expr rand1;
    Expr rand2;
    Binary(ExprType, const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) = 0;
    virtual Value eval(Assoc &) override;
};

struct Unary : ExprBase {
    Expr rand;
    Unary(ExprType, const Expr &);
    virtual Value evalRator(const Value &) = 0;
    virtual Value eval(Assoc &) override;
};

struct Mult : Binary {
    Mult(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Plus : Binary {
    Plus(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Minus : Binary {
    Minus(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Less : Binary {
    Less(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct LessEq : Binary {
    LessEq(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Equal : Binary {
    Equal(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct GreaterEq : Binary {
    GreaterEq(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Greater : Binary {
    Greater(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct IsEq : Binary {
    IsEq(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Cons : Binary {
    Cons(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct IsBoolean : Unary {
    IsBoolean(const Expr &);
    virtual Value evalRator(const Value &) override;
};

struct IsFixnum : Unary {
    IsFixnum(const Expr &);
    virtual Value evalRator(const Value &) override;
};

struct IsSymbol : Unary {
    IsSymbol(const Expr &);
    virtual Value evalRator(const Value &) override;
};

struct IsNull : Unary {
    IsNull(const Expr &);
    virtual Value evalRator(const Value &) override;
};

struct IsPair : Unary {
    IsPair(const Expr &);
    virtual Value evalRator(const Value &) override;
};

struct IsProcedure : Unary {
    IsProcedure(const Expr &);
    virtual Value evalRator(const Value &) override;
};

struct Not : Unary {
    Not(const Expr &);
    virtual Value evalRator(const Value &) override;
};

struct Car : Unary {
    Car(const Expr &);
    virtual Value evalRator(const Value &) override;
};

struct Cdr : Unary {
    Cdr(const Expr &);
    virtual Value evalRator(const Value &) override;
};

#endif
