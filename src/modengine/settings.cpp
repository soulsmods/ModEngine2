#include "modengine/settings.h"

#include <fstream>

namespace modengine {

bool Settings::load_from(const std::string& path)
{
    try {
        m_config = toml::parse(path);
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

}