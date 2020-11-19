#pragma once

#include "modengine/hook_set.h"

namespace modengine::base {

typedef INT (*fnWSAStartup)(
    WORD wVersionRequested,
    LPVOID lpWSAData);

extern std::shared_ptr<Hook<fnWSAStartup>> hooked_WSAStartup;

INT __cdecl tWSAStartup(
    WORD wVersionRequested,
    LPVOID lpWSAData);

}