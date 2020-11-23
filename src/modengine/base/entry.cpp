#include "modengine/base/scyllahide_inject.h"
#include "modengine/mod_engine.h"

#include "entry.h"

namespace modengine::base {

std::shared_ptr<Hook<fnEntry>> hooked_Entry;
HookSet entry_hook_set;

int WINAPI Entry(void)
{
    spdlog::info("At hooked entrypoint");
    entry_hook_set.unhook_all();
    spdlog::info("Entrypoint unhooked");

    // Attempt to inject scyllahide if it's available
    if (!inject_scyllahide_external()) {
        spdlog::info("Scyllahide was not injected. Attempting to debug this game will crash!");
    }

    mod_engine_global->attach();
    spdlog::info("Calling original");
    return hooked_Entry->original();
}

}