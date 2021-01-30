#include "modengine/settings.h"

#include <fstream>

namespace modengine {

bool Settings::load_from(const std::string& path)
{
    try {
        m_config = toml::parse(path);
    } catch (const toml::exception&) {
        return false;
    }

    return true;
}

}