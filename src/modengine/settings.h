#pragma once

#include <spdlog/spdlog.h>
#include <toml.hpp>

#include <locale>
#include <codecvt>
#include <string>
#include <filesystem>

namespace modengine {

using namespace spdlog;
namespace fs = std::filesystem;

static std::wstring utf8_to_wide(const std::string& str)
{
    auto count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstr[0], count);
    return wstr;
}

struct ExtensionInfo {
    bool enabled;
    toml::value other;

    void from_toml(const toml::value& v)
    {
        this->enabled = toml::find_or<bool>(v, "enabled", false);
        this->other = v;
    }
};

struct ModInfo {
public:
    std::wstring name;
    std::wstring location;
    bool enabled;

    void from_toml(const toml::value& v)
    {
        this->name = utf8_to_wide(toml::find_or<std::string>(v, "name", "Unknown"));
        this->location = utf8_to_wide(toml::find<std::string>(v, "path"));
        this->enabled = toml::find_or<bool>(v, "enabled", false);
    }
};

class Settings {
public:
    Settings()
        : m_config()
    {
    }

    bool load_from(const fs::path& path);

    bool is_debug_enabled();

    bool is_crash_reporting_enabled();

    const ExtensionInfo extension(const std::string& name);

    std::vector<std::string> script_roots();

    std::vector<ModInfo> mods();

    const std::vector<fs::path> config_folders() const;

    const fs::path& modengine_install_path() const;
    void set_modengine_install_path(const fs::path& install_path);

    const fs::path& modengine_local_path() const;
    void set_modengine_local_path(const fs::path& local_path);

    const fs::path& game_path() const;
    void set_game_path(const fs::path& game_path);

    const fs::path& modengine_data_path() const;
    void set_modengine_data_path(const fs::path& data_path);

private:
    std::vector<fs::path> m_config_parent_paths;
    toml::value m_config;
    fs::path m_modengine_data_path;
    fs::path m_modengine_install_path;
    fs::path m_modengine_local_path;
    fs::path m_game_path;
};

}
