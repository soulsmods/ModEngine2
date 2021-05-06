#pragma once

#include "modengine/memory_scanner.h"

#include <spdlog/spdlog.h>
#include <string>
#include <functional>

namespace modengine {

template <typename T>
std::function<void(uintptr_t)> replace_with(std::initializer_list<T> input)
{
    const auto input_len = input.size();

    T* input_ptr = new T[input_len];
    std::copy(input.begin(), input.end(), input_ptr);

    return [input_ptr, input_len](auto addr) {
        T* ptr = (T*)addr;

        for (auto i = 0; i < input_len; i++) {
            ptr[i] = input_ptr[i];
        }

        delete[] input_ptr;
    };
}

class Patch {
public:
    virtual bool apply(MemoryScanner& scanner) = 0;
    virtual ~Patch() {

    }
};

class FixedPatch : public Patch {

public:
    FixedPatch(uintptr_t location, std::function<void(uintptr_t)> replacement_callback)
        : m_location(location)
        , m_replacement_callback(replacement_callback)
    {
    }

    bool apply(MemoryScanner& scanner) override;

private:
    uintptr_t m_location;
    std::function<void(uintptr_t)> m_replacement_callback;
};

class ScannedPatch : public Patch {
public:
    ScannedPatch(std::string_view signature, std::function<void(uintptr_t)> replacement_callback)
        : m_signature(signature)
        , m_replacement_callback(replacement_callback)
    {
    }

    bool apply(MemoryScanner& scanner) override;

private:
    std::string m_signature;
    std::function<void(uintptr_t)> m_replacement_callback;
};
}
