#include "modengine/mod_engine.h"
#include "modengine/base/allocator_table.h"
#include "modengine/base/dinput_hook.h"
#include "modengine/base/winsockets_disable.h"

#include <fstream>

namespace modengine {

void ModEngine::attach()
{
    m_config = toml::parse_file("modengine.toml");
    m_logger->info("Registering anti-debug and DirectInput hooks");

    for (auto& extension : m_extensions) {
        extension->on_attach();
    }

    MemoryScanner memory_scanner;

    for (auto& patch : m_patches) {
        if (!patch.apply(memory_scanner)) {
            m_logger->error("Failed to apply a patch");
        }
    }

    if (!m_hooks.hook_all()) {
        m_logger->error("Failed to register all hooks");
    }
}

void ModEngine::detach()
{
    m_logger->flush();
    m_hooks.unhook_all();
}

void ModEngine::register_extension(std::unique_ptr<ModEngineExtension> extension)
{
    m_extensions.push_back(std::move(extension));
}

void ModEngineBaseExtension::on_attach()
{
    using namespace modengine::base;

    register_patch(static_cast<GameType>(DS3 | SEKIRO), allocator_table_aob.as_string(), increase_fmod_allocation_limits);
    hooked_DirectInput8Create = register_hook(ALL, "C:\\windows\\system32\\dinput8.dll", "DirectInput8Create", DirectInput8Create);
    hooked_WSAStartup = register_hook(static_cast<GameType>(DS_REMASTERED | DS2 | DS3), "C:\\windows\\system32\\ws2_32.dll", "WSAStartup", tWSAStartup);
}

void ModEngineBaseExtension::on_detach()
{
}

}