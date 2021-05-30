#pragma once

#include <sol/sol.hpp>
#include <lua.h>

#include <mutex>
#include <shared_mutex>
#include <filesystem>
#include <map>

namespace modengine {

struct ScriptCallback {
    sol::function fn;
    std::filesystem::path script;
};

class ScriptHost {
public:
    ScriptHost();
    ~ScriptHost();

    sol::state_view get_state();
    void evaluate(const std::string_view& text);
    void load_scripts(const std::vector<std::filesystem::path>& script_roots);
    void start_reload();
    void stop_reload();

    void run_callbacks(const std::string& name);

private:
    std::shared_mutex m_script_lock;
    bool m_reloading = false;

    void reload();

    std::thread m_reload_thread;
    std::vector<std::string> m_script_roots;
    std::multimap<std::string, ScriptCallback> m_script_callbacks;
    std::map<std::filesystem::path, std::filesystem::file_time_type> m_script_update_times;

    lua_State* m_state;
    void load_script(const std::filesystem::path& path);
};

}