#ifndef SCAN_H_
#define SCAN_H_

#include <ostream>
#include <string>

namespace ts {
namespace scan {

enum Type {
  RETURN = 1,
  IF,
  IDENT,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  OP,
  SEMI,
  NUMBER,
  FIN
};

std::string TypeToString(Type type);

struct Token {
  Type type;
  std::string cargo;
};

struct Position {
  int row;
  int col;
  std::string ToString();
};

class Scanner {
 public:
  Scanner(const std::string& text);
  bool HasNext() const;
  Token Next();
  Position Pos() const;

 private:
  char Peek() const;
  char Eat();
  bool IsSymbol() const;
  Token EatSymbol();
  bool IsIdent() const;
  Token EatIdent();
  bool IsNumber() const;
  Token EatNumber();
  bool IsSpace() const;

  std::string text_;
  int pos_;
};

}  // namespace scan
}  // namespace ts

#endif  // SCAN_H_
