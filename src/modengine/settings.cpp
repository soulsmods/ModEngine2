#include <modengine/settings.h>

#include <fstream>
#include <variant>
#include <spdlog/spdlog.h>

using namespace spdlog;

namespace modengine {

static std::optional<toml::node*> node_at_path(toml::table& root, std::initializer_list<std::string> path)
{
    std::vector<std::string> keys = path;
    toml::table* container = root.as_table();

    while (!keys.empty()) {
        auto key = keys[0];
        keys.erase(keys.begin());

        if (!container->contains(key)) {
            return {};
        }

        auto value = container->get(key);
        if (keys.empty()) {
            return value;
        }

        container = value->as_table();
    }

    return {};
}

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

std::vector<ModInfo> Settings::mods()
{
    std::vector<ModInfo> mods;
    // @todo: should be part of the ModLoaderExtension configuration
    auto node = node_at_path(m_config, { "extension", "mod_loader", "mods" });
    if (!node || !(*node)->is_array_of_tables()) {
        return mods;
    }

    auto array = (*node)->as_array();
    for (auto& mod : *array) {
        ModInfo mod_info;
        mod_info.from_toml(*mod.as_table());

        mods.push_back(mod_info);
    }

    return mods;
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

std::vector<std::string> Settings::script_roots()
{
    return {};
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

void ModInfo::from_toml(const toml::table& v)
{
    this->name = v["name"].value_or(L"Unknown");
    this->location = v["path"].value_exact<std::wstring>().value();
    this->enabled = v["enabled"].value_or(false);
}
}