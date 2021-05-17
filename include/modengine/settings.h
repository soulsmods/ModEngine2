#pragma once

#include <toml++/toml.h>

#include <variant>
#include <locale>
#include <codecvt>
#include <string>
#include <filesystem>

namespace modengine {

namespace fs = std::filesystem;

template <typename T>
concept IsSettingsObject = requires(T obj, toml::table& t) {
    { obj.from_toml(t) } -> std::same_as<bool>;
    { T::create_default() } -> std::same_as<T>;

};

struct ExtensionInfo {
    bool enabled;
    toml::table other;

    void from_toml(const toml::table& v)
    {
        this->enabled = v["enabled]"].value_or(false);
        this->other = v;
    }
};

struct ModInfo {
public:
    std::wstring name;
    std::wstring location;
    bool enabled;

    void from_toml(const toml::table& v);
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

    bool is_external_dll_enumeration_enabled();

    std::vector<fs::path> get_external_dlls();

private:
    std::vector<fs::path> m_config_parent_paths;
    toml::table m_config;
    fs::path m_modengine_data_path;
    fs::path m_modengine_install_path;
    fs::path m_modengine_local_path;
    fs::path m_game_path;
};

}
