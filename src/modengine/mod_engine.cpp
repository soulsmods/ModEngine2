#include "modengine/mod_engine.h"
#include "modengine/hooks/anti_debug.h"
#include "modengine/hooks/direct_input.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace modengine {

ModEngine::ModEngine()
{
    detect_game_type();
    m_logger = spdlog::basic_logger_mt("modengine", "modengine.log", true);
    m_hooks = HookSet(m_gametype);
}

bool ModEngine::detect_game_type()
{
    WCHAR exefilename[MAX_PATH];
    GetModuleFileNameW(NULL, exefilename, MAX_PATH);
    std::wstring fname = std::wstring(exefilename);
    auto lastidx = fname.find_last_of(L"\\");
    auto exename = fname.substr(lastidx + 1);
    //m_logger->info(L"EXE name is %s", exename.data());
    if (exename == L"DarkSoulsRemastered.exe") {
        m_logger->info("Detected game as Dark Souls Remastered");
        m_gametype = DarkSoulsRemastered;
        return true;
    } else if (exename == L"DarkSoulsII.exe") {
        m_logger->info("Detected game as Dark Souls II SOTFS");
        m_gametype = DarkSoulsIISOTFS;
        return true;
    } else if (exename == L"DarkSoulsIII.exe") {
        m_logger->info("Detected game as Dark Souls III");
        m_gametype = DarkSoulsIII;
        return true;
    } else if (exename == L"sekiro.exe") {
        m_logger->info("Detected game as Sekiro");
        m_gametype = Sekiro;
        return true;
    }
    m_logger->error("Fatal Error: Could not detect game. Make sure you are using one of the supported games and that your exe is not renamed.");
    return false;
}

void ModEngine::attach()
{
    m_logger->info("Registering anti-debug and DirectInput hooks");

    hooks::register_anti_debug_hooks(m_hooks);
    hooks::register_direct_input_hooks(m_hooks);

    if (!m_hooks.hook_all()) {
        m_logger->error("Failed to register all hooks");
    }
}

void ModEngine::detach()
{
    m_hooks.unhook_all();
}

}