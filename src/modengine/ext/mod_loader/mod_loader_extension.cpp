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

auto virtual_to_archive_path_er_aob = util::hex_aob("e8 ?? ?? ?? ?? 48 83 7b 20 08 48 8d 4b 08 72 03 48 8b 09 4c 8b 4b 18 41 b8 05 00 00 00 4d 3b c8");

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

    const auto primary_search_path = mod_engine_global->get_settings().modengine_local_path();
    const auto primary_mod_path = primary_search_path / mod_path;

    if (fs::exists(primary_search_path / mod_path)) {
        return primary_mod_path;
    }

    return std::nullopt;
}

void ModLoaderExtension::on_attach()
{
    register_patch(DS3, loose_params_aob_1, replace_with<uint8_t>({ 0xEB, 0x68 }));
    register_patch(DS3, loose_params_aob_2, replace_with<uint8_t>({ 0x0F, 0x84, 0xc5, 0x00, 0x00, 0x00 }));
    register_patch(DS3, loose_params_aob_3, replace_with<uint8_t>({ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }));

    // TODO: AOB scan this?
    register_hook(ALL, &hooked_CreateFileW, "C:\\windows\\system32\\kernel32.dll", "CreateFileW", tCreateFileW);
    register_hook(DS3, &hooked_virtual_to_archive_path_ds3, 0x14007d5e0, virtual_to_archive_path_ds3);
    register_hook(ELDEN_RING, &hooked_virtual_to_archive_path_eldenring, virtual_to_archive_path_er_aob, virtual_to_archive_path_eldenring, SCAN_CALL_INST);

    auto config = get_config<ModLoaderConfig>();
    for (const auto& mod : config.mods) {
        info("Installing mod location {}", mod.location);

        auto mod_path = resolve_mod_path(mod);
        if (mod_path) {
            info(L"Resolved mod path to {}", mod_path->wstring());
            hooked_file_roots.push_back(mod_path->wstring());
        } else {
            warn(L"Unable to resolve mod path");
        }
    }
}

void ModLoaderExtension::on_detach()
{
}

}