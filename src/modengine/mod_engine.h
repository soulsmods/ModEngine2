#pragma once

#include "modengine/hook_set.h"
#include "game_type.h"

#include <spdlog/spdlog.h>

namespace modengine {

class ModEngine {
private:
    bool detect_game_type();

public:
    ModEngine();

    void attach();
    void detach();
private:
    GameType m_gametype;
    HookSet m_hooks;
    std::shared_ptr<spdlog::logger> m_logger;
};
}
