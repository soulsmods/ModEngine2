#pragma once

#include <stdexcept>
#include <unordered_set>

#include <windows.h>
#include <detours/detours.h>
#include <memory>

#include "game_info.h"

namespace modengine {

/**
 * Storing a list of function pointers as void* is undefined behaviour in C++, however, casting between
 * function pointers of different types is completely legal.
 */
using GenericFunctionPointer = void (*)();

template <typename T>
struct Hook {
    Hook(T _original, T _replacement)
        : original(_original)
        , replacement(_replacement)
        , applied(false)
    {
    }

    bool applied;
    T original;
    T replacement;
};

class HookSet {
private:
    std::vector<std::shared_ptr<Hook<GenericFunctionPointer>>> hooks;

public:
    void install(std::shared_ptr<Hook<GenericFunctionPointer>> hook);

    bool hook_all();
    bool unhook_all();
};

}
