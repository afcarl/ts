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
  std::unique_ptr<ast::Program> Program();

 private:
  scan::Token Eat(scan::Type token_type);
  std::unique_ptr<ast::Statement> Statement();
  std::unique_ptr<ast::IfStatement> IfStatement();
  std::unique_ptr<ast::ReturnStatement> ReturnStatement();
  std::unique_ptr<ast::Block> Block();
  std::unique_ptr<ast::Function> Function();
  std::unique_ptr<ast::Expression> Expression();
  std::unique_ptr<ast::Expression> Expression(
      std::unique_ptr<ast::Expression> lhs,
      int min_precedence);
  std::unique_ptr<ast::Parameter> Parameter();
  std::unique_ptr<ast::ExpressionStatement> ExpressionStatement();
  std::unique_ptr<ast::Expression> PrimaryExpression();

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
