#pragma once

#include "modengine/hook_set.h"
#include "modengine/settings.h"

#include <memory>
#include <string>
#include <functional>

namespace modengine {

class ModEngine;

class ModEngineExtension {
public:
    ModEngineExtension(std::shared_ptr<ModEngine> mod_engine)
        : m_mod_engine(mod_engine)
    {
    }

    ModEngineExtension(const ModEngineExtension&) = delete;

    virtual ~ModEngineExtension()
    {
    }

    virtual void on_attach() = 0;
    virtual void on_detach() = 0;
    virtual std::string id() = 0;

protected:
    void reapply();

    template <typename T>
    std::shared_ptr<Hook<T>> register_hook(GameType type, const std::string& module, const std::string& function, T detour)
    {
        auto original = DetourFindFunction(module.c_str(), function.c_str());
        if (original == nullptr) {
            throw std::runtime_error("Unable to find a function named " + function + " in " + module);
        }

        return register_hook(type, reinterpret_cast<uintptr_t>(original), detour);
    }

    template <typename T>
    std::shared_ptr<Hook<T>> register_hook(GameType type, uintptr_t location, T detour)
    {
        auto hook = std::make_shared<Hook<T>>(reinterpret_cast<T>(location), detour);
        register_hook(type, std::reinterpret_pointer_cast<Hook<GenericFunctionPointer>>(hook));

        return hook;
    }

    void register_hook(GameType type, std::shared_ptr<Hook<GenericFunctionPointer>> hook);
    void register_patch(GameType type, const std::string_view& signature, std::function<void(uintptr_t)> replace_callback);
    void register_patch(GameType type, uint64_t addr, std::function<void(uintptr_t)> replace_callback);

    const Settings& settings() const;

private:
    std::shared_ptr<ModEngine> m_mod_engine;
};

}
