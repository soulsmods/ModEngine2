#pragma once

#include <cstdint>

namespace modengine {

enum GameType : std::uint32_t {
    DS_REMASTERED = 1,
    DS2 = 1 << 1,
    DS3 = 1 << 2,
    SEKIRO = 1 << 3,
    ELDEN_RING = 1 << 4,
    ARMORED_CORE_6 = 1 << 5,
    ALL = 0xFFFFFFFF
};

inline GameType operator|(GameType lhs, GameType rhs)
{
    return static_cast<GameType>((uint32_t)lhs | (uint32_t)rhs);
}

}