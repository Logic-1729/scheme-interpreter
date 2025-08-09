#ifndef SYNTAX 
#define SYNTAX

#include <cstring>
#include <memory>
#include <vector>
#include "Def.hpp"

struct SyntaxBase {
    virtual Expr parse(Assoc &) = 0;
    virtual void show(std::ostream &) = 0;
    virtual ~SyntaxBase() = default;
};

struct Syntax {
    std::shared_ptr<SyntaxBase> ptr;
    Syntax(SyntaxBase *);
    SyntaxBase* operator->() const;
    SyntaxBase& operator*();
    SyntaxBase* get() const;
    Expr parse(Assoc &);
};

struct Number : SyntaxBase {
    int n;
    Number(int);
    virtual Expr parse(Assoc &) override;
    virtual void show(std::ostream &) override;
};

struct TrueSyntax : SyntaxBase {
    // This will not match
    virtual Expr parse(Assoc &) override;
    virtual void show(std::ostream &) override;
};

struct FalseSyntax : SyntaxBase {
    // FalseSyntax();
    virtual Expr parse(Assoc &) override;
    virtual void show(std::ostream &) override;
};

struct SymbolSyntax : SyntaxBase {
    std::string s;
    SymbolSyntax(const std::string &);
    virtual Expr parse(Assoc &) override;
    virtual void show(std::ostream &) override;
};

struct StringSyntax : SyntaxBase {
    std::string s;
    StringSyntax(const std::string &);
    virtual Expr parse(Assoc &) override;
    virtual void show(std::ostream &) override;
};

struct List : SyntaxBase {
    std::vector<Syntax> stxs;
    List();
    virtual Expr parse(Assoc &) override;
    virtual void show(std::ostream &) override;
};

Syntax readSyntax(std::istream &);

std::istream &operator>>(std::istream &, Syntax);
#endif