#include "mod_loader_extension.h"
#include "archive_file_overrides.h"

#include "modengine/util/hex_string.h"

namespace fs = std::filesystem;

namespace modengine::ext {

auto loose_params_aob_1 = util::hex_string("74 68 48 8b cf 48 89 5c 24 30 e8 1c 6d 08");
auto loose_params_aob_2 = util::hex_string("0F 85 C5 00 00 00 48 8D 4C 24 28 E8 17 F4");
auto loose_params_aob_3 = util::hex_string("E8 78 08 F8 FF 90 E9 0E E9 08 05 53 E9 EF");

static std::vector<fs::path> mod_base_paths(const Settings& settings)
{
    return {
        settings.game_path(),
        settings.modengine_install_path(),
        settings.modengine_local_path()
    };
}

void ModLoaderExtension::install_mod(const ModInfo& mod)
{
    info(L"Installing mod location {}", mod.location);

    auto mod_path = fs::path(mod.location);
    if (mod_path.is_absolute()) {
        hooked_file_roots.push_back(mod.location);
        return;
    }

    const auto base_paths = mod_base_paths(settings());
    for (const auto& base_path : base_paths) {
        auto new_path = base_path / mod_path;

        if (fs::exists(new_path)) {
            info(L"Resolved mod to {}", new_path.wstring());

            hooked_file_roots.push_back(new_path.wstring());
            return;
        }
    }
}

void ModLoaderExtension::on_attach()
{
    register_patch(DS3, loose_params_aob_1, replace_with<uint8_t>({ 0xEB, 0x68 }));
    register_patch(DS3, loose_params_aob_2, replace_with<uint8_t>({ 0x0F, 0x84, 0xc5, 0x00, 0x00, 0x00 }));
    register_patch(DS3, loose_params_aob_3, replace_with<uint8_t>({ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }));

    // TODO: AOB scan this?
    hooked_CreateFileW = register_hook(ALL, "C:\\windows\\system32\\kernel32.dll", "CreateFileW", tCreateFileW);
    hooked_virtual_to_archive_path_ds3 = register_hook(DS3, 0x14007d5e0, virtual_to_archive_path_ds3);

    for (const auto& mod : settings().mods()) {
        install_mod(mod);
    }
}

void ModLoaderExtension::on_detach()
{
}

}