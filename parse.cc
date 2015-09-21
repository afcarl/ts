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
             scan::TypeToString(cur_.type) + " at position " +
             scanner_->Pos().ToString());
  scan::Token ret = cur_;
  cur_ = scanner_->Next();
  return ret;
}

bool Parser::HasParameter() {
  return cur_.type == scan::IDENT;
}

std::unique_ptr<ast::Parameter> Parser::Parameter() {
  std::unique_ptr<ast::Parameter> parameter(new ast::Parameter);
  parameter->type = types::TypeFromString(Eat(scan::IDENT).cargo);
  parameter->name = Eat(scan::IDENT).cargo;
  return parameter;
}

std::unique_ptr<ast::Block> Parser::Block() {
  Eat(scan::LBRACE);
  std::unique_ptr<ast::Block> block(new ast::Block);
  while (cur_.type != scan::RBRACE) {
    block->statements.push_back(Statement());
  }
  Eat(scan::RBRACE);
  return block;
}

std::unique_ptr<ast::ReturnStatement> Parser::ReturnStatement() {
  std::unique_ptr<ast::ReturnStatement> return_statement(
      new ast::ReturnStatement);
  Eat(scan::RETURN);
  return_statement->return_value = Expression();
  Eat(scan::SEMI);
  return return_statement;
}

std::unique_ptr<ast::IfStatement> Parser::IfStatement() {
  std::unique_ptr<ast::IfStatement> if_statement(new ast::IfStatement);
  Eat(scan::IF);
  Eat(scan::LPAREN);
  if_statement->antecedent = Expression();
  Eat(scan::RPAREN);
  if_statement->consequent = Block();
  return if_statement;
}

std::unique_ptr<ast::ExpressionStatement> Parser::ExpressionStatement() {
  std::unique_ptr<ast::ExpressionStatement> expression_statement(
      new ast::ExpressionStatement);
  expression_statement->expression = Expression();
  Eat(scan::SEMI);
  return expression_statement;
}

std::unique_ptr<ast::Statement> Parser::Statement() {
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

std::unique_ptr<ast::Expression> Parser::PrimaryExpression() {
  if (cur_.type == scan::LPAREN) {
    Eat(scan::LPAREN);
    auto expr = Expression();
    Eat(scan::RPAREN);
    return expr;
  }
  if (cur_.type == scan::NUMBER) {
    std::unique_ptr<ast::IntExpression> expr(new ast::IntExpression);
    expr->integer = Eat(scan::NUMBER).cargo;
    return std::unique_ptr<ast::Expression>(expr.release());
  }
  std::string ident = Eat(scan::IDENT).cargo;
  if (cur_.type == scan::LPAREN) {
    Eat(scan::LPAREN);
    std::unique_ptr<ast::FunctionCall> function_call(new ast::FunctionCall);
    function_call->function_name = ident;
    while (cur_.type != scan::RPAREN) {
      function_call->arguments.push_back(Expression());
    }
    Eat(scan::RPAREN);
    return std::unique_ptr<ast::Expression>(function_call.release());
  }
  std::unique_ptr<ast::IdentExpression> expr(new ast::IdentExpression);
  expr->ident = ident;
  return std::unique_ptr<ast::Expression>(expr.release());
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

// Operator-precedence method.
// See: https://en.wikipedia.org/wiki/Operator-precedence_parser#Precedence_climbing_method
std::unique_ptr<ast::Expression> Parser::Expression(
    std::unique_ptr<ast::Expression> lhs,
    int min_precedence) {
  while (IsOp() && OpPrecedence(cur_.cargo) >= min_precedence) {
    std::string op = Eat(scan::OP).cargo;
    std::unique_ptr<ast::Expression> rhs = PrimaryExpression();
    while (IsOp() && OpPrecedence(cur_.cargo) > OpPrecedence(op)) {
      rhs = Expression(std::move(rhs), OpPrecedence(cur_.cargo));
    }
    std::unique_ptr<ast::BinaryExpression> new_lhs(new ast::BinaryExpression);
    new_lhs->left = std::move(lhs);
    new_lhs->right = std::move(rhs);
    new_lhs->op = op;
    lhs = std::move(new_lhs);
  }
  return lhs;
}

std::unique_ptr<ast::Expression> Parser::Expression() {
  return Expression(PrimaryExpression(), 0);
}

std::unique_ptr<ast::Function> Parser::Function() {
  std::unique_ptr<ast::Function> function(new ast::Function);
  function->return_type = types::TypeFromString(Eat(scan::IDENT).cargo);
  function->name = Eat(scan::IDENT).cargo;
  Eat(scan::LPAREN);
  while (HasParameter()) {
    function->parameters.push_back(Parameter());
  }
  Eat(scan::RPAREN);
  function->body = Block();
  return function;
}

std::unique_ptr<ast::Program> Parser::Program() {
  std::unique_ptr<ast::Program> program(new ast::Program);
  while (HasFunction()) {
    program->functions.push_back(Function());
  }
  return program;
}

}  // namespace parse
}  // namespace ts
