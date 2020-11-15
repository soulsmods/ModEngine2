#include "modengine/memory_scanner.h"

#include <MINT.h>
#include <algorithm>
#include <execution>
#include <string>
#include <functional>

namespace modengine {

struct MemoryScannerRegion {
private:
    DWORD m_original_protection;
    const MEMORY_BASIC_INFORMATION& m_info;

public:
    MemoryScannerRegion(const MEMORY_BASIC_INFORMATION& info)
        : m_info(info)
    {
        if (!VirtualProtect(m_info.BaseAddress, m_info.RegionSize, PAGE_EXECUTE_READWRITE, &m_original_protection)) {
            throw std::runtime_error("Unable to change process memory protection flags");
        }
    }

    ~MemoryScannerRegion()
    {
        DWORD prot;
        (void)VirtualProtect(m_info.BaseAddress, m_info.RegionSize, m_original_protection, &prot);
    }

    std::string_view view() const
    {
        return std::string_view { reinterpret_cast<char*>(m_info.BaseAddress), m_info.RegionSize };
    }
};

MemoryScanner::MemoryScanner()
{
    HANDLE process = GetCurrentProcess();
    MEMORY_BASIC_INFORMATION info;

    for (uintptr_t next = 0; VirtualQueryEx(process, (void*)next, &info, sizeof(info)) == sizeof(info); next += info.RegionSize) {
        if (info.State == MEM_COMMIT) {
            m_memory_regions.push_back(info);
        }
    }

    m_base_address = reinterpret_cast<uintptr_t>(NtCurrentPeb()->ImageBaseAddress);
}

bool MemoryScanner::replace(const std::string_view & pattern, std::function<void(uintptr_t)> replace_callback)
{
    for (const auto region_info : m_memory_regions) {
        const auto region = MemoryScannerRegion { region_info };
        const auto region_view = region.view();
        const auto iter = std::search(std::execution::par, region_view.begin(), region_view.end(), pattern.begin(), pattern.end());

        if (iter != region_view.end()) {
            replace_callback(reinterpret_cast<uintptr_t>(&(*iter)));
            
            return true;
        }
    }

    return false;
}

}