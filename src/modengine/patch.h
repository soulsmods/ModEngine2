#pragma once

#include "modengine/memory_scanner.h"

#include <string>
#include <functional>

namespace modengine {

class Patch {
public:
    Patch(std::string_view signature, std::function<void(uintptr_t)> replacement_callback)
        : m_signature(signature)
        , m_replacement_callback(replacement_callback)
    {
    }

    bool apply(MemoryScanner& scanner);

private:
    std::string m_signature;
    std::function<void(uintptr_t)> m_replacement_callback;
};
}
