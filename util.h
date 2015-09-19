#ifndef UTIL_H_
#define UTIL_H_

#include <string>

namespace ts {

void Assert(bool condition, const std::string& message);

void Die(const std::string& message);

void LogError(const std::string& message);

void Log(const std::string& message);

bool ReadFile(const std::string& filename, std::string* contents);

}  // namespace ts

#endif  // UTIL_H_
