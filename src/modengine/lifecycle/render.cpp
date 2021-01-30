#include "modengine/lifecycle/render.h"

namespace modengine::lifecycle {

Signal<void(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)> on_frame;

}
