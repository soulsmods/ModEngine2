#define UNICODE 1

#include <windows.h>
#include <filesystem>
#include <map>

#include <detours/detours.h>
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "steam_app_path.h"

namespace fs = std::filesystem;

enum LauncherError : int {
    E_OK = 0,
    E_OS_ERROR = -1,
    E_APP_NOT_FOUND = -2,
    E_MODENGINE_NOT_FOUND = -3,
};

struct LaunchTargetParams {
    std::wstring app_id;
    std::wstring executable_path;
};

enum LaunchTarget {
    DS3,
};

static std::map<LaunchTarget, LaunchTargetParams> launch_targets {
    { DS3, { L"374320", L"Game/DarkSoulsIII.exe" } }
};

static std::map<std::string, LaunchTarget> launch_target_names {
    { "ds3", DS3 },
};

int main(int argc, char* argv[])
{
    auto logger = spdlog::stderr_color_mt("stderr");

    wchar_t launcher_filename[MAX_PATH];

    // This isn't always needed, but cli11 doesn't allow us to signal an error
    // from the function that produces the default value for the modengine.dll path.
    if (!GetModuleFileNameW(nullptr, launcher_filename, MAX_PATH)) {
        return E_OS_ERROR;
    }

    CLI::App app { "ModEngine Launcher" };

    LaunchTarget target;
    app.add_option("-t,--launch-target", target, "Launch target")
        ->required()
        ->transform(CLI::CheckedTransformer(launch_target_names, CLI::ignore_case));

    fs::path config_path;
    app.add_option("-c,--config", config_path, "ModEngine configuration file path")
        ->required()
        ->transform(CLI::ExistingFile);

    fs::path modengine_dll_path;
    app.add_option("--modengine-dll", modengine_dll_path, "ModEngine DLL file path (modengine2.dll)")
        ->default_function([&]() {
            auto launcher_path = fs::path(launcher_filename);
            auto modengine_dll_path = launcher_path.parent_path() / L"modengine2.dll";

            return modengine_dll_path.string();
        });

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    auto launch_params = launch_targets[target];
    auto app_path = get_game_path(launch_params.app_id);

    if (!app_path) {
        logger->error("Couldn't find path to game");
        return E_APP_NOT_FOUND;
    }

    auto app_cmd = *app_path / launch_params.executable_path;
    auto app_cwd = fs::absolute(app_cmd.parent_path());

    if (!fs::exists(modengine_dll_path)) {
        logger->error("Couldn't find path to modengine2.dll at {}", modengine_dll_path.string());
        return E_MODENGINE_NOT_FOUND;
    }

    STARTUPINFOW si = {};
    PROCESS_INFORMATION pi = {};

    auto kernel32 = LoadLibraryW(L"kernel32.dll");
    auto create_process_addr = GetProcAddress(kernel32, "CreateProcessW");

    // These are inherited by the game process we launch with Detours.
    SetEnvironmentVariable(L"SteamAppId", launch_params.app_id.c_str());
    SetEnvironmentVariable(L"MODENGINE_CONFIG", fs::absolute(config_path).c_str());

    wchar_t cmd[MAX_PATH] = {};
    wcscpy_s(cmd, app_cmd.c_str());

    bool success = DetourCreateProcessWithDllW(
        cmd,
        nullptr,
        nullptr,
        nullptr,
        false,
        CREATE_NEW_PROCESS_GROUP,
        nullptr,
        app_cwd.c_str(),
        &si,
        &pi,
        fs::absolute(modengine_dll_path).string().c_str(),
        reinterpret_cast<PDETOUR_CREATE_PROCESS_ROUTINEW>(create_process_addr));

    if (!success) {
        logger->error("Couldn't create process: {:x}", GetLastError());
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return E_OK;
}