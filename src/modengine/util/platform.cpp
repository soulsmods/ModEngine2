#include "platform.h"

#include <windows.h>

namespace modengine::util {
std::filesystem::path&& system_directory()
{
    TCHAR system_folder[MAX_PATH];
    if (!GetSystemDirectory(system_folder, MAX_PATH)) {
        throw std::runtime_error("GetSystemDirectory(..., MAX_PATH) failed");
    }

    return std::move(std::filesystem::path(system_folder));
}

uintptr_t rva2addr(ptrdiff_t offset)
{
    return (uintptr_t)GetModuleHandle(nullptr)+offset;
}
}