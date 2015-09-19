#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;

int main(int argc, char* argv[]) {
  auto name1 = "hello";
  auto name2 = "world";
  vector<string> names {name1, name2};
  for (const auto& name : names) {
    cout << name << " ";
  }
  cout << endl;
  return 0;
}
