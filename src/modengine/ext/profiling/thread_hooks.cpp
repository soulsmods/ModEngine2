#include "modengine/ext/profiling/thread_hooks.h"
#include "optick.h"

#include <spdlog/spdlog.h>

namespace modengine::ext {

using namespace spdlog;
std::shared_ptr<Hook<fnDLThreadHandler>> hooked_DLThreadHandler;

INT __cdecl tDLThreadHandler(DLThread *thread)
{
    info("Thread created: {} (id: {})", thread->m_name().str(), thread->m_thread_id());
    OPTICK_THREAD(thread->m_name().str())
    return hooked_DLThreadHandler->original(thread);
}

}