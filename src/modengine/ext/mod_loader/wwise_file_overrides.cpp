#include <filesystem>

#include "wwise_file_overrides.h"
#include "archive_file_overrides.h"

namespace modengine::ext {

// This hook the IAkFileLocationResolver::open() method. It takes in a path 
// and a so-called openMode. This openMode parameter is of type AkOpenMode 
// which is an enum with 4 states: Read, Write, WriteOverwrite and ReadWrite.
// Passing in any of these 4 invariants will cause this function to yield a 
// FileOperator used for sourcing the file bytes. FromSoftware added another 
// possible state to AKOpenMode, decimal 9. which will make this fn yield an 
// EBLFileOperator, this implementation does all its fetching from the BDTs.
// EBLFileOperator does not use the usual virtual path lookup that ME2 already 
// hooks. Hence we need this hook here.
//
// In order to selectively make it read from disk again this hook checks if an 
// override file exists and sets the openMode back from 9 to Read and replaces
// the virtual path parameter string with an absolute path.
//
// This is a messy one though:
// Figuring out if there is an override isn't straight forward. The hooked 
// function gets invoked with a virtual path string ex: `sd:/50846376.wem`. 
// Wwise uses subdirectories for localized content, meaning that the "simple"
// WEM example makes Wwise look in `/50846376.wem` but also in 
// `enus/50846376.wem` (or `ja/50846376.wem` for AC6 which has `ja` as an extra 
// locale for some audio and BNKs). To make matters even worse, Elden Ring 
// specifically sorts the WEMs into a subdir (`wem/`) and then another subdir 
// based on the  first two digits of the WEM. So above example will spawn 
// lookups in  `/wem/50/50846376.wem` and `enus/wem/50/50846376.wem`. In order 
// to figure out if there is an override we will need to look in multiple 
// directories per request. Luckily, aside from boot, this routine is called
// quite infrequently.
//
// Also, worth pointing out that not all paths passed to this hook will have 
// the `sd:/` prefix. So we cannot get away with the usual prefix / rewrite 
// trick and will have to allocate a completely new string.

namespace fs = std::filesystem;

using namespace spdlog;

std::optional<fs::path> find_override_file(const fs::path game_path)
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

const wchar_t* prefixes[3] = {
    L"sd/",
    L"sd/enus/",
    L"sd/ja/",
};

std::optional<fs::path> check_paths(const std::wstring filename) {
    for (auto prefix: prefixes) {
        if (auto override = find_override_file(prefix+filename)) {
            return override;
        }
    }

    return {};
}

std::optional<fs::path> find_override(const std::wstring filename)
{
    // Check wem/<first to digits of filename>/<filename> too since ER uses 
    // this format
    if (filename.ends_with(L".wem")) {
        auto wem_path = L"wem/" + filename.substr(0,2) + L"/" + filename;
        auto wem_path_result = check_paths(wem_path);

        if (wem_path_result.has_value()) {
            return wem_path_result;
        }
    }

    return check_paths(filename);
}

std::optional<std::wstring> normalize_filename(const std::wstring path) {
    if (path.starts_with(L"sd:/")) {
        return std::wstring(path.substr(4));
    }

    return {};
}

ScannedHook<decltype(&ak_file_location_resolver_open)> hooked_ak_file_location_resolver_open;

void* __cdecl ak_file_location_resolver_open(UINT64 p1, wchar_t* path, AKOpenMode openMode, UINT64 p4, UINT64 p5, UINT64 p6)
{
    std::wstring lookup(path);
    debug(L"sd.bhd entry requested: {}", lookup);

    auto normalized = normalize_filename(lookup);
    if (!normalized.has_value()) {
        return hooked_ak_file_location_resolver_open.original(p1, path, openMode, p4, p5, p6);
    }

    auto filename = normalized.value();
    auto override = find_override(filename);
    if (!override.has_value()) {
        return hooked_ak_file_location_resolver_open.original(p1, path, openMode, p4, p5, p6);
    }

    auto override_path_string = override.value().wstring();
    return hooked_ak_file_location_resolver_open.original(p1, override_path_string.data(), AKOpenMode::READ, p4, p5, p6);
}

}
