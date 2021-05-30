#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

#include <filesystem>

namespace modengine::ext {

struct ModInfo {
public:
    std::string name;
    std::string location;
    bool enabled;

    bool from_toml(ConfigReader& v)
    {
        auto loc = v.read_config_option<std::string>({ "path" });
        if (!loc) {
            return false;
        }

        name = v.read_config_option<std::string>({ "name" }).value_or("Unknown");
        enabled = v.read_config_option<bool>({ "enabled" }).value_or(true);
        location = loc.value();

        return true;
    }
};

struct ModLoaderConfig {
    std::vector<ModInfo> mods;
    bool load_loose_params;

    bool from_toml(ConfigReader& config)
    {
        mods = config.read_config_objects<ModInfo>({ "mods" });
        load_loose_params = config.read_config_option<bool>({ "load_loose_params" }).value_or(false);
        return true;
    }

    static ModLoaderConfig create_default()
    {
        auto config = ModLoaderConfig();
        return config;
    }
};

class ModLoaderExtension : public ModEngineExtension {
public:
    ModLoaderExtension(ModEngineExtensionConnector* instance)
        : ModEngineExtension(instance)
    {
    }

private:
    void on_attach() override;
    void on_detach() override;

    const char* id() override
    {
        return "mod_loader";
    }

    std::optional<std::filesystem::path> resolve_mod_path(const ModInfo& mod);
};

}
