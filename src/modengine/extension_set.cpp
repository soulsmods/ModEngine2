#include "extension_set.h"

#include <spdlog/spdlog.h>

#include <psapi.h>
#include <windows.h>

using namespace spdlog;

namespace modengine {

extern "C" bool modengine_ext_init(ModEngineExtensionConnector* connector, ModEngineExtension** extension);
using ModEngineExtensionInitializer = decltype(&modengine_ext_init);
static const char* const modengine_ext_init_name = "modengine_ext_init";

struct ExtensionInfo {
    bool enabled;

    bool from_toml(ConfigReader& v)
    {
        enabled = v.read_config_option<bool>({ "enabled" }).value_or(false);
        return true;
    }
};

void ExtensionSet::load_extensions(std::vector<fs::path> dlls, bool enumerate_modules)
{
    if (enumerate_modules) {
        HMODULE in_proc_modules[1024];
        DWORD in_proc_modules_count;

        if (EnumProcessModules(GetCurrentProcess(), in_proc_modules, sizeof(in_proc_modules), &in_proc_modules_count)) {
            for (auto i = 0; i < (in_proc_modules_count / sizeof(HMODULE)); i++) {
                if (GetProcAddress(in_proc_modules[i], modengine_ext_init_name) == nullptr) {
                    continue;
                }

                if (!load_extension(in_proc_modules[i])) {
                    // error
                }
            }
        }
    }

    for (auto& dll : dlls) {
        auto handle = GetModuleHandleW(dll.c_str());
        if (handle != nullptr) {
            continue; // already loaded by above, probably by lazy load or similar
        }

        auto module = LoadLibraryW(dll.c_str());
        if (module == nullptr) {
            error(L"Failed to load DLL {}", dll.wstring());
            continue;
        }

        info(L"Loaded external DLL {}", dll.wstring());

        if (!load_extension(module)) {
            warn(L"External dll {} at base address {:p} is not a modengine extension", dll.wstring(), fmt::ptr((void*)module));
        }
    }
}

bool ExtensionSet::load_extension(HMODULE module)
{
    auto init_symbol = reinterpret_cast<ModEngineExtensionInitializer>(GetProcAddress(module, modengine_ext_init_name));
    if (init_symbol == nullptr) {
        return false;
    }

    ModEngineExtension* extension = nullptr;
    if (!init_symbol(m_connector, &extension)) {
        return false;
    }

    auto extension_id = extension->id();
    m_extension_modules[extension_id] = module;
    m_extensions[extension_id] = std::unique_ptr<ModEngineExtension>(extension);
    return true;
}

void ExtensionSet::attach_all(Settings& settings)
{
    for (const auto &[id, _v] : m_extensions) {
        const auto ext_settings = settings.get_config_reader().read_config_object<ExtensionInfo>({"extension", id});

        if (ext_settings.enabled || id == "base") {
            info("Enabling extension {}", id);
            attach(id);
            info("Enabled extension {}", id);
        }
    }
}

void ExtensionSet::attach(const std::string& id)
{
    if (!m_extensions.contains(id)) {
        return;
    }

    m_extensions[id]->on_attach();
}

void ExtensionSet::detach_all()
{
    for (const auto &[id, _v] : m_extensions) {
        detach(id);
    }
}

void ExtensionSet::detach(const std::string& id)
{
    if (!m_extensions.contains(id)) {
        return;
    }

    m_extensions[id]->on_detach();
    m_extensions.erase(id);

    if (m_extension_modules.contains(id)) {
        if (!FreeLibrary(m_extension_modules[id])) {
            // warning: dangling references to library
        }

        m_extension_modules.erase(id);
    }
}

void ExtensionSet::reload(const std::string& id)
{

}

}