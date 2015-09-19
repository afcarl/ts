#include <string>

#include "scan.h"
#include "util.h"

using std::string;
using ts::Die;
using ts::Log;
using ts::ReadFile;
using ts::scan::Scanner;
using ts::scan::TypeToString;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    Die("Usage: ts <file>");
  }
  string filename = argv[1];
  string text;
  if (!ReadFile(filename, &text)) {
    Die("Failed to read file: " + filename);
  }
  Scanner scanner(text);
  while (scanner.HasNext()) {
    auto tok = scanner.Next();
    Log("Got: " + TypeToString(tok.type) + "('" + tok.cargo + "')");
  }
}
