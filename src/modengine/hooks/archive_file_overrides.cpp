#include <stdio.h>
#include <wchar.h>
#include <Shlwapi.h>
#include <string>
#include <string_view>
#include <optional>
#include <concurrent_unordered_set.h>

// Cached set of files that have an override file available
concurrency::concurrent_unordered_set<std::wstring> overrideSet;

// Cached set of files that don't have an override and should be loaded from archives
concurrency::concurrent_unordered_set<std::wstring> archiveSet;

inline std::optional<std::wstring> find_override_file(const std::wstring mod_dir, const std::wstring full_path, const std::wstring game_path)
{
    std::wstring search_path = full_path + mod_dir + game_path;
	if (GetFileAttributesW(search_path.data()) != INVALID_FILE_ATTRIBUTES)
	{
		return search_path;
	}
	return {};
}

bool has_override_file(std::wstring_view path)
{
    // Stub for now
    return false;
}

// Replaces i.e. "data1:/some/path" with ".//////some/path" which has the effect of causing the game to load a file from the file system instead
// of an archive
inline void archive_to_file_path(wchar_t *archive_path, int count)
{
    archive_path[0] = L'.';
    for (int i = 1; i < count; i++)
    {
        archive_path[i] = L'/';
    }
}

void process_archive_path(std::wstring_view archive_path, wchar_t *rawpath)
{
    bool override = false;
    // References to main data archives (data1:/ etc)
    if (archive_path.rfind(L"data", 0) == 0 && archive_path.size() >= 6)
    {
        override = has_override_file(archive_path.substr(6));
        if (override)
        {
            archive_to_file_path(rawpath, 6);
        }
    }
    // References to DS2 data archives
    if (archive_path.rfind(L"gamedata", 0) == 0 && archive_path.size() >= 9)
    {
        override = has_override_file(archive_path.substr(9));
        if (override)
        {
            archive_to_file_path(rawpath, 9);
        }
    }
    // References to DS3 DLC archives
    if (archive_path.rfind(L"game_", 10) == 0 && archive_path.size() >= 10)
    {
        override = has_override_file(archive_path.substr(10));
        if (override)
        {
            archive_to_file_path(rawpath, 10);
        }
    }
}