#include "optick.h"
#include "modengine/ext/profiling_extension.h"
#include "modengine/ext/profiling/main_loop.h"
#include "modengine/ext/profiling/thread_hooks.h"

namespace modengine::ext {

// Prelude struct that stores constant data associated with a hook
struct ProfilerPreludeData {
    const char* zone; // 0x00
    uintptr_t original_address; // 0x08
    uintptr_t profiler_entry_address; // 0x10
    uintptr_t profiler_exit_address; // 0x18
    uintptr_t profiler_epilogue_address; // 0x20
    DWORD tls_index; // 0x28
    DWORD _pad; // 0x2C
}; // Total size: 0x30

// Runtime context data associated with a call to a hook
struct ProfilerContextData {
    uintptr_t return_address; // 0x00
    uint64_t rbx; // 0x08
    uint64_t r12; // 0x10
}; // Total size: 0x18

void ProfilingExtension::on_attach()
{
    if (!settings().is_profiler_enabled()) {
        return;
    }

    info("Setting up profiler");

    // Thread local storage to allocate per-thread context stacks
    m_tls_idx = TlsAlloc();
    if (m_tls_idx == TLS_OUT_OF_INDEXES) {
        error("Could not allocate TLS index for profiler");
        return;
    }

    // Hooks main loop to install Optick main loop event
    hooked_MainLoop = register_hook(DS3, 0x140eccb30, tMainLoop);

    // Registers engine created DLThreads with Optick
    hooked_DLThreadHandler = register_hook(DS3, 0x1417ef4b0, tDLThreadHandler);

    // Zones for functions called by the main loop tick
    install_profiler_zone(0x140ee4d10, "fun_140ee4d10");
    install_profiler_zone(0x140e95b35, "fun_140e95b35");
    install_profiler_zone(0x140f01cc0, "fun_140f01cc0");
    install_profiler_zone(0x140e96260, "fun_140e96260");
    install_profiler_zone(0x141779af0, "fun_141779af0");
    //install_profiler_zone(0x14007d5e0, "test_zone");

    // Test SprjGraphicsStep (don't these are actually called though?)
    install_profiler_zone(0x140d4d240, "SprjGraphicsStep::STEP_Init");
    install_profiler_zone(0x140d4e220, "SprjGraphicsStep::STEP_Prepare_forGraphicsFramework");
    install_profiler_zone(0x140d4d370, "SprjGraphicsStep::STEP_Initialize_forGraphicsFramework");
    install_profiler_zone(0x140d4e2a0, "SprjGraphicsStep::STEP_Prepare_forSystem");
    install_profiler_zone(0x140d4e0f0, "SprjGraphicsStep::STEP_Initialize_forSystem");
    install_profiler_zone(0x140d4e240, "SprjGraphicsStep::STEP_Prepare_forGuiFramework");
    install_profiler_zone(0x140d4d450, "SprjGraphicsStep::STEP_Initialize_forGuiFramework");
    install_profiler_zone(0x140d4e260, "SprjGraphicsStep::STEP_Prepare_forRenderingSystem");
    install_profiler_zone(0x140d4d4c0, "SprjGraphicsStep::STEP_Initialize_forRenderingSystem");
    install_profiler_zone(0x140d4e280, "SprjGraphicsStep::STEP_Prepare_forSfxSystem");
    install_profiler_zone(0x140d4e070, "SprjGraphicsStep::STEP_Initialize_forSfxSystem");
    install_profiler_zone(0x140d4e200, "SprjGraphicsStep::STEP_Prepare_forDecalSystem");
    install_profiler_zone(0x140d4d320, "SprjGraphicsStep::STEP_Initialize_forDecalSystem");
    install_profiler_zone(0x140d4c9d0, "SprjGraphicsStep::STEP_BeginDrawStep");
    install_profiler_zone(0x140d4e770, "SprjGraphicsStep::STEP_Update");
    install_profiler_zone(0x140d4e800, "SprjGraphicsStep::STEP_WaitFinishDrawStep");
    install_profiler_zone(0x140d4ca20, "SprjGraphicsStep::STEP_Finish");

    install_profiler_zone(0x140d485b0, "SprjDrawStep::STEP_Update");
    install_profiler_zone(0x140d0f9d0, "SprjFD4LocationStep::STEP_Update");
    install_profiler_zone(0x140980de0, "SprjCameraStep::STEP_Update");
    //install_profiler_zone(0x1409577f0, "SprjWorldScopeStep::STEP_Update");
    install_profiler_zone(0x1404a1820, "SprjLuaEventUpdateTask::_STEP_Update");
    install_profiler_zone(0x14045cd10, "prjEmkSystemUpdateTask::_STEP_Update");
    install_profiler_zone(0x1404b8650, "SprjObjActUpdateTask::_STEP_Update");
    install_profiler_zone(0x140508b10, "SprjFileStep::STEP_Update");
    install_profiler_zone(0x140df18b0, "RendManStep::STEP_Update");
    install_profiler_zone(0x140e3df40, "SprjResStep::STEP_Update");
    install_profiler_zone(0x140e74c10, "SprjSoundStep::STEP_Update");
    install_profiler_zone(0x140e42280, "SprjScaleformStep::STEP_UpdateB");

    install_profiler_zone(0x1400d1cb0, "ForTaskRunner?(fun_1400d1cb0)");
    install_profiler_zone(0x1400b5e80, "GXInternalForTask::Entry?(fun_1400b5e80)");

    // Sync stuff?
    install_profiler_zone(0x1417edb00, "DLKR::DLPlainReadWriteLock::BeginRead");
}

void ProfilingExtension::on_detach()
{
}

void ProfilingExtension::install_profiler_zone(uintptr_t function_address, const char* zone)
{
    unsigned char prelude_code[] = {
        0x53,                                     // push rbx
        0x48, 0x8D, 0x1D, 0xC8, 0xFF, 0xFF, 0xFF, // lea rbx, [rip-56]
        0xFF, 0x63, 0x10,                         // jmp [rbx+16]
    };

    unsigned char prologue_code[] = {
        0x48, 0x8D, 0x1D, 0xBE, 0xFF, 0xFF, 0xFF, // lea rbx, [rip-66]
        0xFF, 0x63, 0x18                          // jmp [rbx+24]
    };

    const auto prelude_data_len = sizeof(struct ProfilerPreludeData);
    const auto prelude_code_len = sizeof(prelude_code);
    const auto prologue_code_len = sizeof(prologue_code);

    void* prelude = VirtualAlloc(0, prelude_data_len + prelude_code_len + prologue_code_len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    void* prelude_trampoline = (void*)((uintptr_t)prelude + prelude_data_len);
    void* prologue_trampoline = (void*)((uintptr_t)prelude_trampoline + prelude_code_len);

    auto hook = register_hook(ALL, function_address, prelude_trampoline);
    reapply();

    struct ProfilerPreludeData prelude_data = {
        //nullptr,
        zone,
        (uintptr_t)hook->original,
        //0,
        (uintptr_t)&profiler_zone,
        (uintptr_t)&profiler_zone_exit,
        (uintptr_t)prologue_trampoline,
        m_tls_idx,
        0
    };

    memcpy(prologue_trampoline, prologue_code, prologue_code_len);
    memcpy(prelude, &prelude_data, prelude_data_len);
    memcpy(prelude_trampoline, prelude_code, prelude_code_len);
}

}

extern "C" void __cdecl __profiler_end(void*)
{
    OPTICK_POP();
}

extern "C" void* __cdecl __profiler_begin(const char* name, void * /*ctx*/)
{
    //spdlog::info("t:{}  {}", GetCurrentThreadId(), ((modengine::ext::ProfilerPreludeData*)ctx)->original_return_address);
    //OPTICK_EVENT(name);
    OPTICK_PUSH_DYNAMIC(name);
    return nullptr;
}
