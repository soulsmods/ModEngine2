#include <filesystem>

#include "wwise_file_overrides.h"
#include "archive_file_overrides.h"

namespace modengine::ext {

// This hook the IAkFileLocationResolver::open() method. It takes in a path and a so-called openMode. This openMode parameter
// is of type AkOpenMode which is an enum with 4 states: Read, Write, WriteOverwrite and ReadWrite. Based on this parameter
// the IAkFileLocationResolver implementation might acquire the file bytes different. Passing in any of these 4 invariants
// will cause this function to yield an FileOperator used for sourcing the file bytes. FromSoftware funnily enough added
// another invariant to AKOpenMode, dec 9, which will instead yield an EBLFileOperator, causing the reads to happen from
// the bhd/bdt. This unfortunately happens without touching the usual virtual path lookup which ME2 already hooks.
// In order to selectively make it read from disk again this checks if an override file exists and sets the openMode
// back from 9 to Read. Then an absolute path must be passed as a parameter.
//
// This is a messy one though:
// Figuring out if there is an override isn't straight forward. The hooked function gets invoked with a virtual path string
// ex: `sd:/50846376.wem`. Wwise uses subdirectories for localized content, meaning that the WEM example makes Wwise look
// in `/50846376.wem` but also in `enus/50846376.wem` (or `ja/50846376.wem` for AC6 which has `ja` as an extra locale
// for some audio and BNKs). To make matters even worse, Elden Ring sorts the WEMs into a subdir (`wem/`) and then
// another subdir based on the first two digits of the WEM. So above example will spawn lookups in `/wem/50/50846376.wem`
// and `enus/wem/50/50846376.wem`. In order to figure out if there is an override we will need to look in multiple directories
// per request. Luckily, aside from boot, it doesn't load a lot of files.
//
// Also, worth pointing out that not all paths passed to this hook will have the `sd:/` prefix. So we cannot get away with the
// usual prefix / rewrite trick and will have to allocate a completely new string.

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

std::optional<fs::path> find_override(const std::wstring filename)
{
    for (auto prefix: prefixes) {
        if (auto override = find_override_file(prefix+filename)) {
            return override;
        }
    }

    return {};
}

std::optional<std::wstring> normalize_filename(const std::wstring path) {
    if (path.length() > 3 && path.substr(0, 4) == L"sd:/") {
        return std::wstring(path.substr(4));
    }

    return {};
}

ScannedHook<decltype(&ak_file_location_resolver_open)> hooked_ak_file_location_resolver_open;

void* __cdecl ak_file_location_resolver_open(UINT64 p1, wchar_t* path, UINT64 openMode, UINT64 p4, UINT64 p5, UINT64 p6)
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
    return hooked_ak_file_location_resolver_open.original(p1, override_path_string.data(), 0, p4, p5, p6);
}

}