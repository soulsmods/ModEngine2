#pragma once

#include <spdlog/spdlog.h>
#include <toml++/toml.h>

#include <locale>
#include <codecvt>
#include <string>

namespace modengine {

using namespace spdlog;

struct ModInfo {
public:
    std::wstring name;
    bool enabled;
    std::wstring location;
};

static std::wstring utf8_to_wide(const std::string& str)
{
    auto count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int) str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int) str.length(), &wstr[0], count);
    return wstr;
}

class Settings {
public:
    Settings()
        : m_config()
    {
    }

    bool load_from(const std::wstring_view& path);

    const boolean is_debug_enabled() const
    {
        return m_config["modengine"]["debug"].value_or(false);
    }

    const boolean is_disable_networking() const
    {
        // Default to always disabling networking unless explicitely opting in
        return m_config["modengine"]["disable_networking"].value_or(true);
    }

    const std::vector<ModInfo> mods() const
    {
        auto mod_info = std::vector<ModInfo>();
        auto mod_config = m_config["mod"].as_table();

        if (!mod_config) {
            return mod_info;
        }

        for (auto&& [k, v] : *mod_config) {
            const auto mod = *v.as_table();
            const auto path = mod["path"].as_string();
            const auto enabled = mod["enabled"].value_or(false);

            if (!path) {
                warn("Mod {} has no path set, skipping.");
                continue;
            }

            mod_info.push_back(ModInfo { utf8_to_wide(k), enabled, utf8_to_wide(path->get()) });
        }

        return mod_info;
    }

    // Debug menu
    const boolean is_debug_menu_enabled() const
    {
        return m_config["debugmenu"]["enable"].value_or(false);
    }

    const boolean is_debug_menu_boot_enabled() const
    {
        return m_config["debugmenu"]["boot_menu"].value_or(false);
    }

    // Profiler
    const boolean is_profiler_enabled() const
    {
        return m_config["profiler"]["enable"].value_or(false);
    }

private:
    toml::table m_config;
};

}
