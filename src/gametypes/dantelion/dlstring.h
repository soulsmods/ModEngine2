#pragma once
#include "modengine/game_info.h"

template <modengine::GameType t, typename T>
struct DLString;

template <typename T>
struct DLString<modengine::GameType::DS3, T> {
    T* string;
    void* unk;
    UINT64 length;
    UINT64 capacity;

    T* str()
    {
        if (sizeof(T) * capacity >= 15) {
            return string;
        }

        return (T*)&string;
    }
};

using DLBasicString = DLString<modengine::GameType::DS3, char>;
