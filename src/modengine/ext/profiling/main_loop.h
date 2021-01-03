#pragma once

#include "modengine/hook_set.h"

namespace modengine::ext {

typedef INT (*fnMainLoop)(uint64_t, uint64_t, uint64_t, uint64_t);

extern std::shared_ptr<Hook<fnMainLoop>> hooked_MainLoop;

INT __cdecl tMainLoop(uint64_t, uint64_t, uint64_t, uint64_t);

}