#pragma once

#include "modengine/hook_set.h"

namespace modengine::ext {

INT __cdecl tMainLoop(uint64_t, uint64_t, uint64_t, uint64_t);

extern Hook<decltype(&tMainLoop)> hooked_MainLoop;

}