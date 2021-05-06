#include "debug_menu_ds3.h"

#include "modengine/ext/mod_loader/archive_file_overrides.h"

// Outside namespace since they link agains the ASM patches
DWORD64 bLoadDbgFont = 0;
DWORD64 bInitDebugBootMenuStep = 0;
DWORD64 bInitMoveMapListStep = 0;
DWORD64 bGameStepSelection = 0;
DWORD64 bCheckDebugDashSwitch = 0;
DWORD64 bLoadGameProperties = 0;
DWORD64 bDecWindowCounter = 0;
DWORD64 bIncWindowCounter = 0;

BYTE pGestureBytes[1] = { 0x75 };

BYTE pFreeCamBytes1[5] = { 0xE8, 0xBD, 0xB0, 0xD0, 01 };
BYTE pFreeCamBytes2[35] = { 0x44, 0x88, 0xA6, 0x98, 0x00, 0x00, 0x00, 0x8B, 0x83, 0xE0, 0x00, 0x00, 0x00, 0xFF, 0xC8, 0x83, 0xF8, 0x01, 0x0F, 0x87, 0x35, 0x01, 0x00, 0x00, 0x44, 0x88, 0xBE, 0x98, 0x00, 0x00, 0x00 };
BYTE dbgFontPatch[2] = { 0xEB, 0x10 };
BYTE moveMapListStepPatch[18] = { 0xC7, 0x46, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0x48, 0x8B, 0x74, 0x24, 0x38, 0x48, 0x83, 0xC4, 0x20, 0x5F, 0xC3 };
BYTE sfxGUIPatch1[16] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
BYTE sfxGUIPatch2[8] = { 0x48, 0x8B, 0x8A, 0x40, 0x03, 0x00, 0x00, 0x90 };

