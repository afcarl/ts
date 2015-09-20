#ifndef PRINT_H_
#define PRINT_H_

#include "ast.h"

namespace ts {
namespace print {

class PrintVisitor : public ast::Visitor {
 public:
  void Visit(const ast::Program& node) override;
  void Visit(const ast::Function& node) override;
  void Visit(const ast::Parameter& node) override;
  void Visit(const ast::IfStatement& node) override;
  void Visit(const ast::Block& node) override;
  void Visit(const ast::ReturnStatement& node) override;
  void Visit(const ast::ExpressionStatement& node) override;
  void Visit(const ast::FunctionCall& node) override;
  void Visit(const ast::IdentExpression& node) override;
  void Visit(const ast::IntExpression& node) override;
  void Visit(const ast::BinaryExpression& node) override;
};

}  // namespace print
}  // namespace ts

#endif  // PRINT_H_
