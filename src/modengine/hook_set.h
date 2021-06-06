#pragma once

#include <stdexcept>
#include <unordered_set>
#include <concurrent_vector.h>

#include <windows.h>
#include <detours/detours.h>
#include <memory>

#include "game_info.h"
#include "modengine/hook.h"
namespace modengine {


class HookSet {
private:
    concurrency::concurrent_vector<std::unique_ptr<Hook<GenericFunctionPointer>>> hooks;
    std::mutex mutex;
public:
    void install(Hook<GenericFunctionPointer>* hook);

    bool hook_all();
    bool unhook_all();
};

}
