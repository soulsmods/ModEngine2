#pragma once
#include "modengine/util/memory_scanner.h"

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

typedef enum
{
    SCAN_FUNCTION,  // The scanned result is the beginning of the target function
    SCAN_CALL_INST, // The scanned result is a call instruction to the target function
} HookScanMode;

template <typename T>
struct ScannedHook {
    ScannedHook()
        : mode(SCAN_FUNCTION)
        , pattern()
        , offset(0)
        , original(nullptr)
        , replacement(nullptr)
        , applied(false)
    {
    }

    ScannedHook(HookScanMode _mode, ScanPattern _pattern, int64_t _offset, T _replacement)
        : mode(_mode)
        , pattern(_pattern)
        , offset(_offset)
        , original(nullptr)
        , replacement(_replacement)
        , applied(false)
    {
    }

    ~ScannedHook()
    {
        spdlog::info("Destroying");
    }

    bool applied;
    HookScanMode mode;
    ScanPattern pattern;
    int64_t offset;
    T original;
    T replacement;
};

}