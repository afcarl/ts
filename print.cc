#include "print.h"

#include <iostream>

using std::cout;
using std::endl;

namespace ts {
namespace print {

void PrintVisitor::Visit(const ast::Program& node) {
  cout << "Program(" << endl;
  for (const auto& func : node.functions) {
    func->Accept(this);
  }
  cout << ")" << endl;
}

void PrintVisitor::Visit(const ast::Function& node) {
  cout << "Function(" << endl;
  cout << "return_type = " << node.return_type << endl;
  cout << "name = " << node.name << endl;
  cout << "parameters = (" << endl;
  for (const auto& param : node.parameters) {
    param->Accept(this);
  }
  cout << "body = " << endl;
  node.body->Accept(this);
  cout << ")" << endl;
  cout << ")" << endl;
}

void PrintVisitor::Visit(const ast::Parameter& node) {
  cout << "Parameter(";
  cout << "type = " << node.type << ", ";
  cout << "name = " << node.name << ")" << endl;
}

void PrintVisitor::Visit(const ast::IfStatement& node) {
  cout << "If(" << endl;
  cout << "antecedent = " << endl;
  node.antecedent->Accept(this);
  cout << "consequent = " << endl;
  node.consequent->Accept(this);
  cout << ")" << endl;
}

void PrintVisitor::Visit(const ast::Block& node) {
  cout << "Block(" << endl;
  for (const auto& stmt : node.statements) {
    stmt->Accept(this);
  }
  cout << ")" << endl;
}

void PrintVisitor::Visit(const ast::ReturnStatement& node) {
  cout << "ReturnStatement(" << endl;
  node.return_value->Accept(this);
  cout << ")" << endl;
}

void PrintVisitor::Visit(const ast::ExpressionStatement& node) {
  node.expression->Accept(this);
}

void PrintVisitor::Visit(const ast::FunctionCall& node) {
  cout << "FunctionCall" << endl;
  cout << "name = " << node.function_name << endl;
  cout << "arguments = (" << endl;
  for (const auto& arg : node.arguments) {
    arg->Accept(this);
  }
  cout << ")" << endl;
  cout << ")" << endl;
}

void PrintVisitor::Visit(const ast::IdentExpression& node) {
  cout << node.ident << endl;
}

void PrintVisitor::Visit(const ast::IntExpression& node) {
  cout << node.integer << endl;
}

void PrintVisitor::Visit(const ast::BinaryExpression& node) {
  cout << "BinaryExpression(" << endl;
  cout << "op = " << node.op << endl;
  cout << "left = ";
  node.left->Accept(this);
  cout << "right = ";
  node.right->Accept(this);
  cout << ")" << endl;
}

}  // namespace print
}  // namespace ts
