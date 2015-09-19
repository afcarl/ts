#include <string>

#include "util.h"

using std::string;
using ts::Die;
using ts::Log;
using ts::ReadFile;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    Die("Usage: ts <file>");
  }
  string filename = argv[1];
  string program_text;
  if (!ReadFile(filename, &program_text)) {
    Die("Failed to read file: " + filename);
  }
  Log("Read: " + program_text);
}
