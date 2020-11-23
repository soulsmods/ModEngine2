#include "modengine/mod_engine.h"
#include "modengine/game_info.h"

#include <optional>
#include <windows.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace modengine;

std::shared_ptr<ModEngine> modengine::mod_engine_global;
std::thread mod_engine_thread;

static bool attach()
{
    const auto logger = spdlog::basic_logger_mt("modengine", "modengine.log", true);
    const auto game_info = GameInfo::from_current_module();

    spdlog::set_default_logger(logger);

    if (!game_info) {
        logger->error("Unable to detect a supported game");
        return false;
    }

    logger->info("ModEngine initializing for {}, version {}", game_info->description(), game_info->version);
    mod_engine_global.reset(new ModEngine { *game_info, logger });
    mod_engine_global->register_extension(std::make_unique<ModEngineBaseExtension>(mod_engine_global));
    mod_engine_global->attach();

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