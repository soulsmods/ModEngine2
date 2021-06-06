#include <kiero.h>
#include "base_extension.h"

#include "modengine/util/hex_string.h"
#include "modengine/lifecycle/render.h"
#include <spdlog/spdlog.h>

using namespace spdlog;
namespace modengine::ext {

static Hook<fnDirectInput8Create> hooked_DirectInput8Create;
static long(__stdcall* original_D3D11Present)(IDXGISwapChain*, UINT, UINT) = nullptr;

/** This hook is exported as a symbol, so we need to make sure it's not mangled */
extern "C" __declspec(dllexport) HRESULT __cdecl DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter)
{
    return hooked_DirectInput8Create.original(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

long __stdcall hooked_D3D11Present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags)
{
    lifecycle::on_frame.fire(swap_chain, sync_interval, flags);

    return original_D3D11Present(swap_chain, sync_interval, flags);
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
    // increase default heap sizes
    register_patch(DS3 | SEKIRO, allocator_table_aob, increase_fmod_allocation_limits);

    // nop call to SetUnhandledExceptionFilter
    register_patch(DS3, 0x141fe49c7, replace_with<unsigned char>({ 0x90, 0x90, 0x90, 0x90, 0x90 }));

    // change panic mode to RAISE_EXCEPTION_ON_PANIC
    register_patch(DS3, 0x1446418c8, replace_with<uint32_t>({ 0x02 }));

    if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
        info("Initialized overlay hooks");
        kiero::bind(8, (void**)&original_D3D11Present, (void*)hooked_D3D11Present);
    } else {
        info("Failed to initialize overlay hooks");
    }

    register_hook(ALL, &hooked_DirectInput8Create, "C:\\windows\\system32\\dinput8.dll", "DirectInput8Create", DirectInput8Create);

    lifecycle::on_frame.connect(m_render_overlay_cb);
}

void ModEngineBaseExtension::on_detach()
{
    lifecycle::on_frame.disconnect(m_render_overlay_cb);
}

}