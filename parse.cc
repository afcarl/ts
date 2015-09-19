#include "parse.h"

#include <map>

#include "util.h"

using ts::Assert;

namespace ts {
namespace parse {

Parser::Parser(std::unique_ptr<scan::Scanner> scanner)
    : scanner_(std::move(scanner)) {
  cur_ = scanner_->Next();
}

bool Parser::HasFunction() {
  return cur_.type == scan::IDENT;
}

scan::Token Parser::Eat(scan::Type token_type) {
  Assert(cur_.type == token_type,
         "Expected " + scan::TypeToString(token_type) + ", found " +
             scan::TypeToString(cur_.type) + " at index " +
             std::to_string(scanner_->Pos()));
  scan::Token ret = cur_;
  cur_ = scanner_->Next();
  return ret;
}

bool Parser::HasParameter() {
  return cur_.type == scan::IDENT;
}

ast::Parameter Parser::Parameter() {
  ast::Parameter parameter;
  parameter.type = Eat(scan::IDENT).cargo;
  parameter.name = Eat(scan::IDENT).cargo;
  return parameter;
}

ast::Block Parser::Block() {
  Eat(scan::LBRACE);
  ast::Block block;
  while (cur_.type != scan::RBRACE) {
    block.statements.push_back(Statement());
  }
  Eat(scan::RBRACE);
  return block;
}

ast::ReturnStatement Parser::ReturnStatement() {
  ast::ReturnStatement return_statement;
  Eat(scan::RETURN);
  return_statement.return_value = Expression();
  Eat(scan::SEMI);
  return return_statement;
}

ast::IfStatement Parser::IfStatement() {
  ast::IfStatement if_statement;
  Eat(scan::IF);
  Eat(scan::LPAREN);
  if_statement.antecedent = Expression();
  Eat(scan::RPAREN);
  if_statement.consequent = Block();
  return if_statement;
}

ast::ExpressionStatement Parser::ExpressionStatement() {
  ast::ExpressionStatement expression_statement;
  expression_statement.expression = Expression();
  Eat(scan::SEMI);
  return expression_statement;
}

ast::Statement Parser::Statement() {
  switch (cur_.type) {
  case scan::RETURN:
    return ReturnStatement();
  case scan::IF:
    return IfStatement();
  case scan::LBRACE:
    return Block();
  default:
    return ExpressionStatement();
  }
}

ast::Expression Parser::PrimaryExpression() {
  if (cur_.type == scan::LPAREN) {
    Eat(scan::LPAREN);
    ast::Expression expr = Expression();
    Eat(scan::RPAREN);
    return expr;
  }
  if (cur_.type == scan::NUMBER) {
    ast::IntExpression expr;
    expr.integer = Eat(scan::NUMBER).cargo;
    return expr;
  }
  std::string ident = Eat(scan::IDENT).cargo;
  if (cur_.type == scan::LPAREN) {
    Eat(scan::LPAREN);
    ast::FunctionCall function_call;
    function_call.function_name = ident;
    while (cur_.type != scan::RPAREN) {
      function_call.arguments.push_back(Expression());
    }
    return function_call;
  }
  ast::IdentExpression expr;
  expr.ident = ident;
  return expr;
}

bool Parser::IsOp() {
  return cur_.type == scan::OP;
}

int Parser::OpPrecedence(const std::string& op) {
  static const std::map<std::string, int> kPrecedence {
    {"<", 0},
    {"-", 1},
    {"*", 2},
  };
  auto pos = kPrecedence.find(op);
  Assert(pos != kPrecedence.end(), op + " is not an operator");
  return pos->second;
}

ast::Expression Parser::Expression(const ast::Expression& lhs,
                                   int min_precedence) {
/*  if (!IsOp()) {
    return lhs;
  }
  ast::BinaryExpression 
  while (IsOp() && OpPrecedence(cur_.cargo) >= min_precedence) {
    std::string op = Eat(scan::OP).cargo;
    ast::Expression rhs = PrimaryExpression();
    while (IsOp() && OpPrecedence(cur_.cargo) > OperatorPrecedence(op)) {
      rhs = Expression(rhs, OpPrecedence(cur_.cargo));
    }
    lhs.left = lhs;
    lhs.right = rhs;
    lhs.op = op;
  }
  return lhs;
  */
  return {};
}

// Operator-precedence method.
// See: https://en.wikipedia.org/wiki/Operator-precedence_parser#Precedence_climbing_method
ast::Expression Parser::Expression() {
  return Expression(PrimaryExpression(), 0);
}

ast::Function Parser::Function() {
  ast::Function function;
  function.return_type = Eat(scan::IDENT).cargo;
  function.name = Eat(scan::IDENT).cargo;
  Eat(scan::LPAREN);
  while (HasParameter()) {
    function.parameters.push_back(Parameter());
  }
  Eat(scan::RPAREN);
  function.body = Block();
  return function;
}

ast::Program Parser::Program() {
  ast::Program program;
  while (HasFunction()) {
    program.functions.push_back(Function());
  }
  return program;
}

}  // namespace parse
}  // namespace ts
