#include "Def.hpp"
#include "expr.hpp"
#include <cstring>
#include <vector>
using std :: vector;
using std :: string;
using std :: pair;

ExprBase :: ExprBase(ExprType et) : e_type(et) {}

Expr :: Expr(ExprBase * eb) : ptr(eb) {}
ExprBase* Expr :: operator -> () const { return ptr.get(); }
ExprBase& Expr :: operator * () { return *ptr; }
ExprBase* Expr :: get() const { return ptr.get(); }

Let :: Let(const vector<pair<string, Expr>> &vec, const Expr &e) : ExprBase(E_LET), bind(vec), body(e) {}

Lambda :: Lambda(const vector<string> &vec, const Expr &expr) : ExprBase(E_LAMBDA), x(vec), e(expr) {}

Apply :: Apply(const Expr &expr, const vector<Expr> &vec) : ExprBase(E_APPLY), rator(expr), rand(vec) {}

Letrec :: Letrec(const vector<pair<string, Expr>> &vec, const Expr &expr) : ExprBase(E_LETREC), bind(vec), body(expr) {}

Var :: Var(const string &s) : ExprBase(E_VAR), x(s) {}

Fixnum :: Fixnum(int x) : ExprBase(E_FIXNUM), n(x) {}

If :: If(const Expr &c, const Expr &c_t, const Expr &c_e) : ExprBase(E_IF), cond(c), conseq(c_t), alter(c_e) {}

True :: True() : ExprBase(E_TRUE) {}

False :: False() : ExprBase(E_FALSE) {}

Begin :: Begin(const vector<Expr> &vec) : ExprBase(E_BEGIN), es(vec) {}

Quote :: Quote(const Syntax &t) : ExprBase(E_QUOTE), s(t) {}

MakeVoid :: MakeVoid() : ExprBase(E_VOID) {}

Exit :: Exit() : ExprBase(E_EXIT) {}

Binary :: Binary(ExprType et, const Expr &r1, const Expr &r2) : ExprBase(et), rand1(r1), rand2(r2) {}

Unary :: Unary(ExprType et, const Expr &expr) : ExprBase(et), rand(expr) {}

Mult :: Mult(const Expr &r1, const Expr &r2) : Binary(E_MUL, r1, r2) {}

Plus :: Plus(const Expr &r1, const Expr &r2) : Binary(E_PLUS, r1, r2) {}

Minus :: Minus(const Expr &r1, const Expr &r2) : Binary(E_MINUS, r1, r2) {}

Less :: Less(const Expr &r1, const Expr &r2) : Binary(E_LT, r1, r2) {}

LessEq :: LessEq(const Expr &r1, const Expr &r2) : Binary(E_LE, r1, r2) {}

Equal :: Equal(const Expr &r1, const Expr &r2) : Binary(E_EQ, r1, r2) {}

GreaterEq :: GreaterEq(const Expr &r1, const Expr &r2) : Binary(E_GE, r1, r2) {}

Greater :: Greater(const Expr &r1, const Expr &r2) : Binary(E_GT, r1, r2) {}

IsEq :: IsEq(const Expr &r1, const Expr &r2) : Binary(E_EQQ, r1, r2) {}

Cons :: Cons(const Expr &r1, const Expr &r2) : Binary(E_CONS, r1, r2) {}

IsBoolean :: IsBoolean(const Expr &r1) : Unary(E_BOOLQ, r1) {}

IsFixnum :: IsFixnum(const Expr &r1) : Unary(E_INTQ, r1) {}

IsSymbol :: IsSymbol(const Expr &r1) : Unary(E_SYMBOLQ, r1) {}

IsNull :: IsNull(const Expr &r1) : Unary(E_NULLQ, r1) {}

IsPair :: IsPair(const Expr &r1) : Unary(E_PAIRQ, r1) {}

IsProcedure :: IsProcedure(const Expr &r1) : Unary(E_PROCQ, r1) {}

Not :: Not(const Expr &r1) : Unary(E_NOT, r1) {}

Car :: Car(const Expr &r1) : Unary(E_CAR, r1) {}

Cdr :: Cdr(const Expr &r1) : Unary(E_CDR, r1) {}