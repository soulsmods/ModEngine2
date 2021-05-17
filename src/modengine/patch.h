#pragma once

#include "modengine/util/memory_scanner.h"

#include <spdlog/spdlog.h>
#include <string>
#include <functional>
#include <iterator>

namespace modengine {


template <typename It>
std::function<void(uintptr_t)> replace_with(It first, It last) requires std::contiguous_iterator<It>
{
    using T = std::iter_value_t<It>;

    auto size = std::distance(first, last);
    auto input_ptr = new T[size];
    std::copy(first, last, input_ptr);

    return [input_ptr, size](auto addr) {
        T* ptr = (T*)addr;
        std::copy(input_ptr, input_ptr + size, ptr);

        delete[] input_ptr;
    };
}

template <typename T>
std::function<void(uintptr_t)> replace_with(std::initializer_list<T> input)
{
    return replace_with(input.begin(), input.end());
}

std::function<void(uintptr_t)> replace_with(std::string input);

class Patch {
public:
    virtual bool apply(MemoryScanner& scanner) = 0;
    virtual ~Patch()
    {
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
