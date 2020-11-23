#include "modengine/mod_engine.h"
#include "modengine/game_info.h"
#include "modengine/base/entry.h"

#include <filesystem>
#include <optional>
#include <windows.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace modengine;

std::shared_ptr<ModEngine> modengine::mod_engine_global;
std::shared_ptr<spdlog::logger> logger_global;
std::thread mod_engine_thread;

static bool attach()
{
    using namespace modengine::base;
    // Create debug console
	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONIN$", "r", stdin);

    //logger_global = spdlog::basic_logger_mt("modengine", "modengine.log", true);
    logger_global = spdlog::stdout_color_mt("modengine");
    const auto game_info = GameInfo::from_current_module();

    spdlog::set_default_logger(logger_global);

    if (!game_info) {
        logger_global->error("Unable to detect a supported game");
        return false;
    }

    logger_global->info("ModEngine initializing for {}, version {}", game_info->description(), game_info->version);
    mod_engine_global.reset(new ModEngine { *game_info, logger_global });
    mod_engine_global->register_extension(std::make_unique<ModEngineBaseExtension>(mod_engine_global));

    hooked_Entry = std::make_shared<Hook<fnEntry>>(reinterpret_cast<fnEntry>(DetourGetEntryPoint(NULL)), Entry);
    entry_hook_set.install(std::reinterpret_pointer_cast<Hook<GenericFunctionPointer>>(hooked_Entry));
    entry_hook_set.hook_all();

    return true;
}

static bool detach()
{
    mod_engine_thread.join();

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