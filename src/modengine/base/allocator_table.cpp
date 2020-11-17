#include "modengine/base/allocator_table.h"

#include <cstdint>

namespace modengine::base {

void increase_fmod_allocation_limits(uintptr_t addr)
{
    std::uint64_t *allocator_table = reinterpret_cast<uint64_t*>(addr);
    allocator_table[9] *= 3;
    allocator_table[10] *= 3;
}

}