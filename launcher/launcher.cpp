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
    E_CREATE_PROCESS_FAILED = -4,
};

struct LaunchTargetParams {
    std::wstring app_id;
    std::wstring executable_path;
    std::wstring default_config;
};

enum LaunchTarget {
    AUTODETECT,
    DS3,
    ELDEN_RING,
    ARMORED_CORE_6,
};

static std::map<LaunchTarget, LaunchTargetParams> launch_targets {
    { DS3, { L"374320", L"Game/DarkSoulsIII.exe", L"config_darksouls3.toml" } },
    { ELDEN_RING, { L"1245620", L"Game/eldenring.exe", L"config_eldenring.toml" } },
    { ARMORED_CORE_6, { L"1888160", L"Game/armoredcore6.exe", L"config_armoredcore6.toml" } }
};

static std::map<std::string, LaunchTarget> launch_target_names {
    { "ds3", DS3 },
    { "er", ELDEN_RING },
    { "ac6", ARMORED_CORE_6 },
};

static std::map<std::string, LaunchTarget> exe_names {
    { "DarkSoulsIII.exe", DS3 },
    { "eldenring.exe", ELDEN_RING },
    { "armoredcore6.exe", ARMORED_CORE_6 },
};

namespace platform {
std::wstring get_env_var(const std::wstring& name)
{
    size_t buffer_size = GetEnvironmentVariableW(name.c_str(), nullptr, 0);
    auto* buffer = new wchar_t[buffer_size + 1];

    std::wstring value;

    if (buffer_size > 0) {
        size_t len = GetEnvironmentVariableW(name.c_str(), &buffer[0], buffer_size + 1);
        value.append(buffer, len);
    }

    delete buffer;

    return value;
}

void set_env_var(const std::wstring& name, const std::wstring& value)
{
    SetEnvironmentVariableW(name.c_str(), value.c_str());
}

fs::path get_launcher_directory()
{
    size_t buffer_size = GetModuleFileNameW(nullptr, nullptr, 0);
    auto* buffer = new wchar_t[buffer_size + 1];

    fs::path path = fs::current_path();

    if (buffer_size > 0) {
        size_t len = GetModuleFileNameW(nullptr, buffer, buffer_size + 1);
        fs::path launcher_path(std::wstring_view { buffer, len });

        path = launcher_path.parent_path();
    }

    delete buffer;

    return path;
}

}

int main()
{
    auto logger = spdlog::stderr_color_mt("stderr");
    auto launcher_directory = platform::get_launcher_directory();


    CLI::App app { "ModEngine Launcher" };

    LaunchTarget target = AUTODETECT;
    auto target_option = app.add_option("-t,--launch-target", target, "Launch target")
                             ->transform(CLI::CheckedTransformer(launch_target_names, CLI::ignore_case));

    std::string target_path_string;
    auto target_path_option = app.add_option("-p,--game-path", target_path_string, "Path to game executable. Will autodetect if not specified.")
                                  ->transform(CLI::ExistingFile);

    std::string config_path_string;
    auto config_option = app.add_option("-c,--config", config_path_string, "ModEngine configuration file path")
                             ->transform(CLI::ExistingFile);

    bool suspend = false;
    app.add_option("-s,--suspend", suspend, "Start the game in a suspended state");

    auto modengine_dll_path = launcher_directory / L"modengine2" / L"bin" / L"modengine2.dll";

    app.add_option("--modengine-dll", modengine_dll_path, "ModEngine DLL file path (modengine2.dll)");

    try {
        app.parse();
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    std::optional<fs::path> app_path = std::nullopt;

    // First if the game path was specified, use that along with the specified target
    if (!target_path_option->empty()) {
        app_path = absolute(CLI::to_path(target_path_string)).parent_path().parent_path();
        if (target == AUTODETECT) {
            logger->error("Game target must be specified when supplying a manual path");
            return E_APP_NOT_FOUND;
        }
    }

    // If the game target was not set, try to find a game exe in the current directory and infer from that
    if (target_option->empty()) {
        for (auto& name_kv : exe_names) {
            auto exepath = launcher_directory / name_kv.first;
            if (fs::exists(exepath)) {
                target = name_kv.second;
                app_path = exepath.parent_path().parent_path(); // app_path is expected to be steam app path not exe path
            }
        }
    }

    // If we still didn't detect the game, error and quit
    if (target == AUTODETECT) {
        logger->error("Couldn't find path to game");
        return E_APP_NOT_FOUND;
    }

    auto launch_params = launch_targets[target];

    // If a config wasn't specified, try to load the default one for the game
    if (config_option->empty()) {
        auto default_config_path = launcher_directory / launch_params.default_config;
        if (!fs::exists(default_config_path)) {
            logger->error("Could not find default config file at {}", default_config_path.string());
        }
        config_path_string = default_config_path.string();
    }

    // If path wasn't already set from detecting the game exe in the launcher directory, use the Steam DB to lookup game path
    if (!app_path) {
        app_path = get_game_path(launch_params.app_id);
    }

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

    auto exec_path = platform::get_env_var(L"PATH");
    exec_path.append(L";");
    exec_path.append(modengine_dll_path.parent_path().native());

    auto config_path = CLI::to_path(config_path_string);
    if (config_path.is_relative()) {
        const auto search_path = launcher_directory / config_path;
        config_path = fs::absolute(search_path);
    }

    // These are inherited by the game process we launch with Detours.
    platform::set_env_var(L"SteamAppId", launch_params.app_id);
    platform::set_env_var(L"MODENGINE_CONFIG", config_path.native());
    platform::set_env_var(L"PATH", exec_path);

    if (suspend || IsDebuggerPresent()) {
        platform::set_env_var(L"MODENGINE_DEBUG_GAME", L"1");
    }

    std::wstring cmd_str = app_cmd.native();
    size_t cmd_len = cmd_str.length();

    auto *cmd = new wchar_t[cmd_len + 1];
    cmd[cmd_len] = 0;

    wcscpy_s(cmd, cmd_len, cmd_str.c_str());

    auto proc_flags = 0;
    bool success = DetourCreateProcessWithDllW(
        cmd,
        nullptr,
        nullptr,
        nullptr,
        false,
        proc_flags,
        nullptr,
        app_cwd.c_str(),
        &si,
        &pi,
        fs::absolute(modengine_dll_path).native().c_str(),
        reinterpret_cast<PDETOUR_CREATE_PROCESS_ROUTINEW>(create_process_addr));

    auto status = E_OK;

    if (!success) {
        logger->error("Couldn't create process: {:x}", GetLastError());
        status = E_CREATE_PROCESS_FAILED;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return status;
}