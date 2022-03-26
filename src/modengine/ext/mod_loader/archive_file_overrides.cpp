#include <stdio.h>
#include <wchar.h>
#include <algorithm>
#include <filesystem>
#include <string>
#include <string_view>
#include <optional>
#include <regex>
#include <concurrent_unordered_set.h>
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>

#include <spdlog/spdlog.h>

#include "gametypes/dantelion/dlstring.h"
#include "archive_file_overrides.h"

namespace modengine::ext {

// This hook is designed to enable file mods by intercepting internal file load requests from archives,
// detecting if there's a modded version of that file, and telling the game to load the on-disk modded file
// instead of file from the archive.
//
// The game internally uses a multi-level virtual file system to reference assets. For example, the game could
// reference a map asset by requesting map://m30_00_00_00/someasset.mapbnd.dcx. The game has an alias table to
// remap these internal asset URLs to something that references an archive. For example, this url would be remapped
// to data5://map/m30_00_00_00/someasset.mapbnd.dcx. Since this asset references an archive, it will be hashed and
// then looked up in the data5 archive dictionary. However, the game supports loading assets directly from disk if
// a game directory relative path is specified. For example, if you replace data5://map/m30_00_00_00/someasset.mapbnd.dcx
// with .///////map/m30_00_00_00/someasset.mapbnd.dcx, it will attempt to load this asset directly from disc. Therefore,
// our modded file injection method consists of two steps.
//
// The first step is to intercept archive paths and replace them with file system paths if a modded file is detected.
// To do this, we hook the function responsible to translate internal virtual file system paths into archive paths. Once
// we let this function do its thing, we can then do a "post-processing" where we detect if a modded file for this file
// exists in the mod directory, and if so we will modify the path inline, replacing the "data5://" with the ".///////".
// Using this method means we don't have to worry about reallocating the string or messing with game allocated memory.
//
// Once we replace this string, the game will attempt to load the asset from the base game directory. Since we want where
// the modded files are located to be configurable, we need to modify the path from being game directory relative to "mod
// directory" relative. There's various ways to do this, but I found simply hooking CreateFileW to do the remapping to be
// the easiest and most robust way to do this. It also has the bonus of allowing files that don't go through the asset system
// (like the data archives and some of the sounds) to be overridable.

namespace fs = std::filesystem;

concurrency::concurrent_unordered_map<std::wstring, std::optional<std::filesystem::path>> archive_override_paths;
concurrency::concurrent_unordered_map<std::wstring, std::optional<std::filesystem::path>> file_override_paths;

Hook<fpCreateFileW> hooked_CreateFileW;

Hook<decltype(&virtual_to_archive_path_ds3)> hooked_virtual_to_archive_path_ds3;
Hook<decltype(&virtual_to_archive_path_ds2)> hooked_virtual_to_archive_path_ds2;
Hook<decltype(&virtual_to_archive_path_sekiro)> hooked_virtual_to_archive_path_sekiro;
ScannedHook<decltype(&virtual_to_archive_path_eldenring)> hooked_virtual_to_archive_path_eldenring;
concurrency::concurrent_vector<std::wstring> hooked_file_roots;

using namespace spdlog;


std::optional<fs::path> find_override_file(const fs::path& game_path)
{
    for (const auto& root : hooked_file_roots) {
        trace(L"Searching for {} in {}", game_path.wstring(), root);

        auto file_path = root / fs::path(game_path);
        if (fs::exists(file_path)) {
            return file_path;
        }
    }

    return {};
}

/* Check if the given path is an archive reference and replaces
 i.e. "data1:/some/path" with ".//////some/path" which has the
 effect of causing the game to load a file from the local file
 system instead of an archive.*/
void process_archive_path(wchar_t* raw_path, size_t raw_path_len)
{
    /* clang-format off */
    static auto archive_file_pattern = std::wregex(
        L"("
        "data.:\\/"   // References to main data archives (data1:/ etc)
        "|gamedata."  // References to DS2 data archives
        "|game_....." // References to DS3 DLC archives
        ").+");
    /* clang-format on */

    std::wstring path(raw_path, raw_path_len);
    debug(L"Checking archive path {}", path);

    std::wsmatch matches;
    std::regex_match(path, matches, archive_file_pattern);

    if (matches.empty()) {
        return;
    }

    const auto prefix_len = matches[1].length();
    const auto archive_file = L"./" + path.substr(prefix_len);
    const auto archive_file_path = fs::path(archive_file).lexically_normal();

    const auto& local_file_key = std::wstring(archive_file_path.native());
    const auto& local_override = archive_override_paths.find(local_file_key);

    fs::path override_path;

    if (local_override != archive_override_paths.end() && !local_override->second.has_value()) {
        return;
    } else if (local_override == archive_override_paths.end()) {
        auto override = find_override_file(archive_file_path);

        if (!override.has_value()) {
            return;
        }

        override_path = override.value();
        archive_override_paths[local_file_key] = override_path;

        debug(L"Found override in {}", override_path.native());
    }

    raw_path[0] = L'.';

    for (auto i = 1; i < prefix_len && i < raw_path_len; i++) {
        raw_path[i] = '/';
    }
}

bool path_contains(const fs::path& root, const fs::path& filepath)
{
    auto dir_len = std::distance(root.begin(), root.end());
    auto file_len = std::distance(filepath.begin(), filepath.end());

    if (dir_len >= file_len) {
        return false;
    }

    return std::equal(root.begin(), root.end(), filepath.begin(), [](fs::path a, fs::path b) {
        return a.native().length() == b.native().length() && _wcsicmp(a.native().c_str(), b.native().c_str()) == 0;
    });
}

HANDLE WINAPI tCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                           LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                           HANDLE hTemplateFile)
{
    if (lpFileName != nullptr) {
        trace(L"Looking for {}", std::wstring(lpFileName));

        auto override_file_opt = file_override_paths.find(std::wstring(lpFileName));
        std::optional<fs::path> override_path = {};
        if (override_file_opt != file_override_paths.end()) {
            override_path = override_file_opt->second;
        } else {
            auto root = fs::current_path();
            auto filepath = fs::path(lpFileName);

            if (path_contains(root, filepath)) {
                auto relpath = fs::path(filepath.wstring().substr(root.wstring().length() + 1));
                override_path = find_override_file(relpath);
            }

            file_override_paths[std::wstring(lpFileName)] = override_path;
        }

        if (override_path) {
            const auto override = override_path->native().c_str();
            trace(L"Loading overriden path {}", override);

            for (int i = 0; i < 10; i++) {
                HANDLE res = hooked_CreateFileW.original(
                    override,
                    dwDesiredAccess,
                    dwShareMode,
                    lpSecurityAttributes,
                    dwCreationDisposition,
                    dwFlagsAndAttributes,
                    hTemplateFile);

                if (res != INVALID_HANDLE_VALUE) {
                    return res;
                }

                warn(L"Failed to load file {}", override);
            }
        }
    }

    return hooked_CreateFileW.original(
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile);
}

