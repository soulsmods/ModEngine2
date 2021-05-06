#pragma once

#include "modengine/extension.h"
#include "modengine/extension_set.h"
#include "modengine/hook_set.h"
#include "modengine/game_info.h"
#include "modengine/patch.h"
#include "modengine/settings.h"
#include "script_host.h"
#include "overlay.h"

#include <spdlog/spdlog.h>
#include <map>

namespace modengine {

class ModEngine {
    friend class ModEngineExtensionConnectorV1;

public:
    ModEngine(GameInfo game, Settings settings);

    void attach();
    void detach();

    ScriptHost& script_host()
    {
        return m_script_host;
    }

    const GameInfo& game_info() const
    {
        return m_game;
    }

    Settings& get_settings()
    {
        return m_settings;
    }

    Overlay& get_overlay()
    {
        return m_overlay;
    }

    [[noreturn]] void run_worker();
private:
    GameInfo m_game;
    HookSet m_hooks;
    Overlay m_overlay;
    Settings m_settings;
    ScriptHost m_script_host;

    std::vector<std::unique_ptr<Patch>> m_patches;
    ExtensionSet m_extensions;
    std::map<std::string, ExtensionInfo> m_extension_info;
    std::thread m_worker;
};

extern std::shared_ptr<ModEngine> mod_engine_global;

}
