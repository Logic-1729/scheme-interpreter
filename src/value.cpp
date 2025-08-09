/**
 * @file value.cpp
 * @brief Implementation of value types and environment operations
 * 
 * This file implements all value types, their constructors, show methods,
 * and environment (association list) operations for the Scheme interpreter.
 */

#include "value.hpp"

// ============================================================================
// Base ValueBase Implementation
// ============================================================================

ValueBase::ValueBase(ValueType vt) : v_type(vt) {}

void ValueBase::showCdr(std::ostream &os) {
    os << " . ";
    show(os);
    os << ')';
}

// ============================================================================
// Value Smart Pointer Implementation
// ============================================================================

Value::Value(ValueBase *ptr) : ptr(ptr) {}

ValueBase* Value::operator->() const { 
    return ptr.get(); 
}

ValueBase& Value::operator*() { 
    return *ptr; 
}

ValueBase* Value::get() const { 
    return ptr.get(); 
}

void Value::show(std::ostream &os) {
    ptr->show(os);
}

// ============================================================================
// Environment (Association List) Implementation
// ============================================================================

AssocList::AssocList(const std::string &x, const Value &v, Assoc &next)
    : x(x), v(v), next(next) {}

Assoc::Assoc(AssocList *x) : ptr(x) {}

AssocList* Assoc::operator->() const { 
    return ptr.get(); 
}

AssocList& Assoc::operator*() { 
    return *ptr; 
}

AssocList* Assoc::get() const { 
    return ptr.get(); 
}

Assoc empty() {
    return Assoc(nullptr);
}

Assoc extend(const std::string &x, const Value &v, Assoc &lst) {
    return Assoc(new AssocList(x, v, lst));
}

void modify(const std::string &x, const Value &v, Assoc &lst) {
    for (auto i = lst; i.get() != nullptr; i = i->next) {
        if (x == i->x) {
            i->v = v;
            return;
        }
    }
}

Value find(const std::string &x, Assoc &l) {
    for (auto i = l; i.get() != nullptr; i = i->next) {
        if (x == i->x) {
            return i->v;
        }
    }
    return Value(nullptr);
}

// ============================================================================
// Simple Value Types Implementation
// ============================================================================

// Void
Void::Void() : ValueBase(V_VOID) {}

void Void::show(std::ostream &os) {
    os << "#<void>";
}

Value VoidV() {
    return Value(new Void());
}

// Integer
Integer::Integer(int n) : ValueBase(V_INT), n(n) {}

void Integer::show(std::ostream &os) {
    os << n;
}

Value IntegerV(int n) {
    return Value(new Integer(n));
}

// Rational
// Helper function to calculate greatest common divisor
static int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

Rational::Rational(int num, int den) : ValueBase(V_RATIONAL) {
    if (den == 0) {
        throw std::runtime_error("Division by zero");
    }
    
    // Simplify the fraction
    int g = gcd(num, den);
    numerator = num / g;
    denominator = den / g;
    
    // Ensure denominator is positive
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
}

void Rational::show(std::ostream &os) {
    if (denominator == 1) {
        os << numerator;
    } else {
        os << numerator << "/" << denominator;
    }
}

Value RationalV(int num, int den) {
    return Value(new Rational(num, den));
}

// Boolean
Boolean::Boolean(bool b) : ValueBase(V_BOOL), b(b) {}

void Boolean::show(std::ostream &os) {
    os << (b ? "#t" : "#f");
}

Value BooleanV(bool b) {
    return Value(new Boolean(b));
}

// Symbol
Symbol::Symbol(const std::string &s) : ValueBase(V_SYM), s(s) {}

void Symbol::show(std::ostream &os) {
    os << s;
}

Value SymbolV(const std::string &s) {
    return Value(new Symbol(s));
}

// String
String::String(const std::string &s) : ValueBase(V_STRING), s(s) {}

void String::show(std::ostream &os) {
    os << "\"" << s << "\"";
}

Value StringV(const std::string &s) {
    return Value(new String(s));
}

// ============================================================================
// Special Value Types Implementation
// ============================================================================

// Null
Null::Null() : ValueBase(V_NULL) {}

void Null::show(std::ostream &os) {
    os << "()";
}

void Null::showCdr(std::ostream &os) {
    os << ')';
}

Value NullV() {
    return Value(new Null());
}

// Terminate
Terminate::Terminate() : ValueBase(V_TERMINATE) {}

void Terminate::show(std::ostream &os) {
    os << "()";
}

Value TerminateV() {
    return Value(new Terminate());
}

// ============================================================================
// Composite Value Types Implementation
// ============================================================================

// Pair
Pair::Pair(const Value &car, const Value &cdr) 
    : ValueBase(V_PAIR), car(car), cdr(cdr) {}

void Pair::show(std::ostream &os) {
    os << '(' << car;
    cdr->showCdr(os);
}

void Pair::showCdr(std::ostream &os) {
    os << ' ' << car;
    cdr->showCdr(os);
}

Value PairV(const Value &car, const Value &cdr) {
    return Value(new Pair(car, cdr));
}

// Procedure
Procedure::Procedure(const std::vector<std::string> &xs, const Expr &e, const Assoc &env)
    : ValueBase(V_PROC), parameters(xs), e(e), env(env) {}

void Procedure::show(std::ostream &os) {
    os << "#<procedure>";
}

Value ProcedureV(const std::vector<std::string> &xs, const Expr &e, const Assoc &env) {
    return Value(new Procedure(xs, e, env));
}

// ============================================================================
// Utility Functions Implementation
// ============================================================================

std::ostream &operator<<(std::ostream &os, Value &v) {
    v->show(os);
    return os;
}
