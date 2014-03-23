#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "specexamples.h"  // IWYU pragma: keep
#include "yaml-cpp/yaml.h"   // IWYU pragma: keep

#define YAML_ASSERT(cond)               \
  do {                                  \
    if (!(cond))                        \
      return "  Assert failed: " #cond; \
  } while (false)
#define PARSE(doc, input)          \
  std::stringstream stream(input); \
  YAML::Parser parser(stream);     \
  YAML::Node doc;                  \
  parser.GetNextDocument(doc)
#define PARSE_NEXT(doc) parser.GetNextDocument(doc)

namespace Test {
namespace Spec {
}
}
