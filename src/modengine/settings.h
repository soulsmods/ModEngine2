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

private:
    toml::table m_config;
};

}
