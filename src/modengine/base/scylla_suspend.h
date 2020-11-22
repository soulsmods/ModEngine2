#pragma once

#include "modengine/hook_set.h"

namespace modengine::base {

typedef NTSTATUS (*fnNtSuspendThread)(
    HANDLE threadHandle,
    PULONG previousSuspendCount);

typedef NTSTATUS (*fnNtResumeThread)(
    HANDLE threadHandle,
    PULONG previousSuspendCount);

extern std::shared_ptr<Hook<fnNtSuspendThread>> hooked_NtSuspendThread;
extern std::shared_ptr<Hook<fnNtResumeThread>> hooked_NtResumeThread;

NTSTATUS __cdecl tNtSuspendThread(
    HANDLE threadHandle,
    PULONG previousSuspendCount);

NTSTATUS __cdecl tNtResumeThread(
    HANDLE threadHandle,
    PULONG previousSuspendCount);

}