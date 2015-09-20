#ifndef AST_H_
#define AST_H_

#include <ostream>
#include <vector>

#include "scan.h"

namespace ts {
namespace ast {

class Visitor;

class Node {
 public:
  virtual void Accept(Visitor* visitor) const = 0;
};

// <Statement> ::= <ExpressionStatement> | <ReturnStatement> | <IfStatement>
//               | <Block>;
class Statement : public Node {};

// <Expression> ::= <LogicalExpression>;
class Expression : public Node {};

class BinaryExpression : public Expression {
 public:
  void Accept(Visitor* visitor) const override;
  std::string op;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

// <IntExpression> ::= NUMBER;
class IntExpression : public Expression {
 public:
  void Accept(Visitor* visitor) const override;
  std::string integer;
};

// <IdentExpression> ::= IDENT;
class IdentExpression : public Expression {
 public:
  void Accept(Visitor* visitor) const override;
  std::string ident;
};

// <FunctionCall> ::= <Identifier> '(' <Expression>* ')';
class FunctionCall : public Expression {
 public:
  void Accept(Visitor* visitor) const override;
  std::string function_name;
  std::vector<std::unique_ptr<Expression>> arguments;
};

// <ExpressionStatement> ::= <Expression> ';';
class ExpressionStatement : public Statement {
 public:
  void Accept(Visitor* visitor) const override;
  std::unique_ptr<Expression> expression;
};

// <ReturnStatement> ::= 'return' <Expression> ';';
class ReturnStatement : public Statement {
 public:
  void Accept(Visitor* visitor) const override;
  std::unique_ptr<Expression> return_value;
};

// <Block> ::= '{' <Statement>* '}';
class Block : public Statement {
 public:
  void Accept(Visitor* visitor) const override;
  std::vector<std::unique_ptr<Statement>> statements;
};

// <IfStatement> ::= 'if' '(' <Expression> ')' <Block>;
class IfStatement : public Statement {
 public:
  void Accept(Visitor* visitor) const override;
  std::unique_ptr<Expression> antecedent;
  std::unique_ptr<Block> consequent;
};

// <Parameter> ::= <Identifier> <Identifier>;
class Parameter : public Node {
 public:
  void Accept(Visitor* visitor) const override;
  std::string type;
  std::string name;
};

// <Function> ::=
// <Identifier> <Identifier> '(' (<Parameter> (',' <Parameter>)*)? ')' <Block>;
class Function : public Node {
 public:
  void Accept(Visitor* visitor) const override;
  std::string return_type;
  std::string name;
  std::vector<std::unique_ptr<Parameter>> parameters;
  std::unique_ptr<Block> body;
};

// <Program> ::= <Function>*;
class Program : public Node {
 public:
  void Accept(Visitor* visitor) const override;
  std::vector<std::unique_ptr<Function>> functions;
};

class Visitor {
 public:
  virtual void Visit(const ast::Program& node);
  virtual void Visit(const ast::Function& node);
  virtual void Visit(const ast::Parameter& node);
  virtual void Visit(const ast::IfStatement& node);
  virtual void Visit(const ast::Block& node);
  virtual void Visit(const ast::ReturnStatement& node);
  virtual void Visit(const ast::ExpressionStatement& node);
  virtual void Visit(const ast::FunctionCall& node);
  virtual void Visit(const ast::IdentExpression& node);
  virtual void Visit(const ast::IntExpression& node);
  virtual void Visit(const ast::BinaryExpression& node);
  virtual void Visit(const ast::Expression& node);
  virtual void Visit(const ast::Statement& node);
  virtual void Visit(const ast::Node& node);
};

}  // namespace ast
}  // namespace ts

#endif  // AST_H_
