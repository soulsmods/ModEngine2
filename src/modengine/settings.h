#pragma once

#include <spdlog/spdlog.h>
#include <toml.hpp>

#include <locale>
#include <codecvt>
#include <string>

namespace modengine {

using namespace spdlog;

static std::wstring utf8_to_wide(const std::string& str)
{
    auto count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstr[0], count);
    return wstr;
}

struct ExtensionInfo {
    std::string name;
    bool enabled;
    toml::value other;

    void from_toml(const toml::value& v)
    {
        this->name = toml::find<std::string>(v, "name");
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
        this->name = utf8_to_wide(toml::find<std::string>(v, "name"));
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

    bool load_from(const std::string& path);

    const boolean is_debug_enabled() const
    {
        auto modengine = toml::find(m_config, "modengine");
        auto debug = toml::find_or(modengine, "modengine", false);

        return debug;
    }

    const ExtensionInfo extension(const std::string& name)
    {
        return toml::find_or<ExtensionInfo>(m_config, name, ExtensionInfo { name, false, toml::value() });
    }

    const std::vector<ModInfo> mods() const
    {
        return toml::find<std::vector<ModInfo>>(m_config, "mod_loader", "mods");
    }
private:
    toml::value m_config;
};

}
