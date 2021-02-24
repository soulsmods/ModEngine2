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

bool Settings::load_from(const fs::path& path)
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

bool Settings::is_debug_enabled()
{
    return toml::expect<bool>(m_config["modengine"]["debug"]).unwrap_or(false);
}

const ExtensionInfo Settings::extension(const std::string& name)
{
    toml::table tbl = toml::find_or<toml::table>(m_config, name, toml::table());

    return toml::get<ExtensionInfo>(toml::value(tbl));
}

std::vector<ModInfo> Settings::mods()
{
    return toml::expect<std::vector<ModInfo>>(m_config["mod_loader"]["mods"]).unwrap_or({});
}

const std::vector<fs::path> Settings::config_folders() const
{
    return m_config_parent_paths;
}

const fs::path& Settings::modengine_install_path() const
{
    return m_modengine_install_path;
}

void Settings::set_modengine_install_path(const fs::path& install_path)
{
    m_modengine_install_path = install_path;
}

const fs::path& Settings::modengine_local_path() const
{
    return m_modengine_local_path;
}

void Settings::set_modengine_local_path(const fs::path& local_path)
{
    m_modengine_local_path = local_path;
}

const fs::path& Settings::game_path() const
{
    return m_game_path;
}

void Settings::set_game_path(const fs::path& game_path)
{
    m_game_path = game_path;
}

const fs::path& Settings::modengine_data_path() const
{
    return m_modengine_data_path;
}

void Settings::set_modengine_data_path(const fs::path& data_path)
{
    m_modengine_data_path = data_path;
}

}