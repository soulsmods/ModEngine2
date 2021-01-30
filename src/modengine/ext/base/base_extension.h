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
    ModEngineBaseExtension(const std::shared_ptr<ModEngine>& instance)
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
};

}