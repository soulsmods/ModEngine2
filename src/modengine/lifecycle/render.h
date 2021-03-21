#pragma once

#include <nano_signal_slot.hpp>
#include <d3d11.h>

namespace modengine::lifecycle {

extern Nano::Signal<void(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)> on_frame;

}