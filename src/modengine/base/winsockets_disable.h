#pragma once

#include "modengine/hook_set.h"

namespace modengine::base {

typedef INT (*fnWSAStartup)(
    WORD wVersionRequested,
    LPVOID lpWSAData);

typedef uint64_t (*fnIPv4AddrFunction)(
    char*,
    uint32_t*);

extern std::shared_ptr<Hook<fnWSAStartup>> hooked_WSAStartup;
extern std::shared_ptr<Hook<fnIPv4AddrFunction>> hooked_IPv4AddrFunction;

INT __cdecl tWSAStartup(
    WORD wVersionRequested,
    LPVOID lpWSAData);

uint64_t __cdecl tIPv4AddrFunction(
    char*,
    uint32_t*);

}