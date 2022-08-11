#include "modengine/scripting/api.h"
#include "modengine/util/hex_string.h"

namespace modengine::scripting {

void bind_patch_api(ModEngine* modengine, sol::state_view& lua)
{
    MemoryScanner scanner;

    lua.set_function("hexstring", [](std::string value) -> std::string {
        return util::hex_string(value.c_str());
    });

    lua.set_function("patch_at_address", [&scanner](uintptr_t addr, std::string replacement) -> bool {
        return scanner.replace_at(addr, replace_with(replacement.c_str()));
    });

    lua.set_function("patch_with_pattern", [&scanner](std::string pattern, std::string replacement) -> bool {
        return scanner.replace(pattern, replace_with(replacement.c_str()));
    });
}
}
