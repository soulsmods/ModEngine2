#include "modengine/mod_engine.h"
#include "modengine/game_info.h"

#include <filesystem>
#include <optional>
#include <windows.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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
#if 0
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
    logger->info("ScyllaHide injection successful.");
    return true;
}
#endif

// Use external scyllahide process to inject if available
static bool inject_scyllahide_external(std::shared_ptr<spdlog::logger> logger)
{
    using std::filesystem::path;
    WCHAR exe_path[MAX_PATH];
    WCHAR command_string[500];

    // Check for scylla hide in game directory
    if (GetFileAttributesW(L".\\scyllahide\\InjectorCLIx64.exe") == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    logger->info("Found valid ScyllaHide injector. Injecting into game.");

    auto exe_path_len = GetModuleFileNameW(nullptr, exe_path, MAX_PATH);

    if (exe_path_len == 0) {
        throw std::runtime_error("Unable to determine EXE file of running game");
    }
    const auto exe_name = path(std::wstring(exe_path)).filename().wstring();
    const auto command = std::wstring(L".\\scyllahide\\InjectorCLIx64.exe ") + exe_name + L" .\\HookLibraryx64.dll nowait";
    wchar_t* command_p = const_cast<wchar_t*>(command.c_str());

    if (command.length() > 499) {
        return false;
    }
    command.copy(command_string, 500);
    command_string[command.length()] = 0;
    logger->info(command_string);

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    bool result = CreateProcessW(
        L".\\scyllahide\\InjectorCLIx64.exe",
        command_p,
        NULL, NULL, FALSE, 0, NULL,
        L".\\scyllahide",
        &si, &pi
    );

    if (result) {
        logger->info("Launched Scyllahide process.");

        // Wait for it to finish
        DWORD res = WaitForSingleObject(pi.hProcess, 2000);
        if (res == WAIT_TIMEOUT) {
            logger->info("Scyllahide process timed out.");
        }
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return res;
    }
    return false;
}

static bool attach()
{
    // Create debug console
	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONIN$", "r", stdin);

    //logger_global = spdlog::basic_logger_mt("modengine", "modengine.log", true);
    logger_global = spdlog::stdout_color_mt("modengine");
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
    inject_scyllahide_external(logger_global);

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