#pragma once

#include <toml++/toml.h>

#include <variant>
#include <locale>
#include <codecvt>
#include <string>
#include <filesystem>

namespace modengine {

namespace fs = std::filesystem;

static std::optional<toml::node*> node_at_path(toml::table& root, std::initializer_list<std::string> path)
{
    if (path.size() == 0) {
        return (toml::node*)&root;
    }

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

class ConfigReader;
template <typename T>
concept ConfigObject = requires(T obj, ConfigReader t)
{
    {
        obj.from_toml(t)
        } -> std::same_as<bool>;
};

class ConfigReader {
private:
    template <typename T>
    std::optional<T> node_to_val(toml::node* node)
    {
        return node->value<T>();
    }

    template <>
    std::optional<fs::path> node_to_val(toml::node* node)
    {
        auto fs_path = fs::path(node->value<std::wstring>().value());

        // if we don't have an absolute path, relativize it to the path we loaded configuration from
        if (!fs_path.is_absolute()) {
            fs_path = m_relative_path / fs_path;
        }

        if (!fs::exists(fs_path)) {
            return {};
        }

        return fs_path;
    }

public:
    ConfigReader(toml::table* root_, std::filesystem::path& rel_path_)
        : m_root(root_)
        , m_relative_path(rel_path_)
    {
    }

    template <typename T>
    std::optional<T> read_config_option(std::initializer_list<std::string> path)
    {
        auto node = node_at_path(*m_root, path);
        if (!node) {
            return std::nullopt;
        }

        return node_to_val<T>(*node);
    }

    template <typename T>
    std::vector<T> read_config_options(std::initializer_list<std::string> path)
    {
        std::vector<T> options;

        auto node = node_at_path(*m_root, path);
        if (!node || !(*node)->is_array()) {
            return options;
        }

        for (auto& el : *(*node)->as_array()) {
            auto el_value = node_to_val<T>(&el);
            if (el_value) {
                options.push_back(*el_value);
            }
        }

        return options;
    }

    template <ConfigObject T>
    std::vector<T> read_config_objects(std::initializer_list<std::string> path)
    {
        std::vector<T> results;

        auto node = node_at_path(*m_root, path);
        if (!node || !(*node)->is_array_of_tables()) {
            return results;
        }

        auto array = (*node)->as_array();
        for (auto& obj_node : *array) {
            auto obj = T();
            auto reader = ConfigReader(obj_node.as_table(), m_relative_path);

            if (obj.from_toml(reader)) {
                results.push_back(obj);
            }
        }

        return results;
    }

    template <ConfigObject T>
    T read_config_object(std::initializer_list<std::string> path)
    {
        auto obj = T();
        auto node = node_at_path(*m_root, path);

        if (node && (*node)->is_table()) {
            auto table = (*node)->as_table();
            auto reader = ConfigReader(table, m_relative_path);

            obj.from_toml(reader);
        }

        return obj;
    }

private:
    toml::table* m_root;
    std::filesystem::path m_relative_path;
};

class Settings {
    friend class SettingsLoader;
    friend class ModEngineExtensionConnectorV1;
public:
    Settings()
        : m_config()
    {
    }

    inline const fs::path& modengine_install_path() const
    {
        return m_modengine_install_path;
    }

    inline const fs::path& modengine_local_path() const
    {
        return m_modengine_local_path;
    }

    inline const fs::path& game_path() const
    {
        return m_game_path;
    }

    inline const fs::path& modengine_data_path() const
    {
        return m_modengine_data_path;
    }

    inline ConfigReader get_config_reader()
    {
        return ConfigReader(&m_config, m_modengine_local_path);
    }

private:
    toml::table m_config;
    fs::path m_modengine_data_path;
    fs::path m_modengine_install_path;
    fs::path m_modengine_local_path;
    fs::path m_game_path;
};

}
