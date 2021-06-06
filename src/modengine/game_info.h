#pragma once

#include "modengine/game_type.h"

#include <cstdint>
#include <string>
#include <optional>

namespace modengine {

struct GameInfo {
public:
    GameInfo(GameType _type, std::string _version)
        : type(_type)
        , version(_version)
    {
    }

    GameType type;
    std::string version;

    // \brief Check if the given `other` game type is supported.
    bool is_supported(GameType other) const {
        return (other & type) != 0;
    }

    // \brief Gets a human recognizable description of the game.
    std::string& description() const;

    /**
     * Try to determine information for the running game based on the module ModEngine has been attached to.
     *
     * @return A potentially empty value iff no supported game was found, otherwise game type and version information.
     */
    static std::optional<GameInfo> from_current_module();
};

}