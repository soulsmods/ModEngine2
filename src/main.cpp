#include "modengine/mod_engine.h"
#include "modengine/game_info.h"

#include <codecvt>
#include <locale>
#include <optional>
#include <windows.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace modengine;

std::shared_ptr<ModEngine> modengine::mod_engine_global;
std::shared_ptr<spdlog::logger> logger_global;
std::thread mod_engine_thread;

typedef void(__cdecl * LOGWRAPPER)(const wchar_t *msg);
typedef void (__cdecl *fpScyllaHideInit)(const WCHAR* Directory, LOGWRAPPER Logger, LOGWRAPPER ErrorLogger);
typedef void (__cdecl *fpScyllaHideDebugLoop)(DEBUG_EVENT* DebugEvent);

void scyllahide_log(const wchar_t *msg)
{
    logger_global->info(L"[ScyllaHide] {}", std::wstring_view(msg));
}

// Optionally tries to load the scyllahide generic plugin and self inject
static bool inject_scyllahide(std::shared_ptr<spdlog::logger> logger)
{
    HINSTANCE scylla_lib = LoadLibraryW(L"scyllahide\\ScyllaHideGenericPluginx64.dll");
    if (scylla_lib == NULL) {
        return false;
    }

    fpScyllaHideInit ScyllaHideInit = (fpScyllaHideInit)GetProcAddress(scylla_lib, "ScyllaHideInit");
    fpScyllaHideDebugLoop ScyllaHideDebugLoop = (fpScyllaHideDebugLoop)GetProcAddress(scylla_lib, "ScyllaHideDebugLoop");

    if (ScyllaHideInit == NULL || ScyllaHideDebugLoop == NULL)
    {
        FreeLibrary(scylla_lib);
        return false;
    }

    logger->info("Found valid ScyllaHide plugin. Injecting into game.");
    ScyllaHideInit(L"scyllahide\\", scyllahide_log, scyllahide_log);

    // Emulate the plugin injection process
    DEBUG_EVENT event;
    event.dwDebugEventCode = CREATE_PROCESS_DEBUG_EVENT;
    event.dwProcessId = GetCurrentProcessId();
    event.u.CreateProcessInfo.lpStartAddress = NULL;
    ScyllaHideDebugLoop(&event);

    event.dwDebugEventCode = EXCEPTION_DEBUG_EVENT;
    event.u.Exception.ExceptionRecord.ExceptionCode = ((DWORD)0x80000003L); // STATUS_BREAKPOINT
    ScyllaHideDebugLoop(&event);

    event.dwDebugEventCode = LOAD_DLL_DEBUG_EVENT;
    //ScyllaHideDebugLoop(&event);

    FreeLibrary(scylla_lib);
    return true;
}

static bool attach()
{
    logger_global = spdlog::basic_logger_mt("modengine", "modengine.log", true);
    const auto game_info = GameInfo::from_current_module();

    spdlog::set_default_logger(logger_global);

    if (!game_info) {
        logger_global->error("Unable to detect a supported game");
        return false;
    }

    logger_global->info("ModEngine initializing for {}, version {}", game_info->description(), game_info->version);
    mod_engine_global.reset(new ModEngine { *game_info, logger_global });
    mod_engine_global->register_extension(std::make_unique<ModEngineBaseExtension>(mod_engine_global));

    // Attempt to inject ScyllaHide if it's available
    // NOTE: ScyllaHide plugin calls SafeSuspendProcess on the process being hooked, which has the unfortunate
    // effect of suspending this process. SafeSuspendProcess therefore needs to be hooked to be a no-op before
    // this is called.
    inject_scyllahide(logger_global);

    mod_engine_global->attach();

    return true;
}

static bool detach()
{
    mod_engine_thread.join();

    if (mod_engine_global != nullptr) {
        mod_engine_global->detach();
    }

    return true;
}

BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        return attach();
    case DLL_PROCESS_DETACH:
        return detach();
    }
    return TRUE;
}