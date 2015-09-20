#include <iostream>
#include <string>

#include "ast.h"
#include "parse.h"
#include "scan.h"
#include "util.h"

using std::string;
using ts::Die;
using ts::Log;
using ts::ReadFile;
using ts::ast::Program;
using ts::parse::Parser;
using ts::scan::Scanner;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    Die("Usage: ts <file>");
  }
  string filename = argv[1];
  string text;
  if (!ReadFile(filename, &text)) {
    Die("Failed to read file: " + filename);
  }
  std::unique_ptr<Program> program =
      Parser(std::unique_ptr<Scanner>(new Scanner(text))).Program();
  std::cout << *program;
}
