#include "modengine/mod_engine.h"
#include "crash_handler.h"

#include <sol_ImGui.h>
#include <fstream>
#include <chrono>

namespace modengine {

using namespace spdlog;
using namespace std::literals::chrono_literals;

void ModEngine::attach()
{
    if (m_settings.is_crash_reporting_enabled()) {
        start_crash_handler(m_settings.modengine_install_path(), m_settings.modengine_data_path());
    }

    MemoryScanner memory_scanner;

    for (auto& extension : m_extensions) {
        const auto extension_id = extension->id();
        const auto extension_settings = m_settings.extension(extension_id);

        if (extension_settings.enabled || extension_id == "base") {
            info("Enabling extension {}", extension_id);
            extension->on_attach();
            info("Enabled extension {}", extension_id);
        }

        m_extension_info[extension_id] = extension_settings;
    }

    for (auto& patch : m_patches) {
        if (!patch->apply(memory_scanner)) {
            error("Failed to apply a patch");
        }
    }

    if (!m_hooks.hook_all()) {
        error("Failed to register all hooks");
    }

    auto lua = m_script_host.get_state();
    sol_ImGui::Init(lua);

    m_script_host.load_scripts(m_settings.script_roots(), true);
    m_worker = std::thread(&ModEngine::run_worker, this);
}

void ModEngine::detach()
{
    m_hooks.unhook_all();
}

void ModEngine::register_extension(std::unique_ptr<ModEngineExtension> extension)
{
    m_extensions.push_back(std::move(extension));
}

[[noreturn]] void ModEngine::run_worker()
{
    info("Starting worker thread");

    while (true) {
        if (GetAsyncKeyState(VK_F2)) {
            m_overlay.set_visible(!m_overlay.is_visible());
        }

        if (GetAsyncKeyState(VK_F3)) {
            m_script_host.reload();
        }

        std::this_thread::yield();
        std::this_thread::sleep_for(16ms);
    }
}

}