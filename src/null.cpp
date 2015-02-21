#include "yaml-cpp/node.h"
#include "yaml-cpp/nodereadimpl.h"
#include "yaml-cpp/null.h"

namespace YAML {
_Null Null;

bool IsNull(const Node& node) { return node.Read(Null); }
}
