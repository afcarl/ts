#include "util.h"

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

namespace ts {

void Assert(bool condition, const std::string& message) {
  if (!condition) {
    Die(message);
  }
}

void Die(const std::string& message) {
  LogError(message);
  exit(1);
}

void LogError(const std::string& message) {
  std::cerr << message << std::endl;
}

void Log(const std::string& message) {
  std::cout << message << std::endl;
}

bool ReadFile(const std::string& filename, std::string* contents) {
  std::ifstream stream(filename);
  if (!stream.is_open()) {
    return false;
  }
  *contents = std::string(std::istreambuf_iterator<char>(stream),
                          std::istreambuf_iterator<char>());
  return true;
}

}  // namespace ts
