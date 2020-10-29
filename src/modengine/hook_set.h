#pragma once

#include <stdexcept>
#include <unordered_set>

#include <windows.h>
#include <detours/detours.h>
#include <memory>

namespace modengine {
/**
 * Storing a list of function pointers as void* is undefined behaivor in C++, however, casting between
 * function pointers of different types is completely legal.
 */
using GenericFunctionPointer = void (*)();

template <typename T>
struct Hook {
    Hook(T _original, T _replacement)
        : original(_original)
        , replacement(_replacement)
    {
    }

    T original;
    T replacement;
};

class HookSet {
private:
    std::vector<std::shared_ptr<Hook<GenericFunctionPointer>>> hooks;

public:
    template <typename T>
    std::shared_ptr<Hook<T>> install(const std::string& module, const std::string& function, T detour)
    {
        auto original = DetourFindFunction(module.c_str(), function.c_str());
        if (original == nullptr) {
            throw std::runtime_error("Unable to find a function named " + function + " in " + module);
        }

        return install(static_cast<T>(original), detour);
    }

    template <typename T>
    std::shared_ptr<Hook<T>> install(T original, T detour)
    {
        auto hook = std::make_shared<Hook<T>>(original, detour);
        hooks.push_back(std::reinterpret_pointer_cast<Hook<GenericFunctionPointer>>(hook));

        return hook;
    }


    bool hook_all();
    bool unhook_all();
};

}
