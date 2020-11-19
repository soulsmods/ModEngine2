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
        m_original_protection = m_info.AllocationProtect;

        if ((m_original_protection & (PAGE_EXECUTE_READWRITE | PAGE_READWRITE)) == 0) {
            if (!VirtualProtect(m_info.BaseAddress, m_info.RegionSize, PAGE_READWRITE, &m_original_protection)) {
                throw std::runtime_error("Unable to change process memory protection flags");
            }
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
    const auto base = NtCurrentPeb()->ImageBaseAddress;
    const auto image = RtlImageNtHeader(base);
    const auto start = reinterpret_cast<uintptr_t>(base);
    const auto end = start + image->OptionalHeader.SizeOfImage;

    HANDLE process = GetCurrentProcess();
    MEMORY_BASIC_INFORMATION info;

    for (uintptr_t next = 0; VirtualQueryEx(process, (void*)next, &info, sizeof(info)) == sizeof(info); next += info.RegionSize) {
        if (info.State == MEM_COMMIT) {
            info.BaseAddress = (void*)next;

            if (next >= start && next + info.RegionSize <= end) {
                m_memory_regions.push_back(info);
            }
        }
    }
}

bool MemoryScanner::replace(const std::string_view& pattern, std::function<void(uintptr_t)> replace_callback)
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