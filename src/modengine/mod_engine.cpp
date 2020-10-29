#include "modengine/mod_engine.h"
#include "modengine/hooks/anti_debug.h"
#include "modengine/hooks/direct_input.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace modengine {

ModEngine::ModEngine()
{
    logger = spdlog::basic_logger_mt("modengine", "modengine.log", true);
}

void ModEngine::attach()
{
    logger->info("Registering anti-debug and DirectInput hooks");

    hooks::register_anti_debug_hooks(hooks);
    hooks::register_direct_input_hooks(hooks);

    if (!hooks.hook_all()) {
        logger->error("Failed to register all hooks");
    }
}

void ModEngine::detach()
{
    hooks.unhook_all();
}

}