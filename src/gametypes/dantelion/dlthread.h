#pragma once
#include "gametypes/class.h"
#include "gametypes/dantelion/dlstring.h"


struct DLThread {
    auto thread_handle() { return class_offset(HANDLE, 0x10); }
    auto thread_id() { return class_offset(DWORD, 0x08); }
    auto name() { return class_offset(DLBasicString, 0x80); }
};