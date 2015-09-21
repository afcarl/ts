#include "eval.h"

#include <algorithm>
#include <iostream>

#include "util.h"

using ts::types::Value;
using ts::types::Func;
using ts::types::Int;
using ts::types::Bool;
using ts::types::INVALID;
using ts::types::FUNC;
using ts::types::INT;
using ts::types::BOOL;
using ts::types::VOID;
using ts::types::Type;

namespace ts {
namespace eval {

namespace {
Environment Bind(
    const std::vector<std::unique_ptr<Value>>& arguments,
    const std::vector<std::unique_ptr<ast::Parameter>>& parameters) {
  Environment bindings;
  Assert(arguments.size() == parameters.size(),
         "Evaluation failed: incorrect number of arguments provided (" +
             std::to_string(arguments.size()) + ", expected " +
             std::to_string(parameters.size()));
  for (int i = 0; i < parameters.size(); i++) {
    const auto& arg = arguments[i];
    const auto& param = parameters[i];
    Assert(arg->type == param->type,
           "Evaluation failed: argument type doesn't match parameter type. "
           "Argument: " +
               types::TypeToString(arg->type) + "; Parameter: " +
               types::TypeToString(param->type));
    Assert(bindings.find(param->name) == bindings.end(),
           "Evaluation failed: function has parameter '" + param->name +
               "' twice");
    bindings[param->name] = arg->Copy();
  }
  return bindings;
}
}  // namespace

void EvalVisitor::Def(const std::string& name, std::unique_ptr<Value> value) {
  if (Env().find(name) != Env().end()) {
    Die("Evaluation failed: " + name + " is already defined in this scope");
  }
  Env()[name] = std::move(value);
}

void EvalVisitor::Def(const std::string& name, const ast::Function& func) {
  Def(name, std::unique_ptr<Value>(new Func(func)));
}

void EvalVisitor::Def(const std::string& name, bool bool_value) {
  Def(name, std::unique_ptr<Value>(new Bool(bool_value)));
}

void EvalVisitor::Def(const std::string& name, int int_value) {
  Def(name, std::unique_ptr<Value>(new Int(int_value)));
}

Value& EvalVisitor::Get(const std::string& name) {
  for (auto it = envs_.rbegin(); it != envs_.rend(); ++it) {
    auto fit = it->find(name);
    if (fit != it->end()) {
      return *fit->second;
    }
  }
  auto git = globals_.find(name);
  if (git != globals_.end()) {
    return *git->second;
  }
  Die("Evaluation failed: Undefined binding: '" + name + "'");
}

std::vector<Environment> EvalVisitor::PopAllEnvs() {
  std::vector<Environment> envs;
  envs.swap(envs_);
  return envs;
}

void EvalVisitor::RestoreAllEnvs(std::vector<Environment> envs) {
  envs_.swap(envs);
}

void EvalVisitor::PushEnv() {
  envs_.push_back(Environment());
}

void EvalVisitor::PopEnv() {
  envs_.pop_back();
}

void EvalVisitor::Visit(const ast::Program& node) {
  std::cout << "Beginning evaluation." << std::endl;
  for (const auto& func : node.functions) {
    globals_[func->name] = std::unique_ptr<Func>(new Func(*func));
  }
  ast::FunctionCall invoke_main;
  invoke_main.function_name = "main";
  invoke_main.Accept(this);
}

void EvalVisitor::Visit(const ast::Function& node) {
  // The parameters are already bound, so just go ahead and evaluate.
  // We should have an extra return value on the stack afterwards. If the
  // function has a non-void return type, then the return value should have
  // the same type.
  PushEnv();
  Type return_type = node.return_type;
  int stack_size = return_values_.size();
  node.body->Accept(this);
  if (return_type != VOID) {
    Assert(return_values_.size() == stack_size + 1,
           "Evaluation failed: non-void function must have a return value");
    const auto& return_value = return_values_.back();
    Assert(return_type == return_value->type,
           "Evaluation failed: returned value from function '" +
               node.name + "' has the wrong type. Expected: " +
               types::TypeToString(return_type) + ", found: " +
               types::TypeToString(return_value->type));
    // Leave the return value on the stack.
  }
  PopEnv();
}

void EvalVisitor::Visit(const ast::IfStatement& node) {
  node.antecedent->Accept(this);
  std::unique_ptr<Value> antecedent = PopReturnValue();
  if (antecedent->AsBool().value) {
    node.consequent->Accept(this);
  }
}

void EvalVisitor::Visit(const ast::Block& node) {
  PushEnv();
  for (const auto& stmt : node.statements) {
    // If we encounter a return statement, return early.
    int stack_size = return_values_.size();
    stmt->Accept(this);
    if (return_values_.size() > stack_size) {
      return;
    }
  }
  PopEnv();
}

void EvalVisitor::Visit(const ast::ReturnStatement& node) {
  node.return_value->Accept(this);
  // The result of evaluating the returned expression is already on the return
  // value stack, so just leave it there.
}

void EvalVisitor::Visit(const ast::ExpressionStatement& node) {
  node.expression->Accept(this);
}

std::unique_ptr<Value> EvalVisitor::PopReturnValue() {
  std::unique_ptr<Value> value;
  value.swap(return_values_.back());
  return_values_.pop_back();
  return value;
}

void EvalVisitor::Visit(const ast::FunctionCall& node) {
  // Evaluate the arguments.
  std::vector<std::unique_ptr<Value>> arguments;
  for (const auto& arg : node.arguments) {
    arg->Accept(this);
    arguments.push_back(PopReturnValue());
  }

  // Check for a special function.
  // TODO: Make this general. Right now it's just 'print'.
  if (node.function_name == "print") {
    for (const auto& arg : arguments) {
      std::cout << arg->AsInt().value << std::endl;
    }
    return;
  }

  // Find the function.
  const ast::Function& func = Get(node.function_name).AsFunc().value;

  // Bind the arguments to the function's formal parameters.
  Environment bindings(Bind(arguments, func.parameters));

  // Lexical scope: Pop and save the current environments -- which will be
  // restored after the function is evaluated -- and push the bindings.
  // Note: Keep the globals.
  std::vector<Environment> envs;
  envs.swap(envs_);
  envs_.push_back(std::move(bindings));

  // Evaluate the function.
  func.Accept(this);

  // Restore the environments. The function's return value is on the top
  // of the return_values stack.
  envs_.swap(envs);
}

void EvalVisitor::Visit(const ast::IdentExpression& node) {
  return_values_.push_back(Get(node.ident).Copy());
}

void EvalVisitor::Visit(const ast::IntExpression& node) {
  return_values_.emplace_back(new Int(std::stoi(node.integer)));
}

void EvalVisitor::EvalSub(const Int& left, const Int& right) {
  return_values_.emplace_back(new Int(left.value - right.value));
}

void EvalVisitor::EvalMult(const Int& left, const Int& right) {
  return_values_.emplace_back(new Int(left.value * right.value));
}

void EvalVisitor::EvalLT(const Int& left, const Int& right) {
  return_values_.emplace_back(new Bool(left.value < right.value));
}

void EvalVisitor::Visit(const ast::BinaryExpression& node) {
  node.left->Accept(this);
  std::unique_ptr<Value> left = PopReturnValue();
  node.right->Accept(this);
  std::unique_ptr<Value> right = PopReturnValue();

  std::string op = node.op;
  if (op == "<") {
    EvalLT(left->AsInt(), right->AsInt());
  } else if (op == "-") {
    EvalSub(left->AsInt(), right->AsInt());
  } else if (op == "*") {
    EvalMult(left->AsInt(), right->AsInt());
  } else {
    Die("Evaluation failed: Unknown operator '" + op + "'");
  }
}

}  // namespace eval
} // namespace ts
