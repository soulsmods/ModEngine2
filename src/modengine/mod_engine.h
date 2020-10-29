#pragma once

#include "modengine/hook_set.h"

#include <spdlog/spdlog.h>

namespace modengine {

class ModEngine {
public:
    ModEngine();

    void attach();
    void detach();
private:
    HookSet hooks;
    std::shared_ptr<spdlog::logger> logger;
};
}
