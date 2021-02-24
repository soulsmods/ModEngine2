#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

#include <filesystem>

namespace modengine::ext {

class ModLoaderExtension : public ModEngineExtension {
public:
    ModLoaderExtension(const std::shared_ptr<ModEngine>& instance)
        : ModEngineExtension(instance)
    {
    }

private:
    void on_attach() override;
    void on_detach() override;

    std::string id() override {
        return "mod_loader";
    }

    std::optional<std::filesystem::path> resolve_mod_path(const ModInfo& mod);
};

}
