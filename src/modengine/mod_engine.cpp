#include "modengine/mod_engine.h"
#include "modengine/base/allocator_table.h"
#include "modengine/base/archive_file_overrides.h"
#include "modengine/base/dinput_hook.h"
#include "modengine/base/scyllahide_inject.h"
#include "modengine/base/winsockets_disable.h"

#include <fstream>

namespace modengine {

using namespace spdlog;

void ModEngine::attach()
{
    info("Registering anti-debug and DirectInput hooks");

    MemoryScanner memory_scanner;

    for (auto& extension : m_extensions) {
        extension->on_attach();
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

void ModEngineBaseExtension::on_attach()
{
    using namespace modengine::base;

    if (settings().is_debug_enabled()) {
        if (!inject_scyllahide_external()) {
            warn("Unable to inject ScyllaHide hook");
        } else {
            info("ScyllaHide successfully injected");
        }
    }

    register_patch(DS3 | SEKIRO, allocator_table_aob.as_string(), increase_fmod_allocation_limits);

    hooked_DirectInput8Create = register_hook(ALL, "C:\\windows\\system32\\dinput8.dll", "DirectInput8Create", DirectInput8Create);

    // TODO: AOB scan this?
    hooked_CreateFileW = register_hook(ALL, "C:\\windows\\system32\\kernel32.dll", "CreateFileW", tCreateFileW);
    hooked_virtual_to_archive_path_ds3 = register_hook(DS3, 0x14007d5e0, virtual_to_archive_path_ds3);

    for (const auto& mod : settings().mods()) {
        info(L"Installing mod location {}", mod.location);
        hooked_file_roots.push_back(mod.location);
    }

    if (settings().is_disable_networking()) {
        hooked_WSAStartup = register_hook(DS_REMASTERED | DS2, "C:\\windows\\system32\\ws2_32.dll", "WSAStartup", tWSAStartup);
        hooked_IPv4AddrFunction = register_hook(DS3, 0x1418c0e60, tIPv4AddrFunction);
    }

}

void ModEngineBaseExtension::on_detach()
{
}

}