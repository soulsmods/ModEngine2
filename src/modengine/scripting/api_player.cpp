#include "modengine/scripting/api.h"
#include "modengine/util/hex_string.h"

namespace modengine::scripting {

template <typename T, typename... Offsets>
struct DeepPointer {
    using DeepPointerChain = std::tuple<Offsets...>;

    DeepPointer(uintptr_t _base, Offsets... _chain)
        : base(_base)
        , chain(_chain...)
    {
    }

    T* operator->()
    {
        return this->resolve();
    }

    T& operator*()
    {
        return *(this->resolve());
    }

private:
    T* resolve()
    {
        auto addr = (void*)base;
        std::apply([&addr](auto&&... args) { ((addr = (void*)(*(uintptr_t*)addr + args)), ...); }, chain);

        return (T*)addr;
    }

private:
    uintptr_t base;
    DeepPointerChain chain;
};

template <typename T, typename... Offsets>
auto deep_pointer(const uintptr_t base, Offsets... offsets)
{
    return DeepPointer<T, Offsets...>(base, std::forward<Offsets>(offsets)...);
}

struct Player {
    DeepPointer<float, int> x;
};

struct WorldChrMan {
    WorldChrMan(uintptr_t base)
        : player(deep_pointer<Player>(base, 0x80, 0))
    {
        *(player->x) = 5;
    }

public:
    DeepPointer<Player, int, int> player;
};

void bind_player_api(ModEngine* modengine, sol::state_view& lua)
{
    auto pointer = deep_pointer<int>(0, 1, 2, 3);
    auto table = lua.create_named_table("player");
}
}