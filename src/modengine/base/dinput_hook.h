#pragma once

#include "modengine/hook_set.h"


namespace modengine::base {

typedef HRESULT (*fnDirectInput8Create)(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter);

extern std::shared_ptr<Hook<fnDirectInput8Create>> hooked_DirectInput8Create;

extern "C" __declspec(dllexport) HRESULT __cdecl DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter);

}