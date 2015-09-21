#ifndef EVAL_H_
#define EVAL_H_

#include <map>
#include <string>
#include <vector>

#include "ast.h"
#include "types.h"

namespace ts {
namespace eval {

typedef std::map<std::string, std::unique_ptr<types::Value>> Environment;

class EvalVisitor : public ast::Visitor {
 public:
  void Visit(const ast::Program& node) override;
  void Visit(const ast::Function& node) override;
  void Visit(const ast::IfStatement& node) override;
  void Visit(const ast::Block& node) override;
  void Visit(const ast::ReturnStatement& node) override;
  void Visit(const ast::ExpressionStatement& node) override;
  void Visit(const ast::FunctionCall& node) override;
  void Visit(const ast::IdentExpression& node) override;
  void Visit(const ast::IntExpression& node) override;
  void Visit(const ast::BinaryExpression& node) override;

  void EvalSub(const types::Int& left, const types::Int& right);
  void EvalMult(const types::Int& left, const types::Int& right);
  void EvalLT(const types::Int& left, const types::Int& right);

  Environment& Env() { return envs_.back(); }
  void Def(const std::string& name, std::unique_ptr<types::Value> value);
  void Def(const std::string& name, const ast::Function& func);
  void Def(const std::string& name, int int_value);
  void Def(const std::string& name, bool bool_value);
  types::Value& Get(const std::string& name);
  void PushEnv();
  void PopEnv();
  std::vector<Environment> PopAllEnvs();
  void RestoreAllEnvs(const std::vector<Environment> envs);
  std::unique_ptr<types::Value> PopReturnValue();

 private:
  std::vector<Environment> envs_;
  Environment globals_;
  std::vector<std::unique_ptr<types::Value>> return_values_;
};

}  // namespace eval
} // namespace ts

#endif  // EVAL_H_
