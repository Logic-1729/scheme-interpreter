#include "syntax.hpp"
#include <cstring>
#include <vector>

Syntax :: Syntax(SyntaxBase *stx) : ptr(stx) {}
SyntaxBase* Syntax :: operator -> () const { return ptr.get(); }
SyntaxBase& Syntax :: operator * () { return *ptr; }
SyntaxBase* Syntax :: get() const { return ptr.get(); }

Number :: Number(int n) : n(n) {}
void Number::show(std::ostream &os) {
  os << "the-number-" << n;
}

void TrueSyntax::show(std::ostream &os) {
  os << "#t";
}

void FalseSyntax::show(std::ostream &os) {
  os << "#f";
}

Identifier :: Identifier(const std :: string &s1) : s(s1) {}
void Identifier::show(std::ostream &os) {
  os << s;
}

List :: List() {}
void List::show(std::ostream &os) {
  os << '(';
  for (auto stx : stxs) {
    stx -> show(os);
    os << ' ';
  }
  os << ')';
}

std::istream &readSpace(std::istream &is) {
  while (isspace(is.peek()))
    is.get();
  return is;
}

Syntax readList(std::istream &is);

// no leading space
Syntax readItem(std::istream &is) {
  if (is.peek() == '(' || is.peek() == '[') {
    is.get();
    return readList(is);
  }
  if (is.peek() == '\'')
  {
    is.get();
    return readList(is);
  }
  std::string s;
  do {
    int c = is.peek();
    if (c == '(' || c == ')' ||
        c == '[' || c == ']' || 
        isspace(c) ||
        c == EOF)
      break;
    is.get();
    s.push_back(c);
  } while (true);
  // try parsing a integer
  bool neg = false;
  int n = 0;
  int i = 0;
  if (s.size() == 1 && (s[0] == '+' || s[0] == '-'))
    goto identifier;
  if (s[0] == '-') {
    i += 1;
    neg = true;
  } else if (s[0] == '+')
    i += 1;
  for (; i < s.size(); i++)
    if ('0' <= s[i] && s[i] <= '9')
      n = n * 10 + s[i] - '0';
    else
      goto identifier;
  if (neg)
    n = -n;
  return Syntax(new Number(n));
 identifier:
  // not a number
  if (s == "#t")
    return Syntax(new TrueSyntax());
  if (s == "#f")
    return Syntax(new FalseSyntax());
  return Syntax(new Identifier(s));
}

Syntax readList(std::istream &is) {
    List *stx = new List();
    while (readSpace(is).peek() != ')' && readSpace(is).peek() != ']')
        stx -> stxs.push_back(readItem(is));
    is.get(); // ')'
    return Syntax(stx);
}

Syntax readSyntax(std::istream &is) {
  return readItem(readSpace(is));
}

std::istream &operator>>(std::istream &is, Syntax &stx) {
  stx = readSyntax(is);
  return is;
}
