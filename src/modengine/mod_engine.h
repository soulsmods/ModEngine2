#pragma once

#include "modengine/extension.h"
#include "modengine/hook_set.h"
#include "modengine/game_info.h"
#include "modengine/patch.h"

#include <spdlog/spdlog.h>

namespace modengine {

class ModEngineBaseExtension : public ModEngineExtension {
public:
    ModEngineBaseExtension(const std::shared_ptr<ModEngine>& instance)
        : ModEngineExtension(instance)
    {
    }

private:
    void on_attach() override;
    void on_detach() override;
};

class ModEngine {
    friend class ModEngineExtension;

public:
    ModEngine(GameInfo game, std::shared_ptr<spdlog::logger> logger)
        : m_game(game)
        , m_hooks()
        , m_logger(logger)
    {
    }

    void attach();
    void detach();

    void register_extension(std::unique_ptr<ModEngineExtension> extension);

    const GameInfo& game_info() const
    {
        return m_game;
    }

private:
    GameInfo m_game;
    HookSet m_hooks;
    std::vector<Patch> m_patches;
    std::shared_ptr<spdlog::logger> m_logger;
    std::vector<std::unique_ptr<ModEngineExtension>> m_extensions;
};

extern std::shared_ptr<ModEngine> mod_engine_global;

}
