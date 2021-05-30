#include "modengine/mod_engine.h"
#include "modengine/crash_handler.h"

#include "modengine/ext/base/base_extension.h"
#include "modengine/ext/debug_menu/ds3/debug_menu_ds3.h"
#include "modengine/ext/mod_loader/mod_loader_extension.h"
#include "modengine/ext/profiling/profiling_extension.h"
#include "modengine/ext/scylla/scyllahide_extension.h"

#include <sol_ImGui.h>
#include <fstream>
#include <chrono>

using namespace spdlog;
using namespace std::literals::chrono_literals;
using namespace modengine::ext;

namespace modengine {

ModEngine::ModEngine(GameInfo game, Settings settings)
    : m_game(game)
    , m_hooks()
    , m_settings(settings)
    , m_extensions(new ModEngineExtensionConnectorV1(this))
{
    m_extensions.register_builtin_extension<ModEngineBaseExtension>();
    m_extensions.register_builtin_extension<DebugMenuDS3Extension>();
    m_extensions.register_builtin_extension<ModLoaderExtension>();
    m_extensions.register_builtin_extension<ProfilingExtension>();
    m_extensions.register_builtin_extension<ScyllaHideExtension>();
}

void ModEngine::attach()
{
    if (m_settings.is_crash_reporting_enabled()) {
        start_crash_handler(m_settings.modengine_install_path(), m_settings.modengine_data_path());
    }

    MemoryScanner memory_scanner;
    m_extensions.load_extensions(m_settings.get_external_dlls(), m_settings.is_external_dll_enumeration_enabled());
    m_extensions.attach_all(m_settings);

    for (auto& patch : m_patches) {
        if (!patch->apply(memory_scanner)) {
            error("Failed to apply a patch");
        }
    }

    if (!m_hooks.hook_all()) {
        error("Failed to register all hooks");
    }

    auto lua = m_script_host.get_state();
    sol_ImGui::Init(lua);

    m_script_host.load_scripts(m_settings.script_roots());
    m_script_host.start_reload();
    m_worker = std::thread(&ModEngine::run_worker, this);
}

void ModEngine::detach()
{
    m_hooks.unhook_all();
}

[[noreturn]] void ModEngine::run_worker()
{
    info("Starting worker thread");

    while (true) {
        std::this_thread::yield();
        std::this_thread::sleep_for(16ms);
    }
}

bool ModEngineConfig::from_toml(ConfigReader& reader)
{
    crash_reporting = reader.read_config_option<bool>({"crash_reporting"}).value_or(true);
    debug = reader.read_config_option<bool>({"debug"}).value_or(false);

    external_dll_enumeration = reader.read_config_option<bool>({"external_dll_discovery"}).value_or(true);
    external_dlls = reader.read_config_options<fs::path>({"external_dlls"});

    script_reloading = reader.read_config_option<bool>({"script_reloading"}).value_or(false);
    script_roots = reader.read_config_options<fs::path>({"script_roots"});
    return true;
}
}