#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

typedef HRESULT (*fnDirectInput8Create)(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter);

extern "C" __declspec(dllexport) HRESULT __cdecl DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter);

typedef INT (*fnWSAStartup)(
    WORD wVersionRequested,
    LPVOID lpWSAData);


INT __cdecl tWSAStartup(
    WORD wVersionRequested,
    LPVOID lpWSAData);

namespace modengine::ext {

class ModEngineBaseExtension : public ModEngineExtension {
public:
    ModEngineBaseExtension(ModEngineExtensionConnector* instance)
        : ModEngineExtension(instance)
    {
    }

private:
    void on_attach() override;
    void on_detach() override;

    std::string id() override
    {
        return "base";
    }

    std::function<void(IDXGISwapChain* swap_chain, UINT, UINT)> m_render_overlay_cb = [&](IDXGISwapChain* swap_chain, UINT, UINT) {
//        m_mod_engine->get_overlay().render(swap_chain);
    };
};

}