#pragma once

#include <functional>
#include <algorithm>
#include <execution>
#include <optional>
#include <string>
#include <vector>
#include <exception>

#include <windows.h>
#include <MINT.h>

namespace modengine {

struct ScanPattern {
    std::string pattern;
    std::vector<short> wildcards;
};

class MemoryScanner {
public:
    struct MemoryScannerRegion {
    private:
        DWORD m_original_protection;
        const MEMORY_BASIC_INFORMATION& m_info;

    public:
        MemoryScannerRegion(const MEMORY_BASIC_INFORMATION& info)
            : m_info(info)
        {
            m_original_protection = m_info.AllocationProtect;

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

    MemoryScanner(HMODULE base)
    {
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

    MemoryScanner()
        : MemoryScanner((HMODULE)NtCurrentPeb()->ImageBaseAddress)
    {
    }

    std::optional<uintptr_t> find(const std::string& pattern)
    {
        for (const auto region_info : m_memory_regions) {
            const auto region = MemoryScannerRegion { region_info };
            const auto region_view = region.view();
            const auto iter = std::search(std::execution::par, region_view.begin(), region_view.end(), pattern.begin(), pattern.end());

            if (iter != region_view.end()) {
                return reinterpret_cast<uintptr_t>(&(*iter));
            }
        }

        return std::nullopt;
    }

    std::optional<uintptr_t> find(const ScanPattern& pattern)
    {
        // Fairly naive scanner for now. A regex based scanner would probably be faster down the line
        for (const auto region_info : m_memory_regions) {
            const auto region = MemoryScannerRegion { region_info };
            const auto region_view = region.view();
            std::string_view pattern_view = pattern.pattern;
            for (int i = 0; i < region_view.length() - pattern_view.length(); i++) {
                bool found = true;
                int wildcard_index = 0;
                size_t wildcard_length = pattern.wildcards.size();
                for (int j = 0; j < pattern_view.length(); j++) {
                    if (region_view[i + j] != pattern_view[j]) {
                        if (wildcard_index < wildcard_length && pattern.wildcards[wildcard_index] == j) {
                            wildcard_index++;
                            continue;
                        }
                        found = false;
                        break;
                    }
                }
                if (found) {
                    return reinterpret_cast<uintptr_t>(&(region_view[i]));
                }
            }
        }

        return std::nullopt;
    }

    bool replace_at(uintptr_t location, std::function<void(uintptr_t)> replace_callback)
    {
        DWORD original_protection;

        if (!VirtualProtect((void*)location, 0x1000 /* PAGE_SIZE */, PAGE_EXECUTE_READWRITE, &original_protection)) {
            throw std::runtime_error("Unable to change process memory protection flags");
        }

        replace_callback(location);
        VirtualProtect((void*)location, 0x1000, original_protection, &original_protection);

        return true;
    }

    bool replace(const std::string_view& pattern, std::function<void(uintptr_t)> replace_callback)
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

    bool replace(const ScanPattern& pattern, std::function<void(uintptr_t)> replace_callback)
    {
        for (const auto region_info : m_memory_regions) {
            const auto region = MemoryScannerRegion { region_info };
            const auto region_view = region.view();
            std::string_view pattern_view = pattern.pattern;

            for (int i = 0; i < region_view.length() - pattern_view.length(); i++) {
                bool found = true;
                int wildcard_index = 0;
                size_t wildcard_length = pattern.wildcards.size();
                for (int j = 0; j < pattern_view.length(); j++) {
                    if (region_view[i + j] != pattern_view[j]) {
                        if (wildcard_index < wildcard_length && pattern.wildcards[wildcard_index] == j) {
                            wildcard_index++;
                            continue;
                        }
                        found = false;
                        break;
                    }
                }
                if (found) {
                    replace_callback(reinterpret_cast<uintptr_t>(&(region_view[i])));
                    return true;
                }
            }
        }

        return false;
    }

private:
    std::vector<MEMORY_BASIC_INFORMATION> m_memory_regions;
};

}