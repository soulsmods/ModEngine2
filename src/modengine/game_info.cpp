#include "modengine/game_info.h"

#include <unordered_map>
#include <optional>
#include <filesystem>

#include <windows.h>

namespace modengine {

static std::unordered_map<std::wstring, GameType> game_type_executables {
    { L"DarkSoulsRemastered.exe", DS_REMASTERED },
    { L"DarkSoulsII.exe", DS2 },
    { L"DarkSoulsIII.exe", DS3 },
    { L"sekiro.exe", SEKIRO }
};

static std::unordered_map<GameType, std::string> game_type_descriptions {
    { DS_REMASTERED, "Dark Souls Remastered" },
    { DS2, "Dark Souls 2: Scholar of The First Sin" },
    { DS3, "Dark Souls 3" },
    { SEKIRO, "Sekiro" }
};

static std::optional<GameType> game_type_for_exe(const std::wstring& exe_name)
{
    auto ty = game_type_executables.find(exe_name);
    if (ty != game_type_executables.end()) {
        return ty->second;
    }

    return {};
}

using std::filesystem::path;

std::optional<GameInfo> GameInfo::from_current_module()
{
    WCHAR exe_path[MAX_PATH];
    auto exe_path_len = GetModuleFileNameW(nullptr, exe_path, MAX_PATH);

    if (exe_path_len == 0) {
        throw std::runtime_error("Unable to determine EXE file of running game");
    }

    const auto exe_name = path(std::wstring(exe_path)).filename().wstring();
    const auto ty = game_type_for_exe(exe_name);

    if (!ty) {
        return {};
    }

    return GameInfo { *ty, /* todo: gtierney, get this from file resources */ "UNKNOWN" };
}

std::string& GameInfo::description() const
{
    return game_type_descriptions[type];
}



}
