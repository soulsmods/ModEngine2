#include "modengine/crash_handler.h"
#include "modengine/logging.h"
#include "modengine/mod_engine.h"
#include "modengine/version.h"

#include <optional>
#include <windows.h>
#include <iostream>
#include <modengine/settings_loader.h>

using namespace modengine;
using namespace spdlog;

namespace fs = std::filesystem;

static fs::path modengine_path;
static fs::path game_path;

typedef int(WINAPI* fnEntry)(void);

std::shared_ptr<ModEngine> modengine::mod_engine_global;
Hook<fnEntry> hooked_entrypoint;
HookSet entry_hook_set;

int WINAPI modengine_entrypoint(void)
{
    start_crash_handler(modengine_path, game_path);

    auto is_debugger_enabled = std::getenv("MODENGINE_DEBUG_GAME");
    if (is_debugger_enabled != nullptr) {
        while (!IsDebuggerPresent()) {
            Sleep(100);
        }

        DebugBreak();
    }

    /* We need to restore any changes to entrypoint code.
     * Steam checks the signature of this */
    entry_hook_set.unhook_all();

    SettingsLoader settings_loader(modengine_path, game_path);
    Settings settings;

    auto settings_status = settings_loader.load(settings);
    auto config = settings.get_config_reader().read_config_object<ModEngineConfig>({ "modengine" });
    auto logger = logging::setup_logger(settings.modengine_local_path(), config.debug);
    spdlog::set_default_logger(logger);

    const auto game_info = GameInfo::from_current_module();
    if (!game_info) {
        error("Unable to detect a supported game");
        return false;
    }

    info("ModEngine version {} initializing for {}", modengine::g_version, game_info->description(), game_info->version);
    info("Local settings loaded: {}, Global settings loaded: {}",
        settings_status.found_local_config,
        settings_status.found_global_config);
    info("Main thread ID: {}", GetCurrentThreadId());

    mod_engine_global.reset(new ModEngine { *game_info, settings, config });

    try {
        mod_engine_global->attach();
    } catch (std::exception& e) {
        error("Failed to attach modengine: {}", e.what());
    }

    return hooked_entrypoint.original();
}

static bool attach(HMODULE module)
{
    wchar_t dll_filename[MAX_PATH];

    // Grab the path to the modengine2.dll file, so we can locate the global
    // configuration from here if it exists.
    if (!GetModuleFileNameW(module, dll_filename, MAX_PATH)) {
        return false;
    }

    modengine_path = fs::path(dll_filename).parent_path();
    if (modengine_path.filename() == "bin") {
        modengine_path = modengine_path.parent_path();
    }

    wchar_t game_filename[MAX_PATH];

    // Also get the path to the game executable, to support legacy use-cases of putting
    // mods in the game folder.
    if (!GetModuleFileNameW(nullptr, game_filename, MAX_PATH)) {
        return false;
    }

    game_path = fs::path(game_filename).parent_path();

    hooked_entrypoint.original = reinterpret_cast<fnEntry>(DetourGetEntryPoint(nullptr));
    hooked_entrypoint.replacement = modengine_entrypoint;
    entry_hook_set.install(reinterpret_cast<Hook<modengine::GenericFunctionPointer>*>(&hooked_entrypoint));
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

BOOL APIENTRY DllMain(HMODULE module, DWORD dwReason, LPVOID)
{
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        DetourRestoreAfterWith();
        return attach(module);
    case DLL_PROCESS_DETACH:
        return detach();
    }
    return TRUE;
}