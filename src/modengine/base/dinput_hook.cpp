#include "modengine/base/dinput_hook.h"

namespace modengine::base {

std::shared_ptr<Hook<fnDirectInput8Create>> hooked_DirectInput8Create;

/** This hook is exported as a symbol, so we need to make sure it's not mangled */
extern "C" __declspec(dllexport) HRESULT __cdecl DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter)
{
    return hooked_DirectInput8Create->original(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

}