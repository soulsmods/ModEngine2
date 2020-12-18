#include "modengine/extension.h"
#include "modengine/mod_engine.h"

namespace modengine {

void ModEngineExtension::register_patch(
    GameType type,
    const std::string_view& signature,
    std::function<void(uintptr_t)> replace_callback)
{
    if (m_mod_engine->game_info().is_supported(type)) {
        m_mod_engine->m_patches.push_back(std::unique_ptr<Patch>(new ScannedPatch(signature, replace_callback)));
    }
}

void ModEngineExtension::register_patch(
    GameType type,
    uint64_t addr,
    std::function<void(uintptr_t)> replace_callback)
{
    if (m_mod_engine->game_info().is_supported(type)) {
        m_mod_engine->m_patches.push_back(std::unique_ptr<Patch>(new FixedPatch { addr, replace_callback }));
    }
}

void ModEngineExtension::register_hook(GameType type, std::shared_ptr<Hook<GenericFunctionPointer>> hook)
{
    if (m_mod_engine->game_info().is_supported(type)) {
        m_mod_engine->m_hooks.install(hook);
    }
}

void ModEngineExtension::reapply()
{
    m_mod_engine->m_hooks.hook_all();
}

const Settings& ModEngineExtension::settings() const
{
    return m_mod_engine->settings();
}

}
