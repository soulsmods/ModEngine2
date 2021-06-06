#include "hook_set.h"

#include <spdlog/spdlog.h>

namespace modengine {

using namespace spdlog;

void HookSet::install(Hook<GenericFunctionPointer>* hook)
{
    assert(hook->applied == false);
    hooks.push_back(std::unique_ptr<Hook<GenericFunctionPointer>>(hook));
}

bool HookSet::hook_all()
{
    auto txn_status = DetourTransactionBegin();

    if (txn_status != NO_ERROR) {
        throw std::runtime_error("Unable to create a Detours transaction");
    }

    auto count = 0;

    for (const auto& hook : hooks) {
        if (hook->applied) {
            continue;
        }

        txn_status = DetourAttach((PVOID*)&hook->original, (PVOID)hook->replacement);

        if (txn_status != NO_ERROR) {
            (void)DetourTransactionAbort();
            break;
        }

        hook->applied = true;
        count++;
    }

    info("Applied {} hooks", count);
    txn_status = DetourTransactionCommit();
    return txn_status == NO_ERROR;
}

bool HookSet::unhook_all()
{
    DetourTransactionBegin();

    for (const auto& hook : hooks) {
        DetourDetach((PVOID*)&hook->original, (PVOID)hook->replacement);
    }

    DetourTransactionCommit();
    return true;
}

}