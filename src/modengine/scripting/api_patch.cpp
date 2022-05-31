#include "modengine/scripting/api.h"
#include "modengine/util/hex_string.h"

namespace modengine::scripting {

void bind_patch_api(ModEngine* modengine, sol::state_view& lua)
{
    MemoryScanner scanner;
    auto patch_table = lua.create_named_table("Patch");

    patch_table.set_function("with_fixed_address", [&scanner](uintptr_t addr, std::string replacement) -> bool {
        return scanner.replace_at(addr, replace_with(util::hex_string(replacement.c_str())));
    });

    patch_table.set_function("with_pattern", [&scanner](std::string pattern, std::string replacement) -> bool {
        return scanner.replace(pattern, replace_with(util::hex_string(replacement.c_str())));
    });
}
}
