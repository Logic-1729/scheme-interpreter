#include "syntax.hpp"
#include <cstring>
#include <vector>

Syntax::Syntax(SyntaxBase *stx) : ptr(stx) {}
SyntaxBase* Syntax::operator->() const { return ptr.get(); }
SyntaxBase& Syntax::operator*() { return *ptr; }
SyntaxBase* Syntax::get() const { return ptr.get(); }

Number::Number(int n) : n(n) {}
void Number::show(std::ostream &os) {
  os << "the-number-" << n;
}

void TrueSyntax::show(std::ostream &os) {
  os << "#t";
}

void FalseSyntax::show(std::ostream &os) {
  os << "#f";
}

SymbolSyntax::SymbolSyntax(const std::string &s1) : s(s1) {}
void SymbolSyntax::show(std::ostream &os) {
    os << s;
}

StringSyntax::StringSyntax(const std::string &s1) : s(s1) {}
void StringSyntax::show(std::ostream &os) {
    os << "\"" << s << "\"";
}

List::List() {}
void List::show(std::ostream &os) {
    os << '(';
    for (auto stx : stxs) {
        stx->show(os);
        os << ' ';
    }
    os << ')';
}

std::istream &readSpace(std::istream &is) {
  while (true) {
    // 跳过空白字符
    while (isspace(is.peek()))
      is.get();
    
    // 检查是否是注释
    if (is.peek() == ';') {
      // 跳过注释直到行末
      while (is.peek() != '\n' && is.peek() != EOF)
        is.get();
      // 继续循环以跳过注释后的空白字符
    } else {
      // 没有更多空白字符或注释，退出循环
      break;
    }
  }
  return is;
}

Syntax readList(std::istream &is);

// Helper function to try parsing as integer
bool tryParseNumber(const std::string &s, int &result) {
  bool neg = false;
  int n = 0;
  int i = 0;
  
  // Single '+' or '-' are not numbers
  if (s.size() == 1 && (s[0] == '+' || s[0] == '-'))
    return false;
  
  // Handle sign
  if (s[0] == '-') {
    i += 1;
    neg = true;
  } else if (s[0] == '+') {
    i += 1;
  }
  
  // Check if all remaining characters are digits
  for (; i < s.size(); i++) {
    if ('0' <= s[i] && s[i] <= '9') {
      n = n * 10 + s[i] - '0';
    } else {
      return false;  // Not a valid number
    }
  }
  
  result = neg ? -n : n;
  return true;
}

// Helper function to create identifier/symbol syntax
Syntax createIdentifierSyntax(const std::string &s) {
  if (s == "#t")
    return Syntax(new TrueSyntax());
  if (s == "#f")
    return Syntax(new FalseSyntax());
  return Syntax(new SymbolSyntax(s));
}

// no leading space
Syntax readItem(std::istream &is) {
  if (is.peek() == '(' || is.peek() == '[') {
    is.get();
    return readList(is);
  }
  if (is.peek() == '\'')
  {
    is.get();
    // 读取单引号后的语法元素
    Syntax quoted_syntax = readItem(is);
    
    // 创建 (quote <syntax>) 的列表结构
    List *quote_list = new List();
    quote_list->stxs.push_back(Syntax(new SymbolSyntax("quote")));
    quote_list->stxs.push_back(quoted_syntax);
    
    return Syntax(quote_list);
  }
  // 处理字符串字面量
  if (is.peek() == '"') {
    is.get(); // 消费开始的双引号
    std::string str;
    while (is.peek() != '"' && is.peek() != EOF) {
      char c = is.get();
      if (c == '\\') {
        // 处理转义字符
        char next = is.get();
        switch (next) {
          case 'n': str.push_back('\n'); break;
          case 't': str.push_back('\t'); break;
          case 'r': str.push_back('\r'); break;
          case '\\': str.push_back('\\'); break;
          case '"': str.push_back('"'); break;
          default: str.push_back(next); break;
        }
      } else {
        str.push_back(c);
      }
    }
    if (is.peek() == '"') {
      is.get(); // 消费结束的双引号
    }
    return Syntax(new StringSyntax(str));
  }
  
  // Read token
  std::string s;
  do {
    int c = is.peek();
    if (c == '(' || c == ')' ||
        c == '[' || c == ']' || 
        c == ';' ||  // 添加分号作为分隔符
        isspace(c) ||
        c == EOF)
      break;
    is.get();
    s.push_back(c);
  } while (true);
  
  // Try parsing as integer
  int number_value;
  if (tryParseNumber(s, number_value)) {
    return Syntax(new Number(number_value));
  }
  
  // Not a number, treat as identifier/symbol
  return createIdentifierSyntax(s);
}

Syntax readList(std::istream &is) {
    List *stx = new List();
    while (readSpace(is).peek() != ')' && readSpace(is).peek() != ')')
        stx->stxs.push_back(readItem(is));
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
