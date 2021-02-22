#include "modengine/settings.h"

#include <fstream>

namespace modengine {

static void merge_settings(toml::value& base, toml::value& new_value)
{
    if (base.is_uninitialized()) {
        base = new_value;
        return;
    }

    if (!new_value.is_table()) {
        return;
    }

    for (const auto& [key, _] : new_value.as_table()) {
        auto& orig = base[key];
        auto& value = new_value[key];

        if (value.is_table() && orig.is_table()) {
            merge_settings(base[key], value);
        } else {
            base[key] = value;
        }
    }
}

bool Settings::load_from(const std::filesystem::path& path)
{
    try {
        auto config = toml::parse(path.string());
        merge_settings(m_config, config);

        m_config_parent_paths.push_back(path.parent_path());
    } catch (const toml::exception& e) {
        error("Failed to load config {}", e.what());
        return false;
    }

    return true;
}

const boolean Settings::is_debug_enabled() const
{
    auto modengine = toml::find(m_config, "modengine");
    auto debug = toml::find_or(modengine, "debug", false);

    return debug;
}

const ExtensionInfo Settings::extension(const std::string& name)
{
    toml::table tbl = toml::find_or<toml::table>(m_config, name, toml::table());

    return toml::get<ExtensionInfo>(toml::value(tbl));
}

const std::vector<ModInfo> Settings::mods() const
{
    return toml::find<std::vector<ModInfo>>(m_config, "mod_loader", "mods");
}

const std::vector<std::filesystem::path> Settings::config_folders() const
{
    return m_config_parent_paths;
}
const std::filesystem::path& Settings::modengine_install_path() const
{
    return m_modengine_install_path;
}

void Settings::set_modengine_install_path(const std::filesystem::path& mModengineInstallPath)
{
    m_modengine_install_path = mModengineInstallPath;
}

const std::filesystem::path& Settings::modengine_local_path() const
{
    return m_modengine_local_path;
}

void Settings::set_modengine_local_path(const std::filesystem::path& mModengineLocalPath)
{
    m_modengine_local_path = mModengineLocalPath;
}

const std::filesystem::path& Settings::game_path() const
{
    return m_game_path;
}

void Settings::set_game_path(const std::filesystem::path& mGamePath)
{
    m_game_path = mGamePath;
}

}