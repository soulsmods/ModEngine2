#pragma once

#include <filesystem>

namespace modengine::util {

std::filesystem::path system_directory();
uintptr_t rva2addr(ptrdiff_t offset);

}