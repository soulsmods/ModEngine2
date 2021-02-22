#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

#ifndef MODENGINE_CRASH_REPORT_URL
#define MODENGINE_CRASH_REPORT_URL "http://localhost:3000/"
#endif

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
