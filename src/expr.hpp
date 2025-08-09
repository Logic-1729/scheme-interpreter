#ifndef EXPRESSION
#define EXPRESSION

/**
 * @file expr.hpp
 * @brief Expression structures for the Scheme interpreter
 * @author luke36
 * 
 * This file defines all expression types used in the Scheme interpreter.
 * Structures are organized according to ExprType enumeration order from
 * Def.hpp for consistency and maintainability.
 */

#include "Def.hpp"
#include "syntax.hpp"
#include <memory>
#include <cstring>
#include <vector>

struct ExprBase{
    ExprType e_type;
    ExprBase(ExprType);
    virtual Value eval(Assoc &) = 0;
    virtual ~ExprBase() = default;
};

class Expr {
    std::shared_ptr<ExprBase> ptr;
public:
    Expr(ExprBase *);
    ExprBase* operator->() const;
    ExprBase& operator*();
    ExprBase* get() const;
};

// ================================================================================
//                             CONTROL STRUCTURES
// ================================================================================

/**
 * @brief Let binding expression
 * Creates local variable bindings for expression evaluation
 */
struct Let : ExprBase {
    std::vector<std::pair<std::string, Expr>> bind;
    Expr body;
    Let(const std::vector<std::pair<std::string, Expr>> &, const Expr &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Lambda (function) expression
 * Creates a closure with parameter list and body
 */
struct Lambda : ExprBase {
    std::vector<std::string> x;
    Expr e;
    Lambda(const std::vector<std::string> &, const Expr &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Function application expression
 * Handles function calling where rator is operator and rand are operands
 */
struct Apply : ExprBase {
    Expr rator;
    std::vector<Expr> rand;
    Apply(const Expr &, const std::vector<Expr> &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Recursive let binding expression
 * Supports mutually recursive function definitions
 */
struct Letrec : ExprBase {
    std::vector<std::pair<std::string, Expr>> bind;
    Expr body;
    Letrec(const std::vector<std::pair<std::string, Expr>> &, const Expr &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Conditional (if) expression
 * Evaluates condition and chooses between true/false branches
 */
struct If : ExprBase {
  Expr cond;
  Expr conseq;
  Expr alter;
  If(const Expr &, const Expr &, const Expr &);
  virtual Value eval(Assoc &) override;
};

// ================================================================================
//                        VARIABLE AND DEFINITION MANAGEMENT
// ================================================================================

/**
 * @brief Variable/function definition expression  
 * Defines a new variable or function in the current environment
 */
struct Define : ExprBase {
    std::string var;
    Expr e;
    Define(const std::string &, const Expr &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Variable assignment expression
 * Modifies an existing variable's value
 */
struct Set : ExprBase {
    std::string var;
    Expr e;
    Set(const std::string &, const Expr &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Sequential execution (begin) expression
 * Evaluates expressions in order and returns the last result
 */
struct Begin : ExprBase {
    std::vector<Expr> es;
    Begin(const std::vector<Expr> &);
    virtual Value eval(Assoc &) override;
};

// ================================================================================
//                       REMAINING CONTROL STRUCTURES  
// ================================================================================

/**
 * @brief Multi-way conditional (cond) expression
 * Evaluates clauses in order until one matches
 */
struct Cond : ExprBase {
    std::vector<std::vector<Expr>> clauses;
    Cond(const std::vector<std::vector<Expr>> &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Logical AND expression
 * Short-circuit evaluation of boolean expressions
 */
struct And : ExprBase {
    std::vector<Expr> es;
    And(const std::vector<Expr> &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Logical OR expression  
 * Short-circuit evaluation of boolean expressions
 */
struct Or : ExprBase {
    std::vector<Expr> es;
    Or(const std::vector<Expr> &);
    virtual Value eval(Assoc &) override;
};

// ================================================================================
//                              BASIC TYPES AND LITERALS
// ================================================================================

/**
 * @brief Variable reference expression
 * References a variable in the current environment
 */
struct Var : ExprBase {
    std::string x;
    Var(const std::string &);
    virtual Value eval(Assoc &) override;
};

/**
 * @brief Integer literal expression
 * Represents fixed-point numbers (integers)
 */
struct Fixnum : ExprBase {
  int n;
  Fixnum(int);
  virtual Value eval(Assoc &) override;
};

/**
 * @brief String literal expression
 * Represents string values
 */
struct StringExpr : ExprBase {
  std::string s;
  StringExpr(const std::string &);
  virtual Value eval(Assoc &) override;
};

/**
 * @brief Boolean true literal
 */
struct True : ExprBase {
  True();
  virtual Value eval(Assoc &) override;
};

/**
 * @brief Boolean false literal  
 */
struct False : ExprBase {
  False();
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

struct Variadic : ExprBase {
    std::vector<Expr> rands;
    Variadic(ExprType, const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) = 0;
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

struct Div : Binary {
    Div(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

// 多参数算术运算符
struct MultVar : Variadic {
    MultVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct PlusVar : Variadic {
    PlusVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct MinusVar : Variadic {
    MinusVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct DivVar : Variadic {
    DivVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct ListFunc : Variadic {
    ListFunc(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
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

struct LessVar : Variadic {
    LessVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct LessEqVar : Variadic {
    LessEqVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct EqualVar : Variadic {
    EqualVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct GreaterEqVar : Variadic {
    GreaterEqVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct GreaterVar : Variadic {
    GreaterVar(const std::vector<Expr> &);
    virtual Value evalRator(const std::vector<Value> &) override;
};

struct IsEq : Binary {
    IsEq(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Cons : Binary {
    Cons(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Quotient : Binary {
    Quotient(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Modulo : Binary {
    Modulo(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Expt : Binary {
    Expt(const Expr &, const Expr &);
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

struct IsString : Unary {
    IsString(const Expr &);
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

struct IsList : Unary {
    IsList(const Expr &);
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

struct SetCar : Binary {
    SetCar(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct SetCdr : Binary {
    SetCdr(const Expr &, const Expr &);
    virtual Value evalRator(const Value &, const Value &) override;
};

struct Display : Unary {
    Display(const Expr &);
    virtual Value evalRator(const Value &) override;
};

#endif
