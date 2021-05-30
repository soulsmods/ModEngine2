#include <modengine/settings.h>

#include <fstream>
#include <variant>
#include <spdlog/spdlog.h>

using namespace spdlog;

namespace modengine {


template <typename T>
static std::optional<T> value_at_path(toml::table& root, std::initializer_list<std::string> path)
{
    auto node = node_at_path(root, path);
    if (!node) {
        return {};
    }

    return (*node)->value<T>();
}

bool Settings::load_from(const fs::path& path)
{
    try {
        auto config = toml::parse_file(path.string());

        m_config = config;
        m_config_parent_paths.push_back(path.parent_path());
    } catch (const toml::parse_error& e) {
        error("Failed to load config {}", e.what());
        return false;
    }

    return true;
}

bool Settings::is_debug_enabled()
{
    return value_at_path<bool>(m_config, { "modengine", "debug" }).value_or(false);
}

bool Settings::is_crash_reporting_enabled()
{
    return value_at_path<bool>(m_config, { "modengine", "crash_reporting" }).value_or(false);
}

bool Settings::is_external_dll_enumeration_enabled()
{
    return value_at_path<bool>(m_config, { "modengine", "external_dll_discovery" }).value_or(false);
}

const ExtensionInfo Settings::extension(const std::string& name)
{
    auto extension_config = node_at_path(m_config, { "extension", name });
    if (!extension_config) {
        return ExtensionInfo { false, {} };
    }

    auto table = (*extension_config)->as_table();

    return ExtensionInfo {
        table->get("enabled")->value_or(false),
        toml::table(),
    };
}

ConfigReader Settings::get_config_reader()
{
    return ConfigReader(&m_config, m_modengine_local_path);
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

std::vector<fs::path> Settings::script_roots()
{
    std::vector<fs::path> paths;

    auto node = node_at_path(m_config, {"modengine", "script_roots"});
    if (!node || !(*node)->is_array()) {
        return paths;
    }

    auto array = (*node)->as_array();
    for (auto& root : *array) {
        if (!root.is_string()) {
            continue;
        }

        auto root_path = fs::path(root.as_string()->get());

        // if we don't have an absolute path, relativize it to the path we loaded configuration from
        if (!root_path.is_absolute()) {
            root_path = m_modengine_local_path / root_path;
        }

        if (!fs::exists(root_path)) {
            warn("Script root '{}' doesn't exist", root_path.string());
        }

        paths.push_back(root_path);
    }

    return paths;
}

std::vector<fs::path> Settings::get_external_dlls()
{
    std::vector<fs::path> paths;
    auto node = node_at_path(m_config, { "modengine", "external_dlls" });
    if (!node || !(*node)->is_array()) {
        return paths;
    }

    auto array = (*node)->as_array();
    for (auto& dll : *array) {
        if (!dll.is_string()) {
            continue;
        }

        auto dll_path = fs::path(dll.as_string()->get());

        // if we don't have an absolute path, relativize it to the path we loaded configuration from
        if (!dll_path.is_absolute()) {
            dll_path = m_modengine_local_path / dll_path;
        }

        if (!fs::exists(dll_path)) {
            warn("external DLL '{}' doesn't exist", dll_path.string());
        }

        paths.push_back(dll_path);
    }

    return paths;
}

}