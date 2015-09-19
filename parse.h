#ifndef PARSE_H_
#define PARSE_H_

#include <string>
#include <vector>

#include "ast.h"
#include "scan.h"

namespace ts {
namespace parse {

class Parser {
 public:
  Parser(std::unique_ptr<scan::Scanner> scanner);
  ast::Program Program();

 private:
  scan::Token Eat(scan::Type token_type);
  ast::Statement Statement();
  ast::IfStatement IfStatement();
  ast::ReturnStatement ReturnStatement();
  ast::Block Block();
  ast::Function Function();
  ast::Expression Expression();
  ast::Expression Expression(const ast::Expression& lhs, int min_precedence);
  ast::Parameter Parameter();
  ast::ExpressionStatement ExpressionStatement();
  ast::Expression PrimaryExpression();

  bool HasFunction();
  bool HasParameter();
  bool IsOp();
  int OpPrecedence(const std::string& op);

  std::unique_ptr<scan::Scanner> scanner_;
  scan::Token cur_;
};

}  // namespace parse
}  // namespace ts

#endif  // PARSE_H_
