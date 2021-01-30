#include "scyllahide_extension.h"

#include <filesystem>

namespace modengine::ext {

using namespace spdlog;

bool inject_scyllahide_external()
{
    using std::filesystem::exists;

    if (!exists(L"./scyllahide/InjectorCLIx64.exe")) {
        return false;
    }

    info("Found valid ScyllaHide injector. Injecting into game.");

    const auto pid = GetCurrentProcessId();

    WCHAR command_line[MAX_PATH];
    if (swprintf_s(command_line, MAX_PATH, L"./scyllahide/InjectorCLIx64.exe pid:%d ./HookLibraryx64.dll nowait", pid) < 0) {
        return false;
    }

    info(L"Starting ScyllaHide injector: {}", command_line);

    STARTUPINFOW si = {};
    PROCESS_INFORMATION pi = {};

    bool result = CreateProcessW(
        L"./scyllahide/InjectorCLIx64.exe",
        command_line,
        nullptr,
        nullptr,
        false,
        0,
        nullptr,
        L"./scyllahide",
        &si,
        &pi);

    if (result) {
        spdlog::info("Launched Scyllahide process.");

        // Wait for it to finish
        DWORD res = WaitForSingleObject(pi.hProcess, 2000);
        if (res == WAIT_TIMEOUT) {
            warn("Scyllahide process timed out.");
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return (res != WAIT_TIMEOUT);
    }

    return false;
}

void ScyllaHideExtension::on_attach()
{
    if (inject_scyllahide_external()) {
        info("ScyllaHide successfully injected");
    } else {
        warn("Unable to inject ScyllaHide hook");
    }
}

void ScyllaHideExtension::on_detach()
{
}

}