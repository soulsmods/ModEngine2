#pragma once

#include "modengine/hook_set.h"
#include "gametypes/dantelion/dlthread.h"

namespace modengine::ext {

typedef INT (*fnDLThreadHandler)(DLThread*);

extern std::shared_ptr<Hook<fnDLThreadHandler>> hooked_DLThreadHandler;

INT __cdecl tDLThreadHandler(DLThread *thread);

}