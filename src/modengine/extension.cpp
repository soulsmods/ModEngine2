
#include <modengine/extension.h>

#include "modengine/extension.h"
#include "modengine/mod_engine.h"

namespace modengine {

void ModEngineExtensionConnectorV1::register_hook(GameType type, std::shared_ptr<Hook<GenericFunctionPointer>> hook)
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

Settings& ModEngineExtensionConnectorV1::get_settings()
{
    return m_mod_engine->get_settings();
}

std::shared_ptr<Hook<GenericFunctionPointer>> ModEngineExtensionConnectorV1::register_hook(GameType type, const std::string& module, const std::string& symbol, uintptr_t detour)
{
    auto original = DetourFindFunction(module.c_str(), symbol.c_str());
    if (original == nullptr) {
        throw std::runtime_error("Unable to find a function named " + symbol + " in " + module);
    }

    auto hook = std::make_shared<Hook<GenericFunctionPointer>>(reinterpret_cast<GenericFunctionPointer>(original), (GenericFunctionPointer)detour);
    register_hook(type, hook);

    return hook;
}

}
