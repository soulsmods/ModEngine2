#include "modengine/ext/profiling/thread_hooks.h"
#include "modengine/util/platform.h"

#include <spdlog/spdlog.h>

namespace modengine::ext {

using namespace spdlog;
Hook<fnDLThreadHandler> hooked_DLThreadHandler { util::rva2addr(0x17FD480), tDLThreadHandler };

INT __cdecl tDLThreadHandler(DLThread* thread)
{
    info("Thread created: {} (id: {})", thread->name().str(), thread->thread_id());
    //    OPTICK_THREAD(thread->name().str())
    return hooked_DLThreadHandler.original(thread);
}

}