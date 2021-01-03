#pragma once
#include "modengine/game_info.h"

template <modengine::GameType t>
struct DLString;

template<>
struct DLString<modengine::GameType::DS3> {
    char *string;
	void *unk;
	UINT64 length;
	UINT64 capacity;

    const char* str()
    {
        if (capacity > 15) {
            return string;
        }
        return (char*)&string;
    }
};