#include "types.h"

#include "util.h"

namespace ts {
namespace types {

std::string TypeToString(Type type) {
  switch (type) {
  case FUNC: return "func";
  case BOOL: return "bool";
  case INT: return "int";
  case VOID: return "void";
  default: Die("No such type");
  }
}

Type TypeFromString(const std::string& type) {
  if (type == "func") {
    return FUNC;
  } else if (type == "bool") {
    return BOOL;
  } else if (type == "int") {
    return INT;
  } else if (type == "void") {
    return VOID;
  }
  Die("No such type: " + type);
}

Func& Value::AsFunc() {
  Die("Evaluation failed: not a function");
}

Int& Value::AsInt() {
  Die("Evaluation failed: not an integer");
}

Bool& Value::AsBool() {
  Die("Evaluation failed: not a boolean");
}

}  // namespace types
}  // namespace ts
