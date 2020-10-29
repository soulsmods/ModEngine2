#pragma once

enum GameType {
    DarkSoulsRemastered = 1,
    DarkSoulsIISOTFS = 1 << 1,
    DarkSoulsIII = 1 << 2,
    Sekiro = 1 << 3,
    AllGames = 0xFFFFFFFF
};