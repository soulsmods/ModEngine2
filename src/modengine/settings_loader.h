#pragma once

#include <modengine/settings.h>

#include <filesystem>

namespace modengine {

struct SettingsLoadResult {
    bool found_global_config;
    bool found_local_config;
    bool inline_install;
};

class SettingsLoader {
public:
    SettingsLoader(std::filesystem::path& installation, std::filesystem::path& game)
        : m_installation(installation)
        , m_game(game)
    {
    }
    SettingsLoadResult load(Settings& settings);

private:
    bool load_toml_into(Settings& settings, const fs::path& path);

    std::filesystem::path m_installation;
    std::filesystem::path m_game;
};

}