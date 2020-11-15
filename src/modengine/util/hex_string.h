#ifndef MODENGINE2_HEX_STRING_H
#define MODENGINE2_HEX_STRING_H

#include <cstddef>
#include <algorithm>
#include <string>
#include <stdexcept>

namespace modengine::util {

template <std::size_t N>
struct HexString {
    char data[N];

    constexpr HexString(const char _data[N]) {
        for (auto i = 0; i < N; i++) {
            data[i] = _data[i];
        }
    }

    std::string_view as_string() const {
        return {data, N};
    }
};

constexpr bool isxdigit(char ch1) {
    return (ch1 >= '0' && ch1 <= '9') || (ch1 >= 'a' && ch1 <= 'f');
}

template <std::size_t N>
constexpr auto hex_string(const char (&a)[N]) -> HexString<N / 2>
{
    const auto len = N / 2;
    char output[len];

    for (auto index = 0; index < len; index++) {
        const char ch1 = a[index * 2];
        const char ch2 = a[index * 2 + 1];

        if (!isxdigit(ch1) || !isxdigit(ch2)) {
            throw std::logic_error("A hex-string must not contain invalid characters (0-9A-F only)");
        }

        output[index] = 16 * (ch1 - '0') + (ch2 - '0');
    }

    return HexString<len> { output };
}

}

#endif //MODENGINE2_HEX_STRING_H
