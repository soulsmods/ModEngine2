#include "modengine/mod_engine.h"

#include <fstream>

namespace modengine {

using namespace spdlog;

void ModEngine::attach()
{
    info("Registering anti-debug and DirectInput hooks");

    MemoryScanner memory_scanner;

    for (auto& extension : m_extensions) {
        const auto extension_id = extension->id();
        const auto extension_settings = m_settings.extension(extension_id);

        if (extension_settings.enabled || extension_id == "base") {
            extension->on_attach();
            info("Enabled extension {}", extension_id);
        }

        m_extension_info[extension_id] = extension_settings;
    }

    for (auto& patch : m_patches) {
        if (!patch->apply(memory_scanner)) {
            error("Failed to apply a patch");
        }
    }

    if (!m_hooks.hook_all()) {
        error("Failed to register all hooks");
    }
}

void ModEngine::detach()
{
    m_hooks.unhook_all();
}

void ModEngine::register_extension(std::unique_ptr<ModEngineExtension> extension)
{
    m_extensions.push_back(std::move(extension));
}

}