BYTE mov1ToAlBytes[5] = { 0xB0, 0x01, 0x90, 0x90, 0x90 };
BYTE nopBytes[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
BYTE xorRaxBytes[5] = { 0x48, 0x31, 0xC0, 0x90, 0x90 };
BYTE jmpBytes[2] = { 0xEB, 0x71 };

DWORD64* EzTextlistSelectorVtable = new DWORD64[23];
DWORD64* MoveMapListStepVtable = new DWORD64[5];
DWORD64* debugBootMenuStepVtable = new DWORD64[16];

extern "C" wchar_t DSFontTpfPath[] = L"font:/DSFont24.tpf";
extern "C" wchar_t DSFontCcmPath[] = L"font:/DSFont24.ccm";
extern "C" wchar_t TalkFontTpfPath[] = L"font:/TalkFont.tpf";
extern "C" wchar_t TalkFontCcmPath[] = L"font:/TalkFont.ccm";

extern "C" wchar_t ezSelectBotDebugBootMenuString[] = L"SprjEzSelectBot.DebugBootMenuStep";

extern "C" wchar_t debugBootMenuStringModelViewer[] = L"MODEL_VIEWER";
extern "C" wchar_t debugBootMenuStringSoundTest[] = L"SOUND_TEST";
extern "C" wchar_t debugBootMenuStringMapViewer[] = L"MAP_VIEWER";
extern "C" wchar_t debugBootMenuStringStageSelect[] = L"STAGE_SELECT";
extern "C" wchar_t debugBootMenuStringAutoTest[] = L"AUTO_TEST";
extern "C" wchar_t debugBootMenuStringTitleBoot[] = L"TITLE_BOOT";
extern "C" wchar_t debugBootMenuStringE3TitleBoot[] = L"E3_TITLE_BOOT";
extern "C" wchar_t debugBootMenuStringGamescomTitleBoot[] = L"GAMESCOM_TITLE_BOOT";
extern "C" wchar_t debugBootMenuStringLoadTest[] = L"LOAD_TEST";
extern "C" wchar_t debugBoogMenuStringDebugMoveMapTest[] = L"DEBUG_MOVEMAP_TEST";
extern "C" wchar_t debugBootMenuStringMoveMapTest[] = L"MOVEMAP_TEST";
extern "C" wchar_t debugBootMenuStringMoveMapTestCeremony[] = L"MOVEMAP_TEST_CEREMONY";
extern "C" wchar_t debugBootMenuStringModuleTest[] = L"MODULE_TEST";
extern "C" wchar_t debugBootMenuStringMenuViewer[] = L"MENU_VIEWER";
extern "C" wchar_t debugBootMenuStringSfxViewer[] = L"SFX_VIEWER";

extern "C" wchar_t debugBootMenuTextlistPath[] = L"testdata:/debugBootMenu.textlist";
extern "C" wchar_t debugBootMenuString[] = L"DEBUG BOOT MENU <デバッグ起動メニュー>";

namespace modengine::ext {

inline void MemcpyProtected(uint64_t address, int length, void* bytes)
{
    DWORD oldProtect;

    VirtualProtect((LPVOID)address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((void*)address, bytes, length);
    VirtualProtect((LPVOID)address, length, oldProtect, &oldProtect);
}

inline void WriteProtected(uint64_t address, BYTE value)
{
    DWORD oldProtect;

    VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(BYTE*)address = value;
    VirtualProtect((LPVOID)address, 1, oldProtect, &oldProtect);
}

inline void WriteProtected(uint64_t address, short value)
{
    DWORD oldProtect;

    VirtualProtect((LPVOID)address, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(short*)address = value;
    VirtualProtect((LPVOID)address, 2, oldProtect, &oldProtect);
}

inline void WriteProtected(uint64_t address, int value)
{
    DWORD oldProtect;

    VirtualProtect((LPVOID)address, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(int*)address = value;
    VirtualProtect((LPVOID)address, 4, oldProtect, &oldProtect);
}

inline void WriteProtected(uint64_t address, long long value)
{
    DWORD oldProtect;

    VirtualProtect((LPVOID)address, 8, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(long long*)address = value;
    VirtualProtect((LPVOID)address, 8, oldProtect, &oldProtect);
}

void DebugMenuDS3Extension::ExtraDelayedPatches()
{
    DWORD dBypassCheck1 = 0;
    DWORD dBypassCheck2 = 0;

    Sleep(4000); //Kill me

    //Pav patches
    MemcpyProtected(0x14080A2F0, 2, mov1ToAlBytes); //-- Enable INS
    MemcpyProtected(0x14080A2E0, 2, mov1ToAlBytes); //-- Enable Event
    MemcpyProtected(0x140E82DEE, 5, xorRaxBytes); //-- (HeatMap Menu) Crash on load so it's disabled

    //Enable ChrDbgDraw
    MemcpyProtected(0x1408D8049, 5, mov1ToAlBytes);

    dBypassCheck1 = 0x0030EEA3;
    //MemcpyProtected(0x1408B1CF1, 4, &dBypassCheck1); //Bypass the check that bricks saves
    WriteProtected(0x1408B1CF1, (int)dBypassCheck1);
    dBypassCheck2 = 0xFFE48E31;
    WriteProtected(0x140EE7C01, (int)dBypassCheck2);

    //Enable system.properties (maybe???)
    int systemPropertiesValue = 3;
    WriteProtected(0x140EC365F, systemPropertiesValue);

    //Enable game.properties
    Hook((LPVOID)0x14080905B, 7, &tLoadGameProperties, &bLoadGameProperties);

    Hook((LPVOID)0x1422ED2FC, 6, &decWindowCounter, &bDecWindowCounter);
    //Hook((LPVOID)0x14045399C, 6, &incWindowCounter, &bIncWindowCounter);
    Hook((LPVOID)0x1422F0522, 7, &incWindowCounter, &bIncWindowCounter);

    //Pav patch for sfx GUI menus
    MemcpyProtected(0x140250A5F, 16, sfxGUIPatch1);
    MemcpyProtected(0x140DF3F56, 8, sfxGUIPatch2);
}

void DebugMenuDS3Extension::DelayedPatches()
{
    //Boot Menu
    int size = 0x230;
    WriteProtected(0x140ED13F1, size);
    Hook((LPVOID)0x140ED1457, 5, &tInitDebugBootMenuStep, &bInitDebugBootMenuStep);
    //size = 0x155;
    //WriteProtected(0x1408FDC4B, size);

    MemcpyProtected(0x1408E7E2C, 18, moveMapListStepPatch);

    //Quick and Dirty
    //Hook((LPVOID)0x1408FDC61, 5, &tInitMoveMapListStep, &bInitMoveMapListStep);

    //Proper
    Hook((LPVOID)0x1408FDC13, 7, &tGameStepSelection, &bGameStepSelection);

    Hook((LPVOID)0x140D4E027, 7, &tLoadDbgFont, &bLoadDbgFont);

    //Fix WindWorld option
    MemcpyProtected(0x140CDC43F, 2, nopBytes);

    //Features -- Freecam (A + L3)
    MemcpyProtected(0x14062C3AE, 5, pFreeCamBytes1);
    MemcpyProtected(0x14062C401, 31, pFreeCamBytes2);

    //Disable Gesture Menu
    MemcpyProtected(0x140B2D583, 1, pGestureBytes);

    uint64_t DbgDispLoadingAddress = (uint64_t)&fDbgDispLoading;
    //MemcpyProtected(0x144587EC8, 8, &DbgDispLoadingAddress);
    WriteProtected(0x144587EC8, (long long)DbgDispLoadingAddress);

    MemcpyProtected(0x140022909, 2, nopBytes); //-- Enable Cubemap Generation nodes

    //MemcpyProtected(0x140EE7C01, 4, &dBypassCheck2); //This check restores this one ^

    //Dbg font loading
    MemcpyProtected(0x142346F45, 2, dbgFontPatch);

    //Temporarily patch anti-tamper
    DWORD64 lol = 0;
    Hook((LPVOID)0x1408E7897, 5, &patchMoveMapFinishAntiTamper, &lol);

    MemcpyProtected(0x140B33BCD, 2, jmpBytes);

    Hook((LPVOID)0x1408D475E, 5, &tCheckDebugDashSwitch, &bCheckDebugDashSwitch);

    //why am I calling this twice
    Hook((LPVOID)0x14080905B, 7, &tLoadGameProperties, &bLoadGameProperties);

    //Sleep(4000);

    //Enable ChrDbgDraw
    //MemcpyProtected(0x1408D8049, 5, mov1ToAlBytes);

    reapply();
}

static void setup_vtables()
{
    for (int i = 0; i < 21; i++) {
        EzTextlistSelectorVtable[i] = *(DWORD64*)(0x142894A08 + i * 8);
    }

    //Add functions
    EzTextlistSelectorVtable[21] = EzTextlistSelectorVtable[9];
    EzTextlistSelectorVtable[22] = (DWORD64)&sub_140CF3520;

    //Make additional modifications
    EzTextlistSelectorVtable[1] = (DWORD64)&EzTextlistSelectorDtor;
    EzTextlistSelectorVtable[2] = 0x14065AD60;

    for (int i = 0; i < 5; i++) {
        MoveMapListStepVtable[i] = *(DWORD64*)(0x142849EC8 + i * 8);
    }

    //Make additional modifications
    MoveMapListStepVtable[1] = (DWORD64)&MoveMapListStepDtor;

    for (int i = 0; i < 16; i++) {
        debugBootMenuStepVtable[i] = *(DWORD64*)(0x14285F128 + i * 8);
    }

    //Make additional modifications
    debugBootMenuStepVtable[1] = (DWORD64)&debugBootMenuStepDtor;
}

static DWORD WINAPI ExtraDelayedPatchesStart(void* Param)
{
    DebugMenuDS3Extension* This = (DebugMenuDS3Extension*)Param;
    This->ExtraDelayedPatches();
    return 0;
}

static DWORD WINAPI DelayedPatchesStart(void* Param)
{
    DebugMenuDS3Extension* This = (DebugMenuDS3Extension*)Param;
    This->DelayedPatches();
    return 0;
}

void DebugMenuDS3Extension::on_attach()
{
    auto debug_menu_assets_path = get_settings().modengine_install_path() / "assets" / "debug_menu";
    hooked_file_roots.insert(debug_menu_assets_path.native());

    spdlog::info(L"Applying debug menu patches");

    MemcpyProtected(0x140ECDCE4, 5, mov1ToAlBytes);

    //Boot Menu
    WriteProtected(0x142720800, (long long)&initDebugBootMenuStepFunctions);

    setup_vtables();

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DelayedPatchesStart, this, NULL, NULL);
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ExtraDelayedPatchesStart, this, NULL, NULL);
}

void DebugMenuDS3Extension::on_detach()
{
}

}