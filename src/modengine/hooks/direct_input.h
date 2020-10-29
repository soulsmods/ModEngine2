#pragma once

#include "modengine/hook_set.h"

extern "C" __declspec(dllexport) HRESULT __cdecl DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter);

namespace modengine::hooks {

void register_direct_input_hooks(HookSet& hooks);

}