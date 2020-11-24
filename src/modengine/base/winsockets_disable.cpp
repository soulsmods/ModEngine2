#include "modengine/base/winsockets_disable.h"

namespace modengine::base {

std::shared_ptr<Hook<fnWSAStartup>> hooked_WSAStartup;

// This is a quick and dirty way to force offline mode by ensuring that winsockets doesn't startup.
// This is a placeholder until we come up with a better way that doesn't prevent all networking.
INT __cdecl tWSAStartup(
    WORD,
    LPVOID)
{
    return 10091L; // WSASYSNOTREADY
}

}