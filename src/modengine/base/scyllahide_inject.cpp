#include <filesystem>
#include <optional>
#include <windows.h>
#include <spdlog/spdlog.h>

#include "modengine/base/scyllahide_inject.h"

namespace modengine::base {

typedef void(__cdecl * LOGWRAPPER)(const wchar_t *msg);
typedef void (__cdecl *fpScyllaHideInit)(const WCHAR* Directory, LOGWRAPPER Logger, LOGWRAPPER ErrorLogger);
typedef void (__cdecl *fpScyllaHideDebugLoop)(DEBUG_EVENT* DebugEvent);

#if 0
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
    logger->info("ScyllaHide injection successful.");
    return true;
}
#endif

// Use external scyllahide process to inject if available
bool inject_scyllahide_external()
{
    using std::filesystem::path;
    WCHAR exe_path[MAX_PATH];

    // Check for scylla hide in game directory
    if (GetFileAttributesW(L".\\scyllahide\\InjectorCLIx64.exe") == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    spdlog::info("Found valid ScyllaHide injector. Injecting into game.");

    auto exe_path_len = GetModuleFileNameW(nullptr, exe_path, MAX_PATH);

    if (exe_path_len == 0) {
        throw std::runtime_error("Unable to determine EXE file of running game");
    }
    const auto exe_name = path(std::wstring(exe_path)).filename().wstring();
    // Using heap memory because CreateProcessW seems to not like stack memory for the command line string?
    WCHAR *command_line = new WCHAR[MAX_PATH * 2];
    if (swprintf_s(command_line, MAX_PATH, L".\\InjectorCLIx64.exe %s .\\HookLibraryx64.dll nowait", exe_name.c_str()) < 0) {
        delete[] command_line;
        return false;
    }

    spdlog::info(L"Launching process {}", command_line);
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    bool result = CreateProcessW(
        L".\\scyllahide\\InjectorCLIx64.exe",
        command_line,
        NULL, NULL, FALSE, 0, NULL,
        L".\\scyllahide",
        &si, &pi
    );
    delete[] command_line;
    if (result) {
        spdlog::info("Launched Scyllahide process.");

        // Wait for it to finish
        DWORD res = WaitForSingleObject(pi.hProcess, 2000);
        if (res == WAIT_TIMEOUT) {
            spdlog::info("Scyllahide process timed out.");
        }
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return (res != WAIT_TIMEOUT);
    }
    return false;
}

}