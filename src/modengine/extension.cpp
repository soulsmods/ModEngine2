
#include <modengine/extension.h>

#include "modengine/mod_engine.h"

namespace modengine {

void ModEngineExtensionConnectorV1::register_hook(GameType type, Hook<GenericFunctionPointer>* hook)
{
    if (m_mod_engine->game_info().is_supported(type)) {
        m_mod_engine->m_hooks.install(hook);
    }
}

void ModEngineExtensionConnectorV1::register_hook(GameType type, ScannedHook<GenericFunctionPointer>* hook)
{
    if (m_mod_engine->game_info().is_supported(type)) {
        m_mod_engine->m_hooks.install(hook);
    }
}

void ModEngineExtensionConnectorV1::register_patch(GameType type, const std::string_view& signature, std::function<void(uintptr_t)> replace_callback)
{
    if (m_mod_engine->game_info().is_supported(type)) {
        m_mod_engine->m_patches.push_back(std::unique_ptr<Patch>(new ScannedPatch(signature, replace_callback)));
    }
}

void ModEngineExtensionConnectorV1::install_hooks()
{
    m_mod_engine->m_hooks.hook_all();
}

void ModEngineExtensionConnectorV1::register_patch(GameType type, uint64_t addr, std::function<void(uintptr_t)> replace_callback)
{
    if (m_mod_engine->game_info().is_supported(type)) {
        m_mod_engine->m_patches.push_back(std::unique_ptr<Patch>(new FixedPatch(addr, replace_callback)));
    }
}

lua_State* ModEngineExtensionConnectorV1::lua()
{
    auto state_view = m_mod_engine->m_script_host.get_state();
    return state_view.lua_state();
}

ConfigReader ModEngineExtensionConnectorV1::create_config_reader(const char** names, size_t names_len)
{

    toml::table* root = &m_mod_engine->m_settings.m_config;
    for (auto name_idx = 0; name_idx < names_len; name_idx++) {
        auto next = root->get(names[name_idx]);
        if (next != nullptr && next->is_table()) {
            root = next->as_table();
        } else {
            throw std::runtime_error("Error");
        }
    }

    auto path = m_mod_engine->get_settings().modengine_local_path();
    auto reader = ConfigReader(root, path);

    return reader;
}

ExtensionLogger* ModEngineExtensionConnectorV1::logger()
{
    return new ExtensionLogger();
}

}
