#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

#include <map>

namespace modengine::ext {

typedef bool (*fpProtobufDeserialize)(void*, unsigned char*, size_t);

class MatchmakingExtension : public ModEngineExtension {
public:
    MatchmakingExtension(const std::shared_ptr<ModEngine>& instance)
        : ModEngineExtension(instance)
    {
    }

private:
    void on_attach() override;
    void on_detach() override;

    std::string id() override
    {
        return "matchmaking";
    }
};
}
