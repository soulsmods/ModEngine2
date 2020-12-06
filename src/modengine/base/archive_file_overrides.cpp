#include <stdio.h>
#include <wchar.h>
#include <algorithm>
#include <string>
#include <string_view>
#include <optional>
#include <concurrent_unordered_set.h>

#include <spdlog/spdlog.h>

#include "modengine/base/archive_file_overrides.h"

namespace modengine::base {

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

// Cached set of files that have an override file available
concurrency::concurrent_unordered_set<std::wstring> override_set;

// Cached set of files that don't have an override and should be loaded from archives
concurrency::concurrent_unordered_set<std::wstring> archive_set;

// TODO: replace these with settings once we have a good settings system
std::wstring g_mod_dir = std::wstring(L"collisiontest");
bool g_load_uxm_files = false;
bool g_use_mod_override = true;
bool g_cache_paths = true;

// Hooked functions
std::shared_ptr<Hook<fpCreateFileW>> hooked_CreateFileW;
std::shared_ptr<Hook<fpVirtualToArchivePathDS3>> hooked_virtual_to_archive_path_ds3;
std::shared_ptr<Hook<fpVirtualToArchivePathDS2>> hooked_virtual_to_archive_path_ds2;
std::shared_ptr<Hook<fpVirtualToArchivePathSekiro>> hooked_virtual_to_archive_path_sekiro;

inline std::optional<std::wstring> find_override_file(const std::wstring mod_dir, const std::wstring full_path, const std::wstring game_path)
{
    std::wstring search_path = full_path + L"\\" + mod_dir + game_path;
    if (GetFileAttributesW(search_path.data()) != INVALID_FILE_ATTRIBUTES) {
        return search_path;
    }
    return {};
}

bool has_override_file(std::wstring_view path)
{
    spdlog::info(L"[ArchiveOverrides] Intercepted archive path {}", path);

    bool is_overridden = g_cache_paths ? (override_set.find(std::wstring(path)) != override_set.end()) : false;
    bool is_archived = g_cache_paths ? (archive_set.find(std::wstring(path)) != archive_set.end()) : false;

    // Not cached
    if (!is_overridden && !is_archived) {
        wchar_t working[_MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, working);
        std::wstring dir = std::wstring(working);

        // If we're using a mod override and not using UXM we need to search inside the specified mod directory
        if (g_use_mod_override && !g_load_uxm_files) {
            dir = dir + L"\\" + g_mod_dir;
        }
        // If we're not using mod overrides and not loading UXM files then don't override at all
        else if (!g_load_uxm_files) {
            return false;
        }

        // Add the game file path and replace the slashes
        dir += path;
        std::replace(dir.begin(), dir.end(), L'/', L'\\');

        spdlog::info(L"[ArchiveOverrides] Looking for override file {}", dir);

        // Search if an override file exists for the current mod
        if (GetFileAttributesW(dir.data()) != INVALID_FILE_ATTRIBUTES) {
            if (g_cache_paths) {
                override_set.insert(std::wstring(path));
            }
            spdlog::info(L"[ArchiveOverrides] Adding override for {}", dir);
            return true;
        } else {
            // If the override file doesn't exist, cache it if caching is enabled
            if (g_cache_paths) {
                archive_set.insert(std::wstring(path));
            }
        }
    }

    return is_overridden;
}

// Replaces i.e. "data1:/some/path" with ".//////some/path" which has the effect of causing the game to load a file from the file system instead
// of an archive
inline void archive_to_file_path(wchar_t* archive_path, int count)
{
    archive_path[0] = L'.';
    for (int i = 1; i < count; i++) {
        archive_path[i] = L'/';
    }
}

void process_archive_path(std::wstring_view archive_path, wchar_t* rawpath)
{
    bool override = false;
    // References to main data archives (data1:/ etc)
    if (archive_path.rfind(L"data", 0) == 0 && archive_path.size() >= 6) {
        override = has_override_file(archive_path.substr(6));
        if (override) {
            archive_to_file_path(rawpath, 6);
        }
    }
    // References to DS2 data archives
    if (archive_path.rfind(L"gamedata", 0) == 0 && archive_path.size() >= 9) {
        override = has_override_file(archive_path.substr(9));
        if (override) {
            archive_to_file_path(rawpath, 9);
        }
    }
    // References to DS3 DLC archives
    if (archive_path.rfind(L"game_", 10) == 0 && archive_path.size() >= 10) {
        override = has_override_file(archive_path.substr(10));
        if (override) {
            archive_to_file_path(rawpath, 10);
        }
    }
}

HANDLE WINAPI tCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    if (g_use_mod_override && lpFileName != nullptr) {
        wchar_t gp[MAX_PATH + 40];
        GetCurrentDirectoryW(MAX_PATH, gp);
        auto gamepath = std::wstring(gp);
        auto filename = std::wstring(lpFileName);
        // Case insensitive compare since DS3 gives lower case file paths for some reason. Hopefully doesn't break
        // on non-English systems. Here we determine if a referenced file is game data by checking if the file is
        // inside the game directory
        bool game_relative_path = std::equal(gamepath.begin(), gamepath.end(), filename.begin(), filename.end(),
            [](wchar_t a, wchar_t b) {
                return towlower(a) == towlower(b);
            });

        if (game_relative_path && filename.length() > gamepath.length()) {
            auto relpath = filename.substr(gamepath.length());
            if (relpath.find(L"\\") != -1) {
                // We likely have a game directory path. Attempt to find override files
                spdlog::info(L"[FileOverrides] Intercepted game file path {}", filename);
                auto orpath = find_override_file(g_mod_dir, std::wstring(gamepath), std::wstring(relpath));
                if (orpath.has_value()) {
                    spdlog::info(L"[FileOverrides] Overriding with {}", orpath.value());
                    // Do 10 attempts at loading the override file since we know it exists
                    for (int i = 0; i < 10; i++) {
                        HANDLE res = hooked_CreateFileW->original(orpath.value().c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes,
                            dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
                        if (res != INVALID_HANDLE_VALUE) {
                            return res;
                        }
                        spdlog::warn(L"Failed to load file {}", orpath.value());
                    }
                }
            }
        }
    }
    return hooked_CreateFileW->original(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
        dwFlagsAndAttributes, hTemplateFile);
}

void* __cdecl virtual_to_archive_path_ds3(dlstring_t* path, UINT64 p2, UINT64 p3, dlstring_t* p4, UINT64 p5, UINT64 p6)
{
    dlstring_t* res = static_cast<dlstring_t*>(hooked_virtual_to_archive_path_ds3->original(path, p2, p3, p4, p5, p6));
    if (res != nullptr && res->capacity > 7) {
        process_archive_path(std::wstring_view(res->string, res->length), res->string);
    }
    return static_cast<void*>(res);
}

void* __cdecl virtual_to_archive_path_ds2(LPVOID p1, dlstring_t* path)
{
    return hooked_virtual_to_archive_path_ds2->original(p1, path);
}

void* __cdecl virtual_to_archive_path_sekiro(dlstring_sekiro_t* path, UINT64 p2, UINT64 p3, dlstring_sekiro_t* p4, UINT64 p5, UINT64 p6)
{
    return hooked_virtual_to_archive_path_sekiro->original(path, p2, p3, p4, p5, p6);
}

}