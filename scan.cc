#include "scan.h"

#include <algorithm>

#include "util.h"

namespace ts {
namespace scan {

namespace {

// Keep this in the same order as the Type enum definition.
static const char* const kTypeStrings[] = {
  "",
  "return",
  "int",
  "void",
  "if",
  "ident",
  "lparen",
  "rparen",
  "lbrace",
  "rbrace",
  "lt",
  "minus",
  "asterisk",
  "semi",
  "number",
  "fin",
};

Type GetTypeForIdent(const std::string& ident) {
  static const auto keyword_types = {RETURN, INT, VOID, IF};
  auto pos = std::find_if(
      keyword_types.begin(), keyword_types.end(), [&ident](Type type)->bool {
        return ident == kTypeStrings[type];
      });
  return pos == keyword_types.end() ? IDENT : *pos;
}

}  // namespace

std::string TypeToString(Type type) {
  return kTypeStrings[type];
}

Scanner::Scanner(const std::string& text) : text_(text), pos_(0) {
}

bool Scanner::HasNext() const {
  return pos_ < text_.size();
}

bool Scanner::IsSpace() const {
  if (pos_ >= text_.size()) {
    return false;
  }
  switch (text_[pos_]) {
  case ' ': case '\t': case '\n':
    return true;
  default:
    return false;
  }
}

char Scanner::Eat() {
  return text_[pos_++];
}

char Scanner::Peek() const {
  return text_[pos_];
}

bool Scanner::IsSymbol() const {
  char c = Peek();
  switch (c) {
  case '(':
  case ')':
  case ';':
  case '<':
  case '-':
  case '*':
  case '{':
  case '}':
    return true;
  default:
    return false;
  }
}

Token Scanner::EatSymbol() {
  char c = Eat();
  std::string sym(1, c);
  switch (c) {
  case '(':
    return {LPAREN, sym};
  case ')':
    return {RPAREN, sym};
  case ';':
    return {SEMI, sym};
  case '<':
    return {LT, sym};
  case '-':
    return {MINUS, sym};
  case '*':
    return {ASTERISK, sym};
  case '{':
    return {LBRACE, sym};
  case '}':
    return {RBRACE, sym};
  default:
    Die("Failed to eat a symbol");
    return {};
  }
}

bool Scanner::IsIdent() const {
  char c = Peek();
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

Token Scanner::EatIdent() {
  std::string ident;
  if (!IsIdent()) {
    Die("Failed to eat an identifier");
  }
  while (IsIdent()) {
    ident += Eat();
  }
  return {GetTypeForIdent(ident), ident};
}

bool Scanner::IsNumber() const {
  char c = Peek();
  return c >= '0' && c <= '9';
}

Token Scanner::EatNumber() {
  std::string number;
  if (!IsNumber()) {
    Die("Failed to eat a number");
  }
  while (IsNumber()) {
    number += Eat();
  }
  return {NUMBER, number};
}

Token Scanner::Next() {
  while (IsSpace()) {
    Eat();
  }
  if (pos_ >= text_.size()) {
    return {FIN, ""};
  }
  char c = Peek();
  if (IsSymbol()) {
    return EatSymbol();
  } else if (IsIdent()) {
    return EatIdent();
  } else if (IsNumber()) {
    return EatNumber();
  }
  Die("Unknown token: " + std::string(1, c));
  return {};
}

}  // namespace scan
}  // namespace ts
