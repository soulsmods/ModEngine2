#include "modengine/ext/profiling/thread_hooks.h"

#include <spdlog/spdlog.h>

namespace modengine::ext {

using namespace spdlog;
Hook<fnDLThreadHandler> hooked_DLThreadHandler { 0x1417ef4b0, tDLThreadHandler };

INT __cdecl tDLThreadHandler(DLThread* thread)
{
    info("Thread created: {} (id: {})", thread->name().str(), thread->thread_id());
    //    OPTICK_THREAD(thread->name().str())
    return hooked_DLThreadHandler.original(thread);
}

}