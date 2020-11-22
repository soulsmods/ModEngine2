#include "modengine/base/scylla_suspend.h"
#include <spdlog/spdlog.h>

namespace modengine::base {

std::shared_ptr<Hook<fnNtSuspendThread>> hooked_NtSuspendThread;
std::shared_ptr<Hook<fnNtResumeThread>> hooked_NtResumeThread;

// Scylla hide suspends the process it's attaching to when injecting. Unfortunately, that means the Mod Engine
// host process will suspend itself when injecting Scylla Hide and hang. In order to be compatible with stock
// Scylla hide, we will work around it by making NtSuspendThread a no-op for the duration of the injection.
NTSTATUS __cdecl tNtSuspendThread(
    HANDLE,
    PULONG)
{
    spdlog::get("modengine")->info("NtSuspendThread called");
    return 0;
}

NTSTATUS __cdecl tNtResumeThread(
    HANDLE,
    PULONG)
{
    spdlog::get("modengine")->info("NtResumeThread called");
    return 0;
}

}