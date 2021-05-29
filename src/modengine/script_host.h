#pragma once

#include <sol/sol.hpp>
#include <lua.h>

#include <filesystem>
#include <map>

namespace fs = std::filesystem;

namespace modengine {

struct ScriptCallback {
    sol::function fn;
    fs::path script;
};

class ScriptHost {
public:
    ScriptHost();
    ~ScriptHost();

    sol::state_view get_state();
    void evaluate(const std::string_view& text);
    void load_scripts(std::vector<fs::path> script_roots, bool enable_reload = true);
    void reload();
    void run_callbacks(std::string name);
    void start_reload();
    void stop_reload();
private:
    bool m_reloading = false;

    std::thread m_reload_thread;
    std::vector<std::string> m_script_roots;
    std::multimap<std::string, ScriptCallback> m_script_callbacks;
    std::map<fs::path, fs::file_time_type> m_script_update_times;

    lua_State* m_state;
    void load_script(fs::path path);
};

}