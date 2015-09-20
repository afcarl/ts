#include "ast.h"

using std::endl;

namespace ts {
namespace ast {

void BinaryExpression::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void IntExpression::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void IdentExpression::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void FunctionCall::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void ExpressionStatement::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void ReturnStatement::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void Block::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void IfStatement::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void Parameter::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void Function::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void Program::Accept(Visitor* visitor) const {
  visitor->Visit(*this);
}

void Visitor::Visit(const ast::Program& node) {
}

void Visitor::Visit(const ast::Function& node) {
}

void Visitor::Visit(const ast::Parameter& node) {
}

void Visitor::Visit(const ast::IfStatement& node) {
}

void Visitor::Visit(const ast::Block& node) {
}

void Visitor::Visit(const ast::ReturnStatement& node) {
}

void Visitor::Visit(const ast::ExpressionStatement& node) {
}

void Visitor::Visit(const ast::FunctionCall& node) {
}

void Visitor::Visit(const ast::IdentExpression& node) {
}

void Visitor::Visit(const ast::IntExpression& node) {
}

void Visitor::Visit(const ast::BinaryExpression& node) {
}

void Visitor::Visit(const ast::Expression& node) {
}

void Visitor::Visit(const ast::Statement& node) {
}

void Visitor::Visit(const ast::Node& node) {
}

}  // namespace ast
}  // namespace ts
