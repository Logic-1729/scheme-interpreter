#ifndef VALUE 
#define VALUE

#include "Def.hpp"
#include "shared.hpp"
#include "expr.hpp"
#include <memory>
#include <cstring>
#include <vector>

struct ValueBase {
    ValueType v_type;
    ValueBase(ValueType);
    virtual void show(std::ostream &) = 0;
    virtual void showCdr(std::ostream &);
    virtual ~ValueBase() = default;
};

struct Value {
    SharedPtr<ValueBase> ptr;
    Value(ValueBase *);
    void show(std::ostream &);
    ValueBase* operator -> () const;
    ValueBase& operator * ();
    ValueBase* get() const;
};

struct Assoc {
  SharedPtr<AssocList> ptr;
  Assoc(AssocList *);
  AssocList* operator -> () const;
  AssocList& operator * ();
  AssocList* get() const;
};

struct AssocList {
  std::string x;
  Value v;
  Assoc next;
  AssocList(const std::string &, const Value &, Assoc &);
};

struct Void : ValueBase {
  Void();
  virtual void show(std::ostream &) override;
};
Value VoidV();

struct Integer : ValueBase {
  int n;
  Integer(int);
  virtual void show(std::ostream &) override;
};
Value IntegerV(int);

struct Boolean : ValueBase {
  bool b;
  Boolean(bool);
  virtual void show(std::ostream &) override;
};
Value BooleanV(bool);

struct Symbol : ValueBase {
  std::string s;
  Symbol(const std::string &);
  virtual void show(std::ostream &) override;
};
Value SymbolV(const std::string &);

struct Null : ValueBase {
  Null();
  virtual void show(std::ostream &) override;
  virtual void showCdr(std::ostream &) override;
};
Value NullV();

struct Terminate : ValueBase {
  Terminate();
  virtual void show(std::ostream &) override;
};
Value TerminateV();

struct Pair : ValueBase {
  Value car;
  Value cdr;
  Pair(const Value &, const Value &);
  virtual void show(std::ostream &) override;
  virtual void showCdr(std::ostream &) override;
};
Value PairV(const Value &, const Value &);

struct Closure : ValueBase {
  std::vector<std::string> parameters;
  Expr e;
  Assoc env;
  Closure(const std::vector<std::string> &, const Expr &, const Assoc &);
  virtual void show(std::ostream &) override;
};
Value ClosureV(const std::vector<std::string> &, const Expr &, const Assoc &);

struct String : ValueBase 
{
    std :: string s;
    String(const std :: string &);
    virtual void show(std :: ostream &) override;
};
Value StringV(const std :: string &);

std::ostream &operator<<(std::ostream &, Value &);

Assoc empty();
Assoc extend(const std :: string&, const Value &, Assoc &);
void modify(const std :: string&, const Value &, Assoc &);
Value find(const std::string &, Assoc &);
#endif