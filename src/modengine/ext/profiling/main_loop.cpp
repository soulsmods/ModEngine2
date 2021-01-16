#include "modengine/ext/profiling/main_loop.h"
#include "optick.h"

#include <spdlog/spdlog.h>

namespace modengine::ext {

using namespace spdlog;
std::shared_ptr<Hook<fnMainLoop>> hooked_MainLoop;

INT __cdecl tMainLoop(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
{
    OPTICK_FRAME("MainThread");
    return hooked_MainLoop->original(a, b, c, d);
}

}