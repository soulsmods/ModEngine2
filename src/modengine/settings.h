#pragma once

#include <toml++/toml.h>

namespace modengine {

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

    // Debug menu
    const boolean is_debug_menu_enabled() const
    {
        return m_config["debugmenu"]["enable"].value_or(false);
    }

    const boolean is_debug_menu_boot_enabled() const
    {
        return m_config["debugmenu"]["boot_menu"].value_or(false);
    }

private:
    toml::table m_config;
};

}
