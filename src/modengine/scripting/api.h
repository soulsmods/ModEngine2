#pragma once

#include "modengine/mod_engine.h"
#include "modengine/script_host.h"

namespace modengine::scripting {

/// Creates a simple Lua binding to the Mod Engine patch functionality.
/// Script authors can create search/replace patches by calling the `Patch.with_fixed_address`
/// `Patch.with_pattern`.
///
/// \code{.lua}
/// Patch.with_fixed_address(0x14000000, "90909090") ; write NOPs to 0x140000000
/// Patch.with_pattern(".sl2", ".sl3") ; replace the literal string ".sl2" with ".sl3"
/// \endcode
void bind_patch_api(ModEngine* modengine, sol::state_view& lua);

}