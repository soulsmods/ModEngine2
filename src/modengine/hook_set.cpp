#include "hook_set.h"

#include <spdlog/spdlog.h>

namespace modengine {

using namespace spdlog;

void HookSet::install(Hook<GenericFunctionPointer>* hook)
{
    assert(hook->applied == false);
    hooks.push_back(std::unique_ptr<Hook<GenericFunctionPointer>>(hook));
}

void HookSet::install(ScannedHook<GenericFunctionPointer>* hook)
{
    assert(hook->applied == false);
    scanned_hooks.push_back(std::unique_ptr<ScannedHook<GenericFunctionPointer>>(hook));
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

    for (const auto& hook : scanned_hooks) {
        if (hook->applied) {
            continue;
        }
        auto addr = scanner.find(hook->pattern);
        if (!addr) {
            error("Could not find pattern");
            continue;
        }
        debug("Found address at 0x{0:08x}", *addr);
        if (hook->mode == SCAN_CALL_INST) {
            // Assume near call x64 instruction for now
            int32_t offset = *((int32_t*)(*addr + 1));
            uintptr_t address = *addr + 5 + (int64_t)offset;
            debug("Jump offset is 0x{0:04x}. Final offset is 0x{1:08x}", offset, address);

            // If the called function starts with a JMP, it's a thunk function that needs to be followed
            if (*(uint8_t*)address == 0xe9) {
                offset = *((int32_t*)(address + 1));
                address = address + 5 + (int64_t)offset;
                debug("Jump offset is 0x{0:04x}. Final offset is 0x{1:08x}", offset, address);
            }
            hook->original = reinterpret_cast<GenericFunctionPointer>(address);
        } else {
            hook->original = reinterpret_cast<GenericFunctionPointer>(*addr);
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