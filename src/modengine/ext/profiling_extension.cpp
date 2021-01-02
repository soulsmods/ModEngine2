#include "modengine/ext/profiling_extension.h"

namespace modengine::ext {

struct ProfilerPreludeData {
    void* ctx;
    const char* zone;
    uintptr_t original_address;
    uintptr_t original_return_address;
    uintptr_t profiler_entry_address;
    uintptr_t profiler_exit_address;
    uintptr_t profiler_prologue_address;
    void* rbx;
};

void ProfilingExtension::on_attach()
{
    install_profiler_zone(0x14007d5e0, "test_zone");
}

void ProfilingExtension::on_detach()
{
}

void ProfilingExtension::install_profiler_zone(uintptr_t function_address, const char* zone)
{
    unsigned char prelude_code[] = {
        0x48, 0x89, 0x1d, 0xf1, 0xff, 0xff, 0xff, // mov qword ptr [rip-15], rbx
        0x48, 0x8D, 0x1D, 0xB2, 0xFF, 0xFF, 0xFF, // lea rbx, [rip-70]
        0xFF, 0x63, 0x20,                         // jmp [rbx+24]
    };

    unsigned char prologue_code[] = {
        0x48, 0x8D, 0x1D, 0xA8, 0xFF, 0xFF, 0xFF, // lea rbx, [rip-80]
        0xFF, 0x63, 0x28                          // jmp [rbx+48]
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
        nullptr,
        zone,
        (uintptr_t)hook->original,
        0,
        (uintptr_t)&profiler_zone,
        (uintptr_t)&profiler_zone_exit,
        (uintptr_t)prologue_trampoline,
        0x0,
    };

    memcpy(prologue_trampoline, prologue_code, prologue_code_len);
    memcpy(prelude, &prelude_data, prelude_data_len);
    memcpy(prelude_trampoline, prelude_code, prelude_code_len);
}

}
extern "C" void __profiler_end(void*)
{
}

extern "C" void* __profiler_begin(const char*)
{
    return nullptr;
}
