#pragma once

enum GameType : uint32_t {
    DarkSoulsRemastered = 1,
    DarkSoulsIISOTFS = 1 << 1,
    DarkSoulsIII = 1 << 2,
    Sekiro = 1 << 3,
    AllGames = 0xFFFFFFFF
};