#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

#include <filesystem>

namespace modengine::ext {

class ModLoaderExtension : public ModEngineExtension {
public:
    ModLoaderExtension(ModEngineExtensionConnector* instance)
        : ModEngineExtension(instance)
    {
    }

private:
    void on_attach() override;
    void on_detach() override;

    const char* id() override {
        return "mod_loader";
    }

    std::optional<std::filesystem::path> resolve_mod_path(const ModInfo& mod);
};

}
