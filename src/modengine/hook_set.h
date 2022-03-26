#pragma once

#include <stdexcept>
#include <unordered_set>
#include <concurrent_vector.h>

#include <windows.h>
#include <detours/detours.h>
#include <memory>

#include "game_info.h"
#include "modengine/hook.h"
#include "modengine/util/memory_scanner.h"
namespace modengine {


class HookSet {
private:
    MemoryScanner scanner;
    concurrency::concurrent_vector<std::unique_ptr<Hook<GenericFunctionPointer>>> hooks;
    concurrency::concurrent_vector<std::unique_ptr<ScannedHook<GenericFunctionPointer>>> scanned_hooks;
    std::mutex mutex;
public:
    void install(Hook<GenericFunctionPointer>* hook);
    void install(ScannedHook<GenericFunctionPointer>* hook);

    bool hook_all();
    bool unhook_all();
};

}
