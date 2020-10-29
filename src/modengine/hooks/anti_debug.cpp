#include "modengine/hook_set.h"

#include <MINT.h>

namespace modengine::hooks {

typedef VOID(NTAPI* fnKiUserExceptionDispatcher)(PEXCEPTION_RECORD exception, PCONTEXT frame);
std::shared_ptr<Hook<fnKiUserExceptionDispatcher>> kiUserExceptionDispatcherHook;

/**
 * Clear debug registers before dispatching to other exception handlers.
 */
VOID NTAPI HookedKiUserExceptionDispatcher(PEXCEPTION_RECORD exception, PCONTEXT frame)
{
    if (frame != nullptr) {
        frame->Dr0 = 0;
        frame->Dr1 = 0;
        frame->Dr2 = 0;
        frame->Dr3 = 0;
        frame->Dr6 = 0;
        frame->Dr7 = 0;
        frame->ContextFlags &= ~CONTEXT_DEBUG_REGISTERS;
    }

    kiUserExceptionDispatcherHook->original(exception, frame);
}

typedef NTSTATUS(NTAPI* fnNtSetInformationThread)(HANDLE thread_handle, THREADINFOCLASS thread_info_class, PVOID thread_info, ULONG thread_info_len);
std::shared_ptr<Hook<fnNtSetInformationThread>> ntSetInformationThreadHook;

/**
 * Don't allow the game to hide threads from the debugger.
 */
NTSTATUS NTAPI HookedNtSetInformationThread(HANDLE thread_handle, THREADINFOCLASS thread_info_class, PVOID thread_info, ULONG thread_info_len)
{
    bool intercept = thread_info_class == ThreadHideFromDebugger;
    intercept &= thread_handle == NtCurrentThread() || HandleToULong(NtCurrentTeb()->ClientId.UniqueProcess) == GetProcessIdOfThread(thread_handle);
    intercept &= thread_info_len == 0;

    if (intercept) {
        return STATUS_SUCCESS;
    }

    return ntSetInformationThreadHook->original(thread_handle, thread_info_class, thread_info, thread_info_len);
}

/**
 * Contrary to the name, this function actually registers anti-anti-debug hooks.
 */
void register_anti_debug_hooks(HookSet& hooks)
{
    kiUserExceptionDispatcherHook = hooks.install("ntdll.dll", "KiUserExceptionDispatcher", HookedKiUserExceptionDispatcher);
    ntSetInformationThreadHook = hooks.install("ntdll.dll", "NtSetInformationThread", HookedNtSetInformationThread);
}

}
