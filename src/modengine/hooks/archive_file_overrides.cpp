#include <stdio.h>
#include <wchar.h>
#include <algorithm>
#include <string>
#include <string_view>
#include <optional>
#include <concurrent_unordered_set.h>

// Cached set of files that have an override file available
concurrency::concurrent_unordered_set<std::wstring> override_set;

// Cached set of files that don't have an override and should be loaded from archives
concurrency::concurrent_unordered_set<std::wstring> archive_set;

// TODO: replace these with settings once we have a good settings system
std::wstring g_mod_dir;
bool g_load_uxm_files = false;
bool g_use_mod_override = false;
bool g_cache_paths = false;

inline std::optional<std::wstring> find_override_file(const std::wstring mod_dir, const std::wstring full_path, const std::wstring game_path)
{
    std::wstring search_path = full_path + mod_dir + game_path;
    if (GetFileAttributesW(search_path.data()) != INVALID_FILE_ATTRIBUTES) {
        return search_path;
    }
    return {};
}

bool has_override_file(std::wstring_view path)
{
    // TODO: log archive file interception

    bool is_overridden = g_cache_paths ? (override_set.find(path.data()) != override_set.end()) : false;
    bool is_archived = g_cache_paths ? (archive_set.find(path.data()) != archive_set.end()) : false;

    // Not cached
    if (!is_overridden && !is_archived) {
        wchar_t working[_MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, working);
        std::wstring dir = std::wstring(working);

        // If we're using a mod override and not using UXM we need to search inside the specified mod directory
        if (g_use_mod_override && !g_load_uxm_files) {
            if (!g_mod_dir.rfind(L"\\", 0)) {
                dir = dir + L"\\";
            }
            dir = dir + g_mod_dir;
        }

        // Add the game file path and replace the slashes
        dir += path;
        std::replace(dir.begin(), dir.end(), L'/', L'\\');

        // TODO: Log first time looking for file

        // Search if an override file exists for the current mod
        if (GetFileAttributesW(dir.data()) != INVALID_FILE_ATTRIBUTES) {
            if (g_cache_paths) {
                override_set.insert(std::wstring(path));
            }
            // TODO: Log successful override
            return true;
        }
        else {
            // If the override file doesn't exist, cache it if caching is enabled
            if (g_cache_paths) {
                archive_set.insert(std::wstring(path));
            }
        }
    }

    return false;
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