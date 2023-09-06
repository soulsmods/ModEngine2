#pragma once

#include "modengine/hook_set.h"

namespace modengine::ext {

void* __cdecl ak_file_location_resolver_open(UINT64 p1, wchar_t* path, UINT64 p3, UINT64 p4, UINT64 p5, UINT64 p6);

extern ScannedHook<decltype(&ak_file_location_resolver_open)> hooked_ak_file_location_resolver_open;

}