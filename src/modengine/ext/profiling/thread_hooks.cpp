#include "modengine/ext/profiling/thread_hooks.h"

#include <spdlog/spdlog.h>

namespace modengine::ext {

using namespace spdlog;
std::shared_ptr<Hook<fnDLThreadHandler>> hooked_DLThreadHandler;

INT __cdecl tDLThreadHandler(DLThread *thread)
{
    info("Thread created: {} (id: {})", thread->name().str(), thread->thread_id());
//    OPTICK_THREAD(thread->name().str())
    return hooked_DLThreadHandler->original(thread);
}

}