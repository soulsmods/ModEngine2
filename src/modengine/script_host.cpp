#include "script_host.h"

#include <spdlog/spdlog.h>

using namespace spdlog;

namespace modengine {

ScriptHost::ScriptHost()
{
    m_state = luaL_newstate();

    if (!m_state) {
        throw std::runtime_error("Unable to initialize Lua state");
    }

    luaL_openlibs(m_state);
}

ScriptHost::~ScriptHost()
{
    lua_close(m_state);
}

sol::state_view ScriptHost::get_state()
{
    return sol::state_view(m_state);
}

void ScriptHost::evaluate(const std::string_view& text)
{
    auto state = sol::state_view(m_state);
    state.script(text);
}

void ScriptHost::load_scripts(std::vector<std::string> script_roots, bool)
{
    for (auto& root : script_roots) {
        for (auto& candidate : fs::recursive_directory_iterator(root)) {
            auto& candidate_path = candidate.path();

            if (candidate_path.extension() != ".lua") {
                continue;
            }

            const auto& candidate_path_str = candidate_path.string();
            luaL_loadfile(m_state, candidate_path_str.c_str());
        }
    }
}

void ScriptHost::reload()
{
    load_scripts(m_script_roots, false);
}

}
