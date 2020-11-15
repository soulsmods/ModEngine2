#pragma once

#include "modengine/util/hex_string.h"

using namespace modengine::util;

namespace modengine::base {

constexpr auto allocator_table_aob = hex_string(
    "0000006000000000"
    "0000006000000000"
    "0000000400000000"
    "0000100000000000"
    "0000100000000000"
    "0000000200000000"
    "0000000100000000");

void increase_fmod_allocation_limits(uintptr_t addr);

}

