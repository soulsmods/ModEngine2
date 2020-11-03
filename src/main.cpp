#include "modengine/mod_engine.h"

#include <optional>
#include <windows.h>

using namespace modengine;

ModEngine global_instance;

BOOL APIENTRY DllMain(HMODULE, DWORD dwReason, LPVOID)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        global_instance.attach();
        break;
    case DLL_PROCESS_DETACH:
        global_instance.detach();
        break;
    }
    return TRUE;
}