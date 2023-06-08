#include "modengine/ext/profiling/main_loop.h"
#include "modengine/util/platform.h"

#include <spdlog/spdlog.h>

namespace modengine::ext {

using namespace spdlog;
Hook<decltype(&tMainLoop)> hooked_MainLoop { util::rva2addr(0xEDAAE0), tMainLoop };

INT __cdecl tMainLoop(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
{
//    OPTICK_FRAME("MainThread");
    return hooked_MainLoop.original(a, b, c, d);
}

}