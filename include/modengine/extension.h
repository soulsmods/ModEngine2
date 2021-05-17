#pragma once

#include "modengine/hook.h"
#include "modengine/game_type.h"
#include "settings.h"

#include <memory>
#include <string>
#include <functional>

namespace modengine {

class ModEngine;

class ModEngineExtensionConnectorV1 {
public:
    ModEngineExtensionConnectorV1(ModEngine* mod_engine)
        : m_mod_engine(mod_engine)
    {
    }

private:
    ModEngine* m_mod_engine;

public:
    virtual int connector_version()
    {
        return 1;
    }

    virtual std::shared_ptr<Hook<GenericFunctionPointer>> register_hook(GameType type, const std::string &module, const std::string &symbol, uintptr_t detour);
    virtual void register_hook(GameType type, std::shared_ptr<Hook<GenericFunctionPointer>> hook);
    virtual void install_hooks();

    virtual void register_patch(GameType type, const std::string_view& signature, std::function<void(uintptr_t)> replace_callback);
    virtual void register_patch(GameType type, uint64_t addr, std::function<void(uintptr_t)> replace_callback);

    virtual Settings& get_settings();
};

using ModEngineExtensionConnector = ModEngineExtensionConnectorV1;

class ModEngineExtension {
public:
    ModEngineExtension(ModEngineExtensionConnector* connector)
        : m_ext_connector(connector)
    {
    }

    ModEngineExtension(const ModEngineExtension&) = delete;

    virtual ~ModEngineExtension()
    {
    }

    virtual void on_attach() = 0;
    virtual void on_detach() = 0;
    virtual const char* id() = 0;

protected:
    void reapply()
    {
        m_ext_connector->install_hooks();
    }

    template <typename T>
    std::shared_ptr<Hook<T>> register_hook(GameType type, const std::string& module, const std::string& function, T detour)
    {
        return std::reinterpret_pointer_cast<Hook<T>>(m_ext_connector->register_hook(type, module, function, (uintptr_t)detour));
    }

    template <typename T>
    // TODO: ABI compatibility, std::shared_ptr
    std::shared_ptr<Hook<T>> register_hook(GameType type, uintptr_t location, T detour)
    {
        auto hook = std::make_shared<Hook<T>>(reinterpret_cast<T>(location), detour);
        m_ext_connector->register_hook(type, std::reinterpret_pointer_cast<Hook<GenericFunctionPointer>>(hook));

        return hook;
    }

    // TODO: ABI compatibility, std::string_view, std::function
    void register_patch(GameType type, const std::string_view& signature, std::function<void(uintptr_t)> replace_callback)
    {
        m_ext_connector->register_patch(type, signature, replace_callback);
    }

    // TODO: ABI compatibility, std::function
    void register_patch(GameType type, uint64_t addr, std::function<void(uintptr_t)> replace_callback)
    {
        m_ext_connector->register_patch(type, addr, replace_callback);
    }

    Settings& get_settings()
    {
        return m_ext_connector->get_settings();
    }

    ModEngineExtensionConnector* m_ext_connector;
};

}
