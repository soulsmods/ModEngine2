#pragma once
#include "gametypes/class.h"
#include "gametypes/dantelion/dlstring.h"

struct DLThread
{
    CLASS_MEMBER(m_thread_handle, HANDLE, 0x08)
    CLASS_MEMBER(m_thread_id, DWORD, 0x10)
    CLASS_MEMBER(m_name, DLString<modengine::GameType::DS3>, 0x80)
};