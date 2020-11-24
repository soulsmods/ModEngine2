#pragma once

#include "modengine/extension.h"
#include "modengine/hook_set.h"
#include "modengine/game_info.h"
#include "modengine/patch.h"
#include "modengine/settings.h"

#include <spdlog/spdlog.h>
#include <toml++/toml.h>

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
    ModEngine(GameInfo game, Settings settings)
        : m_game(game)
        , m_hooks()
        , m_settings(settings)
    {
    }

    void attach();
    void detach();

    void register_extension(std::unique_ptr<ModEngineExtension> extension);

    const GameInfo& game_info() const
    {
        return m_game;
    }

    const Settings& settings() const
    {
        return m_settings;
    }
private:
    Settings m_settings;
    GameInfo m_game;
    HookSet m_hooks;
    std::vector<Patch> m_patches;
    std::vector<std::unique_ptr<ModEngineExtension>> m_extensions;
};

extern std::shared_ptr<ModEngine> mod_engine_global;

}
