#ifndef TYPES_H_
#define TYPES_H_

#include <string>

namespace ts {

namespace ast {
class Function;
}  // namespace ast

namespace types {

class Value;
class Func;
class Int;
class Bool;

enum Type {
  INVALID,
  FUNC,
  INT,
  BOOL,
  VOID,
};

std::string TypeToString(Type type);
Type TypeFromString(const std::string& type);

struct Value {
  explicit Value(Type t) : type(t) {}
  explicit Value(const Value& other) : Value(other.type) {}
  virtual std::unique_ptr<Value> Copy() {
    return std::unique_ptr<Value>(new Value(*this));
  }
  virtual Func& AsFunc();
  virtual Bool& AsBool();
  virtual Int& AsInt();
  Type type;
};

struct Func : public Value {
  explicit Func(const ast::Function& function) : value(function), Value(FUNC) {}
  explicit Func(const Func& other) : Func(other.value) {}
  std::unique_ptr<Value> Copy() override {
    return std::unique_ptr<Value>(new Func(*this));
  }
  Func& AsFunc() override { return *this; }
  const ast::Function& value;
};

struct Bool: public Value {
  explicit Bool(bool boolean) : value(boolean), Value(BOOL) {}
  explicit Bool(const Bool& other) : Bool(other.value) {}
  std::unique_ptr<Value> Copy() override {
    return std::unique_ptr<Value>(new Bool(*this));
  }
  Bool& AsBool() override { return *this; }
  const bool value;
};

struct Int : public Value {
  explicit Int(int integer) : value(integer), Value(INT) {}
  explicit Int(const Int& other) : Int(other.value) {}
  std::unique_ptr<Value> Copy() override {
    return std::unique_ptr<Value>(new Int(*this));
  }
  Int& AsInt() override { return *this; }
  const int value;
};

}  // namespace types
}  // namespace ts

#endif  // TYPES_H_
