#ifndef AST_H_
#define AST_H_

#include <vector>

namespace ts {

// <Statement> ::= <Expression> ';' | <ReturnStatement> | <IfStatement>
//               | <Block>;
class Statement {
};

// <Expression> ::= <Identifier> | <MultExpression> | <SubExpression>
//                | <IntExpression> | <FunctionCall>;
class Expression : public Statement {
};

// <Identifier> ::= [a-zA-Z_]+;
class Identifier : public Expression {
  std::string ident;
};

// <MultExpression> ::= <Expression> '*' <Expression>;
class MultExpression : public Expression {
  Expression left;
  Expression right;
};

// <SubExpression> ::= <Expression> '-' <Expression>;
class SubExpression : public Expression {
  Expression left;
  Expression right;
};

// <IntExpression> ::= [0-9]+;
class IntExpression : public Expression {
  int integer;
};

// <FunctionCall> ::= <Identifier> '(' <Expression>* ')';
class FunctionCall : public Expression {
  Identifier function_name;
  std::vector<Expression> arguments;
};

// <ReturnStatement> ::= 'return' <Expression> ';';
class ReturnStatement : public Statement {
  Expression return_value;
};

// <Block> ::= '{' <Statement>* '}';
class Block : public Statement {
  std::vector<Statement> statements;
};

// <IfStatement> ::= 'if' '(' <Expression> ')' <Block>;
class IfStatement : public Statement {
  Expression antecedent;
  Block consequent;
};

// <Parameter> ::= <Identifier> <Identifier>;
class Parameter {
  Identifier type;
  Identifier name;
};

// <Function> ::=
// <Identifier> <Identifier> '(' (<Parameter> (',' <Parameter>)*)? ')' <Block>;
class Function {
  Identifier return_type;
  Identifier name;
  std::vector<Parameter> parameters;
  Block body;
};

// <Program> ::= <Function>*;
class Program {
  std::vector<Function> functions;
};

}  // namespace ts

#endif  // AST_H_
