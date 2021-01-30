#include "base_extension.h"

#include "modengine/util/hex_string.h"

namespace modengine::ext {

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

std::string allocator_table_aob = util::hex_string(
    "00 00 00 60 00 00 00 00"
    "00 00 00 60 00 00 00 00"
    "00 00 00 04 00 00 00 00"
    "00 00 10 00 00 00 00 00"
    "00 00 10 00 00 00 00 00"
    "00 00 00 02 00 00 00 00"
    "00 00 00 01 00 00 00 00");

void increase_fmod_allocation_limits(uintptr_t addr)
{
    std::uint64_t* allocator_table = reinterpret_cast<uint64_t*>(addr);
    allocator_table[9] *= 3;
    allocator_table[10] *= 3;
}

void ModEngineBaseExtension::on_attach()
{
    register_patch(DS3 | SEKIRO, allocator_table_aob, increase_fmod_allocation_limits);

    hooked_DirectInput8Create = register_hook(ALL, "C:\\windows\\system32\\dinput8.dll", "DirectInput8Create", DirectInput8Create);
}

void ModEngineBaseExtension::on_detach()
{
}

}