#pragma once

#include <stdexcept>
#include <unordered_set>

#include <windows.h>
#include <detours/detours.h>
#include <memory>

#include "game_info.h"
#include "modengine/hook.h"
namespace modengine {


class HookSet {
private:
    std::vector<std::shared_ptr<Hook<GenericFunctionPointer>>> hooks;

public:
    void install(std::shared_ptr<Hook<GenericFunctionPointer>> hook);

    bool hook_all();
    bool unhook_all();
};

}
