#pragma once

#include "modengine/extension_logger.h"
#include "modengine/hook.h"
#include "modengine/game_type.h"
#include "settings.h"

#include <memory>
#include <string>
#include <functional>

#include <sol/sol.hpp>
#include <spdlog/spdlog.h>

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

    virtual void register_hook(GameType type, Hook<GenericFunctionPointer>* hook);
    virtual void register_hook(GameType type, ScannedHook<GenericFunctionPointer>* hook);
    virtual void install_hooks();

    virtual void register_patch(GameType type, const std::string_view& signature, std::function<void(uintptr_t)> replace_callback);
    virtual void register_patch(GameType type, uint64_t addr, std::function<void(uintptr_t)> replace_callback);

    virtual ConfigReader create_config_reader(const char** names, size_t names_len);
    virtual ExtensionLogger* logger();
    virtual lua_State* lua();
};

using ModEngineExtensionConnector = ModEngineExtensionConnectorV1;

class ModEngineExtension {
public:
    ModEngineExtension(ModEngineExtensionConnector* connector)
        : m_ext_connector(connector)
    {
#ifdef MODENGINE_EXTERNAL
        auto sink = std::make_shared<detail::ExtensionLoggerSink>(connector->logger());
        auto logger = std::make_shared<spdlog::logger>("modengine", sink);
        spdlog::set_default_logger(logger);
#endif
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
    void register_hook(GameType type, Hook<T> *hook, const std::wstring& module, const std::string& function, T detour)
    {
        auto mod = GetModuleHandleW(module.c_str());
        if (mod == nullptr) {
            return;
        }

        auto addr = (uintptr_t)GetProcAddress(mod, function.c_str());
        if (addr == 0) {
            return;
        }

        hook->original = (T)addr;
        hook->replacement = detour;

        m_ext_connector->register_hook(type, (Hook<GenericFunctionPointer>*)hook);
    }

    template <typename T>
    // TODO: ABI compatibility, std::shared_ptr
    void register_hook(GameType type, Hook<T> *hook, uintptr_t location, T detour)
    {
        hook->original = reinterpret_cast<T>(location);
        hook->replacement = detour;

        m_ext_connector->register_hook(type, (Hook<GenericFunctionPointer>*) hook);
    }

    template <typename T>
    void register_hook(GameType type, Hook<T> *hook)
    {
        m_ext_connector->register_hook(type, (Hook<GenericFunctionPointer>*)hook);
    }

    template <typename T>
    void register_hook(GameType type, ScannedHook<T> *hook, const ScanPattern &signature, int64_t offset, T detour, HookScanMode mode)
    {
        hook->mode = mode;
        hook->pattern = signature;
        hook->offset = offset;
        hook->replacement = detour;

        m_ext_connector->register_hook(type, (ScannedHook<GenericFunctionPointer>*) hook);
    }

    template <typename T>
    void register_hook(GameType type, ScannedHook<T> *hook)
    {
        m_ext_connector->register_hook(type, (ScannedHook<GenericFunctionPointer>*)hook);
    }

    // TODO: ABI compatibility, std::string_view, std::function
    inline void register_patch(GameType type, const std::string_view& signature, std::function<void(uintptr_t)> replace_callback)
    {
        m_ext_connector->register_patch(type, signature, replace_callback);
    }

    template <ConfigObject T>
    T get_config()
    {
        std::vector<const char*> paths { "extension", id() };

        return m_ext_connector->create_config_reader(paths.data(), paths.size()).read_config_object<T>({});
    };

    // TODO: ABI compatibility, std::function
    inline void register_patch(GameType type, uint64_t addr, std::function<void(uintptr_t)> replace_callback)
    {
        m_ext_connector->register_patch(type, addr, replace_callback);
    }

    inline sol::state_view lua()
    {
        return sol::state_view(m_ext_connector->lua());
    }

private:
    ModEngineExtensionConnector* m_ext_connector;
};
}
