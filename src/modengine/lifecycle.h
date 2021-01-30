#pragma once

#include <vector>

namespace modengine {

struct Slot {
    void* object;
    void* func;
};

template <typename F>
struct Signal;

template <typename... Args>
struct Signal<void(Args...)> {
public:
    template <auto M, class C>
    void connect(C* object)
    {
        const auto func = reinterpret_cast<void*>(+[](void* obj, Args... args) { ((*reinterpret_cast<C**>(obj))->*M)(args...); });
        const auto slot = Slot { object, func };

        m_slots.push_back(slot);
    }

    void operator()(Args&&... args) const
    {
        for (const auto& slot : m_slots) {
            reinterpret_cast<void (*)(void*, Args...)>(slot.func)(&slot.object, std::forward<Args>(args)...);
        }
    }

private:
    std::vector<Slot> m_slots;
};


}