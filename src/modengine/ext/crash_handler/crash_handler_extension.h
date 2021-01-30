#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

namespace modengine {
namespace ext {

class CrashHandlerExtension : public ModEngineExtension {
public:
    CrashHandlerExtension(const std::shared_ptr<ModEngine>& instance)
        : ModEngineExtension(instance)
    {
    }

private:
    void on_attach() override;
    void on_detach() override;

    std::string id() override {
        return "crash_reporter";
    }
};

}
}
