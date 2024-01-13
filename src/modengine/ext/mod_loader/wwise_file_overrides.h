#pragma once

#include "modengine/hook_set.h"

namespace modengine::ext {

enum AKOpenMode : uint32_t {
    READ            = 0,
    WRITE           = 1,
    WRITE_OVERWRITE = 2,
    READ_WRITE      = 3,

    // Custom mode specific to From Software's implementation
    READ_EBL        = 9,
};

void* __cdecl ak_file_location_resolver_open(UINT64 p1, wchar_t* path, AKOpenMode openMode, UINT64 p4, UINT64 p5, UINT64 p6);

extern ScannedHook<decltype(&ak_file_location_resolver_open)> hooked_ak_file_location_resolver_open;

}
