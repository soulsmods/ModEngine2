#pragma once

#include "modengine/lifecycle.h"

#include <d3d11.h>

namespace modengine::lifecycle {

extern Signal<void(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)> on_frame;

}