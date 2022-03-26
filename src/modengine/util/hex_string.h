#pragma once

#include <cstddef>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <sstream>

#include "modengine/util/memory_scanner.h"

namespace modengine::util {

std::string hex_string(const char* input);
ScanPattern hex_aob(const char* input);

}
