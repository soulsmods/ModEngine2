#pragma once

#include "modengine/hook_set.h"

#include <memory>

namespace modengine::base {

typedef int (WINAPI *fnEntry)(void);

extern std::shared_ptr<Hook<fnEntry>> hooked_Entry;
extern HookSet entry_hook_set;

int WINAPI Entry();

}
