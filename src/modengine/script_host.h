#pragma once

#include <sol/sol.hpp>
#include <lua.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace modengine {

class ScriptHost {
public:
    ScriptHost();
    ~ScriptHost();

    sol::state_view get_state();
    void evaluate(const std::string_view& text);
    void load_scripts(std::vector<std::string> script_roots, bool enable_reload = true);
    void reload();

private:
    std::vector<std::string> m_script_roots;

    lua_State* m_state;
};

}