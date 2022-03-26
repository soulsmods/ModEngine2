#include "modengine/util/hex_string.h"

#include <spdlog/spdlog.h>


namespace modengine::util {

using namespace spdlog;

char hex2bin(char c)
{
    if ('0' <= c && c <= '9') return (c - '0');
    if ('A' <= c && c <= 'F') return (c - 'A' + 10);
    if ('a' <= c && c <= 'f') return (c - 'a' + 10);
    return -1;
}

std::string hex_string(const char* input)
{
    std::stringstream input_stream { input };
    std::stringstream output;

    while (!input_stream.eof()) {
        char lo;
        char hi;

        input_stream >> lo;
        input_stream >> hi;

        if (input_stream.eof()) {
            break;
        }

        output << (char) (16 * (hex2bin(lo)) + (hex2bin(hi)));
    }

    return output.str();
}

ScanPattern hex_aob(const char* input)
{
    ScanPattern result;
    std::stringstream input_stream { input };
    std::stringstream output;

    short index = 0;
    while (!input_stream.eof()) {
        char lo;
        char hi;

        input_stream >> lo;
        input_stream >> hi;

        if (input_stream.eof()) {
            break;
        }

        if (lo == '?' && hi == '?') {
            output << (char)0;
            result.wildcards.push_back(index);
        } else {
            output << (char) (16 * (hex2bin(lo)) + (hex2bin(hi)));
        }
        index++;
    }

    result.pattern = output.str();
    return result;
}

}