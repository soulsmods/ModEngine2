#include "script_host.h"

#include <spdlog/spdlog.h>
#include <fstream>

using namespace spdlog;

namespace fs = std::filesystem;

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

void ScriptHost::load_script(const fs::path& path)
{
    std::unique_lock writer(m_script_lock);
    std::erase_if(m_script_callbacks, [&path](const auto& item) {
        auto const& [_, cb] = item;
        return cb.script == path;
    });

    sol::state_view state(m_state);
    sol::environment loader_env(state, sol::create, state.globals());

    info("Loading script from {}", path.string());

    loader_env["register_callback"] = [&](std::string name, sol::function fn) {
        const ScriptCallback& callback = ScriptCallback {
            fn,
            path.string()
        };

        info("Registered {} callback for {}", name, path.string());

        m_script_callbacks.insert({ name, callback });
    };

    std::ifstream script_stream(path);
    std::stringstream script_buffer;
    script_buffer << script_stream.rdbuf();

    state.script(script_buffer.str(), loader_env);
    m_script_update_times[path] = fs::last_write_time(path);
}

void ScriptHost::load_scripts(const std::vector<fs::path>& script_roots)
{
    for (auto& root : script_roots) {
        info("Searching for scripts in root: {}", root.string());

        for (auto& candidate : fs::recursive_directory_iterator(root)) {
            auto& candidate_path = candidate.path();

            if (candidate_path.extension() != ".lua") {
                continue;
            }

            load_script(candidate_path);
        }
    }
}

void ScriptHost::reload()
{
    using namespace std::chrono_literals;

    while (m_reloading) {
        for (const auto& [path, time] : m_script_update_times) {
            auto last_update = fs::last_write_time(path);
            if ((last_update - time) > 2s) {
                debug("{} has changed, reloading", path.string());

                load_script(path);
            }
        }

        std::this_thread::sleep_for(1000ms);
    }
}

void ScriptHost::run_callbacks(const std::string& name)
{
    std::shared_lock reader(m_script_lock);

    for (auto itr = m_script_callbacks.begin(); itr != m_script_callbacks.end(); itr++) {
        if (itr->first == name) {
            itr->second.fn();
        }
    }
}

void ScriptHost::start_reload()
{
    if (m_reloading) {
        warn("Already reloading scripts");
        return;
    }

    m_reloading = true;
    m_reload_thread = std::thread(&ScriptHost::reload, this);
}

void ScriptHost::stop_reload()
{
    m_reloading = false;
    m_reload_thread.join();
}

}
