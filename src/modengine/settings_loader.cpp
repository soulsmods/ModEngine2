#include "settings_loader.h"

#include <spdlog/spdlog.h>
#include <fstream>

namespace modengine {

using namespace spdlog;

bool SettingsLoader::load_toml_into(Settings& settings, const fs::path& path)
{
    try {
        auto config = toml::parse_file(path.string());
        settings.m_config = config;
    } catch (const toml::parse_error& e) {
        error("Failed to load config: {}", e.what());
        return false;
    }

    return true;
}

SettingsLoadResult SettingsLoader::load(modengine::Settings& settings)
{
    SettingsLoadResult result;

    fs::path appdata_path(getenv("APPDATA"));
    settings.m_modengine_data_path = appdata_path / "modengine";
    settings.m_modengine_install_path = m_installation;
    settings.m_game_path = m_game ;

    result.inline_install = m_installation == m_game;

    if (!result.inline_install) {
        const auto global_settings_path = m_installation / "config.toml";

        if (fs::exists(global_settings_path)) {
            result.found_global_config = load_toml_into(settings, global_settings_path);
        }
    }

    const auto settings_path_env = std::getenv("MODENGINE_CONFIG");

    if (settings_path_env != nullptr) {
        auto path = fs::path(settings_path_env);
        auto local_modengine_path = path.parent_path();

        result.found_local_config = load_toml_into(settings, path);
        settings.m_modengine_local_path = local_modengine_path;
    }

    return result;
}

}
