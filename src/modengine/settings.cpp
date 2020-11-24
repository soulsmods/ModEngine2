#include "modengine/settings.h"

#include <fstream>

namespace modengine {

bool Settings::load_from(const std::wstring_view& path)
{
    try {
        m_config = toml::parse_file(path);
    } catch (const toml::parse_error&) {
        return false;
    }

    return true;
}

}