void* __cdecl virtual_to_archive_path_ds3(DLString<modengine::GameType::DS3, wchar_t>* path, UINT64 p2, UINT64 p3, DLString<modengine::GameType::DS3, wchar_t>* p4, UINT64 p5, UINT64 p6)
{
    auto res = static_cast<DLString<modengine::GameType::DS3, wchar_t>*>(hooked_virtual_to_archive_path_ds3.original(path, p2, p3, p4, p5, p6));

    if (res != nullptr) {
        process_archive_path(res->str(), res->length);
    }

    return static_cast<void*>(res);
}

void* __cdecl virtual_to_archive_path_ds2(LPVOID p1, dlstring_t* path)
{
    return hooked_virtual_to_archive_path_ds2.original(p1, path);
}

void* __cdecl virtual_to_archive_path_sekiro(DLString<modengine::GameType::SEKIRO, wchar_t>* path, UINT64 p2, UINT64 p3, DLString<modengine::GameType::SEKIRO, wchar_t>* p4, UINT64 p5, UINT64 p6)
{
    auto res = static_cast<DLString<modengine::GameType::SEKIRO, wchar_t>*>(hooked_virtual_to_archive_path_sekiro.original(path, p2, p3, p4, p5, p6));

     if (res != nullptr) {
        process_archive_path(res->str(), res->length);
    }

    return static_cast<void*>(res);
}

void* __cdecl virtual_to_archive_path_eldenring(DLString<modengine::GameType::SEKIRO, wchar_t>* path, UINT64 p2, UINT64 p3, UINT64 p4, UINT64 p5, UINT64 p6)
{
    auto res = hooked_virtual_to_archive_path_eldenring.original(path, p2, p3, p4, p5, p6);

     if (path != nullptr) {
        process_archive_path(path->str(), path->length);
    }

    return res;
}

}