#include "modengine/mod_engine.h"

#include "entry.h"

namespace modengine::base {

std::shared_ptr<Hook<fnEntry>> hooked_Entry;
HookSet entry_hook_set;

int WINAPI Entry(void)
{
    spdlog::info("At hooked entrypoint");
    entry_hook_set.unhook_all();
    mod_engine_global->attach();
    return hooked_Entry->original();
}

}