#pragma once

namespace modengine {

/**
 * Storing a list of function pointers as void* is undefined behaviour in C++, however, casting between
 * function pointers of different types is completely legal.
 */
using GenericFunctionPointer = void (*)();

template <typename T>
struct Hook {
    Hook()
        : original(nullptr)
        , replacement(nullptr)
        , applied(false)
    {
    }

    Hook(uintptr_t _original, T _replacement)
        : original((T)_original)
        , replacement(_replacement)
        , applied(false)
    {
    }

    Hook(T _original, T _replacement)
        : original(_original)
        , replacement(_replacement)
        , applied(false)
    {
    }

    ~Hook()
    {
        spdlog::info("Destroying");
    }

    bool applied;
    T original;
    T replacement;
};
}