#include "mod_loader_extension.h"
#include "archive_file_overrides.h"

#include "modengine/util/hex_string.h"

#include <spdlog/spdlog.h>

using namespace spdlog;
namespace fs = std::filesystem;

namespace modengine::ext {

auto loose_params_aob_1 = util::hex_string("74 68 48 8b cf 48 89 5c 24 30 e8 1c 6d 08");
auto loose_params_aob_2 = util::hex_string("0F 85 C5 00 00 00 48 8D 4C 24 28 E8 17 F4");
auto loose_params_aob_3 = util::hex_string("E8 78 08 F8 FF 90 E9 0E E9 08 05 53 E9 EF");

static fs::path primary_mod_path(const Settings& settings)
{
    return settings.modengine_local_path();
}

static std::vector<fs::path> secondary_mod_paths(const Settings& settings)
{
    return {
        settings.game_path(),
        settings.modengine_install_path(),
    };
}

std::optional<fs::path> ModLoaderExtension::resolve_mod_path(const ModInfo& mod)
{
    auto mod_path = fs::path(mod.location);
    if (mod_path.is_absolute()) {
        return mod_path;
    }

    const auto& settings = get_settings();
    const auto primary_search_path = primary_mod_path(settings);
    const auto primary_mod_path = primary_search_path / mod_path;

    if (fs::exists(primary_search_path / mod_path)) {
        return primary_mod_path;
    }

    const auto secondary_base_paths = secondary_mod_paths(settings);
    for (const auto& base_path : secondary_base_paths) {
        auto secondary_mod_path = base_path / mod_path;

        if (fs::exists(secondary_mod_path)) {
            return secondary_mod_path;
        }
    }

    return std::nullopt;
}

void ModLoaderExtension::on_attach()
{
    register_patch(DS3, loose_params_aob_1, replace_with<uint8_t>({ 0xEB, 0x68 }));
    register_patch(DS3, loose_params_aob_2, replace_with<uint8_t>({ 0x0F, 0x84, 0xc5, 0x00, 0x00, 0x00 }));
    register_patch(DS3, loose_params_aob_3, replace_with<uint8_t>({ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }));

    // TODO: AOB scan this?
    hooked_CreateFileW = register_hook(ALL, "C:\\windows\\system32\\kernel32.dll", "CreateFileW", tCreateFileW);
    hooked_virtual_to_archive_path_ds3 = register_hook(DS3, 0x14007d5e0, virtual_to_archive_path_ds3);

    for (const auto& mod : get_settings().mods()) {
        info(L"Installing mod location {}", mod.location);

        auto mod_path = resolve_mod_path(mod);
        if (mod_path) {
            info(L"Resolved mod path to {}", mod_path->wstring());
            hooked_file_roots.insert(mod_path->wstring());
        } else {
            warn(L"Unable to resolve mod path");
        }
    }
}

void ModLoaderExtension::on_detach()
{
}

}