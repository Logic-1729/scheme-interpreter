#include "value.hpp"

AssocList::AssocList(const std::string &x, const Value &v, Assoc &next)
  : x(x), v(v), next(next) {}

Assoc::Assoc(AssocList *x) : ptr(x) {}
AssocList* Assoc :: operator -> () const { return ptr.get(); }
AssocList& Assoc :: operator * () { return *ptr; }
AssocList* Assoc :: get() const { return ptr.get(); }

Assoc empty() {
  return Assoc(nullptr);
}

Assoc extend(const std::string &x, const Value &v, Assoc &lst)
{
    return Assoc(new AssocList(x, v, lst));
}

void modify(const std::string &x, const Value &v, Assoc &lst)
{
    for (auto i = lst; i.get() != nullptr; i = i -> next)
        if (x == i -> x)
        {
            i -> v = v;
            return;
        }
}

Value find(const std::string &x, Assoc &l) {
  for (auto i = l; i.get() != nullptr; i = i -> next)
    if (x == i -> x)
      return i -> v;
  return Value(nullptr);
}

std::ostream &operator<<(std::ostream &os, Value &v) {
  v -> show(os);
  return os;
}

void ValueBase::showCdr(std::ostream &os) {
  os << " . ";
  show(os);
  os << ')';
}

void Void::show(std::ostream &os) {
  os << "#<void>";
}

void Integer::show(std::ostream &os) {
  os << n;
}

void Boolean::show(std::ostream &os) {
  os << (b ? "#t" : "#f");
}

void Symbol::show(std::ostream &os) {
    os << s;
}

void Null::show(std::ostream &os) {
  os << "()";
}

void Null::showCdr(std::ostream &os) {
  os << ')';
}

void Terminate::show(std::ostream &os) {
  os << "()";
}

void Pair::show(std::ostream &os) {
  os << '(' << car;
  cdr->showCdr(os);
}

void Pair::showCdr(std::ostream &os) {
  os << ' ' << car;
  cdr->showCdr(os);
}

void Closure::show(std::ostream &os) {
  os << "#<procedure>";
}

ValueBase :: ValueBase(ValueType vt) : v_type(vt) {}

Value :: Value(ValueBase *ptr) : ptr(ptr) {}
ValueBase* Value :: operator -> () const { return ptr.get(); }
ValueBase& Value :: operator * () { return *ptr; }
ValueBase* Value :: get() const { return ptr.get(); }

Void::Void() : ValueBase(V_VOID) {}
Value VoidV() {
  return Value(new Void());
}

Integer::Integer(int n) : ValueBase(V_INT), n(n) {}
Value IntegerV(int n) {
  return Value(new Integer(n));
}

Boolean::Boolean(bool b) : ValueBase(V_BOOL), b(b) {}
Value BooleanV(bool b) {
  return Value(new Boolean(b));
}

Symbol::Symbol(const std::string &s) : ValueBase(V_SYM), s(s) {}
Value SymbolV(const std::string &s) {
  return Value(new Symbol(s));
}

Null::Null() : ValueBase(V_NULL) {}
Value NullV() {
  return Value(new Null());
}

Terminate::Terminate() : ValueBase(V_TERMINATE) {}
Value TerminateV() {
  return Value(new Terminate());
}

Pair::Pair(const Value &car, const Value &cdr) : ValueBase(V_PAIR), car(car), cdr(cdr) {}
Value PairV(const Value &car, const Value &cdr) {
  return Value(new Pair(car, cdr));
}

Closure::Closure(const std::vector<std::string> &xs, const Expr &e, const Assoc &env)
  : ValueBase(V_PROC), parameters(xs), e(e), env(env) {}
Value ClosureV(const std::vector<std::string> &xs, const Expr &e, const Assoc &env) {
  return Value(new Closure(xs, e, env));
}
