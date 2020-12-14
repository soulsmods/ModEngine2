#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace modengine {

class MemoryScanner {
public:
    MemoryScanner();

    bool replace(const std::string_view &pattern, std::function<void(uintptr_t)> replace_callback);
    bool replace_at(uintptr_t location, std::function<void(uintptr_t)> replace_callback);
private:
    std::vector<MEMORY_BASIC_INFORMATION> m_memory_regions;
};

}