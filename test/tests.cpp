#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "parsertests.h"
#include "tests.h"

namespace Test {
void RunAll() {
  bool passed = true;
  if (!RunParserTests())
    passed = false;

  if (passed)
    std::cout << "All tests passed!\n";
}
}
