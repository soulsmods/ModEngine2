#include "direct_input.h"

namespace modengine::hooks {

typedef HRESULT (*fnDirectInput8Create)(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter);

std::shared_ptr<Hook<fnDirectInput8Create>> directInput8CreateHook;

/** This hook is exported as a symbol, so we need to make sure it's not mangled */
extern "C" __declspec(dllexport) HRESULT __cdecl DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter)
{
    return modengine::hooks::directInput8CreateHook->original(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

void register_direct_input_hooks(HookSet& hooks)
{
    /* fixme: shouldn't hardcode this path */
    directInput8CreateHook = hooks.install("C:\\windows\\system32\\dinput8.dll", "DirectInput8Create", DirectInput8Create);
}

}
