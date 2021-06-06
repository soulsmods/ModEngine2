#pragma once

#include "modengine/mod_engine.h"

extern "C" VOID tLoadDbgFont();
extern "C" DWORD64 bLoadDbgFont;

extern "C" VOID fDbgDispLoading();

extern "C" DWORD64 bInitDebugBootMenuStep;
extern "C" DWORD64 bInitMoveMapListStep;
extern "C" DWORD64 bGameStepSelection;
extern "C" DWORD64 bCheckDebugDashSwitch;
extern "C" DWORD64 bLoadGameProperties;
extern "C" DWORD64 bDecWindowCounter;
extern "C" DWORD64 bIncWindowCounter;
extern "C" VOID tInitDebugBootMenuStep();
extern "C" VOID tInitMoveMapListStep();
extern "C" VOID tGameStepSelection();
extern "C" VOID tCheckDebugDashSwitch();
extern "C" VOID tLoadGameProperties();
extern "C" VOID initDebugBootMenuStepFunctions();
extern "C" VOID sub_140CF3520();
extern "C" VOID patchMoveMapFinishAntiTamper();
extern "C" VOID decWindowCounter();
extern "C" VOID incWindowCounter();

extern "C" VOID debugBootMenuStepDtor();
extern "C" VOID EzTextlistSelectorDtor();
extern "C" VOID MoveMapListStepDtor();

extern "C" DWORD64* debugBootMenuStepVtable;
extern "C" DWORD64* EzTextlistSelectorVtable;
extern "C" DWORD64* MoveMapListStepVtable;

namespace modengine::ext {

class DebugMenuDS3Extension : public ModEngineExtension {
public:
    DebugMenuDS3Extension(ModEngineExtensionConnector* instance)
        : ModEngineExtension(instance)
    {
    }

    void DelayedPatches();
    void ExtraDelayedPatches();

private:
    template <typename T>
    void InstallHook(LPVOID address, int numBytes, T pFunction, DWORD64* returnPoint)
    {
        *returnPoint = (DWORD64)address + numBytes;
        register_hook<T>(DS3, new Hook<T>((uintptr_t)address, pFunction));
    }

    void on_attach() override;
    void on_detach() override;

    const char* id() override
    {
        return "debug_menu";
    }
};

}