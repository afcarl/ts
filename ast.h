#ifndef AST_H_
#define AST_H_

#include <ostream>
#include <vector>

#include "scan.h"

namespace ts {
namespace ast {

// <Statement> ::= <ExpressionStatement> | <ReturnStatement> | <IfStatement>
//               | <Block>;
class Statement {
};

// <Expression> ::= <LogicalExpression>;
class Expression {
};

class BinaryExpression : public Expression {
 public:
  std::string op;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

// <IntExpression> ::= NUMBER;
class IntExpression : public Expression {
 public:
  std::string integer;
};

// <IdentExpression> ::= IDENT;
class IdentExpression : public Expression {
 public:
  std::string ident;
};

// <FunctionCall> ::= <Identifier> '(' <Expression>* ')';
class FunctionCall : public Expression {
 public:
  std::string function_name;
  std::vector<std::unique_ptr<Expression>> arguments;
};

// <ExpressionStatement> ::= <Expression> ';';
class ExpressionStatement : public Statement {
 public:
   std::unique_ptr<Expression> expression;
};

// <ReturnStatement> ::= 'return' <Expression> ';';
class ReturnStatement : public Statement {
 public:
   std::unique_ptr<Expression> return_value;
};

// <Block> ::= '{' <Statement>* '}';
class Block : public Statement {
 public:
  std::vector<std::unique_ptr<Statement>> statements;
};

// <IfStatement> ::= 'if' '(' <Expression> ')' <Block>;
class IfStatement : public Statement {
 public:
   std::unique_ptr<Expression> antecedent;
   std::unique_ptr<Block> consequent;
};

// <Parameter> ::= <Identifier> <Identifier>;
class Parameter {
 public:
  std::string type;
  std::string name;
};

// <Function> ::=
// <Identifier> <Identifier> '(' (<Parameter> (',' <Parameter>)*)? ')' <Block>;
class Function {
 public:
  std::string return_type;
  std::string name;
  std::vector<std::unique_ptr<Parameter>> parameters;
  std::unique_ptr<Block> body;
};

// <Program> ::= <Function>*;
class Program {
 public:
  std::vector<std::unique_ptr<Function>> functions;
};

std::ostream& operator<<(std::ostream&, const Program&);
std::ostream& operator<<(std::ostream&, const Function&);
std::ostream& operator<<(std::ostream&, const Parameter&);
std::ostream& operator<<(std::ostream&, const Statement&);
std::ostream& operator<<(std::ostream&, const IfStatement&);
std::ostream& operator<<(std::ostream&, const ReturnStatement&);
std::ostream& operator<<(std::ostream&, const Block&);
std::ostream& operator<<(std::ostream&, const Expression&);
std::ostream& operator<<(std::ostream&, const BinaryExpression&);
std::ostream& operator<<(std::ostream&, const IntExpression&);
std::ostream& operator<<(std::ostream&, const IdentExpression&);
std::ostream& operator<<(std::ostream&, const FunctionCall&);

}  // namespace ast
}  // namespace ts

#endif  // AST_H_
