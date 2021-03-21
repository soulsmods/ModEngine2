#include "modengine/lifecycle/render.h"

namespace modengine::lifecycle {

Nano::Signal<void(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)> on_frame;

}
