#include "print.h"

#include <iostream>

using std::cout;
using std::endl;

namespace ts {
namespace print {

void PrintVisitor::Visit(const ast::Program& node) {
  cout << "(program";
  for (const auto& func : node.functions) {
    cout << " ";
    func->Accept(this);
  }
  cout << ")";
}

void PrintVisitor::Visit(const ast::Function& node) {
  cout << "(function ";
  cout << "(return_type '" << node.return_type << "') ";
  cout << "(name '" << node.name << "') ";
  cout << "(parameters";
  for (const auto& param : node.parameters) {
    cout << " ";
    param->Accept(this);
  }
  cout << ") ";
  cout << "(body ";
  node.body->Accept(this);
  cout << ")";
  cout << ")";
}

void PrintVisitor::Visit(const ast::Parameter& node) {
  cout << "(parameter (type " << node.type << ") (name " << node.name << "))";
}

void PrintVisitor::Visit(const ast::IfStatement& node) {
  cout << "(if (antecedent ";
  node.antecedent->Accept(this);
  cout << ") (consequent ";
  node.consequent->Accept(this);
  cout << "))";
}

void PrintVisitor::Visit(const ast::Block& node) {
  cout << "(block";
  for (const auto& stmt : node.statements) {
    cout << " ";
    stmt->Accept(this);
  }
  cout << ")";
}

void PrintVisitor::Visit(const ast::ReturnStatement& node) {
  cout << "(return ";
  node.return_value->Accept(this);
  cout << ")";
}

void PrintVisitor::Visit(const ast::ExpressionStatement& node) {
  node.expression->Accept(this);
}

void PrintVisitor::Visit(const ast::FunctionCall& node) {
  cout << "(function_call (name '" << node.function_name << "') (arguments";
  for (const auto& arg : node.arguments) {
    cout << " ";
    arg->Accept(this);
  }
  cout << ")";
  cout << ")";
}

void PrintVisitor::Visit(const ast::IdentExpression& node) {
  cout << node.ident;
}

void PrintVisitor::Visit(const ast::IntExpression& node) {
  cout << node.integer;
}

void PrintVisitor::Visit(const ast::BinaryExpression& node) {
  cout << "(binary_expression (op " << node.op << ") (left ";
  node.left->Accept(this);
  cout << ") (right ";
  node.right->Accept(this);
  cout << "))";
}

}  // namespace print
}  // namespace ts
