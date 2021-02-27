#include "modengine/mod_engine.h"
#include "modengine/ext/base/base_extension.h"
#include "modengine/ext/debug_menu/ds3/debug_menu_ds3.h"
#include "modengine/ext/crash_handler/crash_handler_extension.h"
#include "modengine/ext/mod_loader/mod_loader_extension.h"
#include "modengine/ext/profiling/profiling_extension.h"
#include "modengine/ext/scylla/scyllahide_extension.h"
#include "modengine/version.h"

#include <optional>
#include <windows.h>
#include <iostream>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

using namespace modengine;
using namespace spdlog;

namespace fs = std::filesystem;

static fs::path modengine_path;
static fs::path game_path;

typedef int(WINAPI* fnEntry)(void);

std::shared_ptr<ModEngine> modengine::mod_engine_global;
std::shared_ptr<Hook<fnEntry>> hooked_entrypoint;
HookSet entry_hook_set;

static std::shared_ptr<spdlog::logger> configure_logger(Settings& settings)
{
    auto logger = std::make_shared<spdlog::logger>("modengine");
    auto log_file_path = settings.modengine_local_path() / "modengine.log";
    auto log_file_sink = new spdlog::sinks::daily_file_sink_mt(log_file_path.string(), 0, 30, false, 5);

    logger->sinks().push_back(std::shared_ptr<spdlog::sinks::daily_file_sink_mt>(log_file_sink));
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);

    if (settings.is_debug_enabled()) {
        // Create debug console
        AllocConsole();
        FILE* stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONIN$", "r", stdin);

        logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logger->set_level(spdlog::level::debug);
    }

    return logger;
}

int WINAPI modengine_entrypoint(void)
{
    /* We need to restore any changes to entrypoint code.
     * Steam checks the signature of this */
    entry_hook_set.unhook_all();

    fs::path appdata_path(getenv("APPDATA"));

    Settings settings;
    settings.set_modengine_data_path(appdata_path / "modengine");
    settings.set_modengine_install_path(modengine_path);
    settings.set_game_path(game_path);

    const auto global_settings_path = modengine_path / "config.toml";
    auto global_settings_found = false;

    if (fs::exists(global_settings_path)) {
        global_settings_found = settings.load_from(global_settings_path);
    }

    const auto settings_path_env = std::getenv("MODENGINE_CONFIG");
    auto settings_found = false;
    if (settings_path_env != nullptr) {
        auto path = fs::path(settings_path_env);
        auto local_modengine_path = path.parent_path();

        settings.set_modengine_local_path(local_modengine_path);
        settings_found = settings.load_from(path);
    }

    spdlog::set_default_logger(configure_logger(settings));

    const auto game_info = GameInfo::from_current_module();
    if (!game_info) {
        error("Unable to detect a supported game");
        return false;
    }

    info("Main thread ID: {}", GetCurrentThreadId());
    info("ModEngine initializing for {}, version {}", game_info->description(), game_info->version);
    info("Local settings: {} (loaded: {}), Global settings: {} (loaded: {})", std::string(settings_path_env), settings_found, global_settings_path.string(), global_settings_found);
    mod_engine_global.reset(new ModEngine { *game_info, settings });
    mod_engine_global->register_extension(std::make_unique<ext::ModEngineBaseExtension>(mod_engine_global));
    mod_engine_global->register_extension(std::make_unique<ext::CrashHandlerExtension>(mod_engine_global));
    mod_engine_global->register_extension(std::make_unique<ext::DebugMenuDS3Extension>(mod_engine_global));
    mod_engine_global->register_extension(std::make_unique<ext::ModLoaderExtension>(mod_engine_global));
    mod_engine_global->register_extension(std::make_unique<ext::ProfilingExtension>(mod_engine_global));
    mod_engine_global->register_extension(std::make_unique<ext::ScyllaHideExtension>(mod_engine_global));

    try {
        mod_engine_global->attach();
    } catch (std::exception& e) {
        error("Failed to attach modengine {}", e.what());
    }

    return hooked_entrypoint->original();
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