#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

extern "C" void* __profiler_begin(const char *zone_name, void *ctx);
extern "C" void __profiler_end(void *);

extern "C" void profiler_zone();
extern "C" void profiler_zone_exit();

namespace modengine::ext {

class ProfilingExtension : public ModEngineExtension {
public:
    ProfilingExtension(ModEngineExtensionConnector* instance)
        : ModEngineExtension(instance)
    {
    }

private:
    DWORD m_tls_idx;

    void on_attach() override;
    void on_detach() override;

    std::string id() override {
        return "profiling";
    }

    void install_profiler_zone(uintptr_t function_address, const char* zone);
};

}