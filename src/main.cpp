#include "modengine/mod_engine.h"

#include <optional>
#include <windows.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace modengine;
using namespace spdlog;

typedef int (WINAPI *fnEntry)(void);

std::shared_ptr<ModEngine> modengine::mod_engine_global;
std::shared_ptr<Hook<fnEntry>> hooked_entrypoint;
HookSet entry_hook_set;

int WINAPI modengine_entrypoint(void)
{
    /* We need to restore any changes to entrypoint code.
     * Steam checks the signature of this */
    entry_hook_set.unhook_all();

    Settings settings;
    bool settings_found = settings.load_from(L"modengine.toml");

    if (settings.is_debug_enabled()) {
        // Create debug console
        AllocConsole();
        FILE *stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONIN$", "r", stdin);

        spdlog::set_default_logger(spdlog::stdout_color_mt("modengine"));
        spdlog::set_level(spdlog::level::debug);

        debug("Debug mode is active");
    } else {
        spdlog::set_default_logger(spdlog::basic_logger_mt("modengine", "modengine.log"));
    }

    if (!settings_found) {
        warn("Unable to find modengine configuration file");
    }

    const auto game_info = GameInfo::from_current_module();

    if (!game_info) {
        error("Unable to detect a supported game");
        return false;
    }

    info("ModEngine initializing for {}, version {}", game_info->description(), game_info->version);

    mod_engine_global.reset(new ModEngine {*game_info, settings });
    mod_engine_global->register_extension(std::make_unique<ModEngineBaseExtension>(mod_engine_global));
    mod_engine_global->attach();

    return hooked_entrypoint->original();
}

static bool attach()
{
    hooked_entrypoint = std::make_shared<Hook<fnEntry>>(reinterpret_cast<fnEntry>(DetourGetEntryPoint(nullptr)), modengine_entrypoint);
    entry_hook_set.install(std::reinterpret_pointer_cast<Hook<GenericFunctionPointer>>(hooked_entrypoint));
    entry_hook_set.hook_all();

    return true;
}

static bool detach()
{
    if (mod_engine_global != nullptr) {
        mod_engine_global->detach();
    }

    return true;
}

BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        return attach();
    case DLL_PROCESS_DETACH:
        return detach();
    }
    return TRUE;
}