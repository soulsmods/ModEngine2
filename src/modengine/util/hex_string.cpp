#include "modengine/util/hex_string.h"

namespace modengine::util {

std::string hex_string(const char* input)
{
    std::stringstream input_stream { input };
    std::stringstream output;

    while (!input_stream.eof()) {
        char lo;
        char hi;

        input_stream >> lo;
        input_stream >> hi;

        output << (char)(16 * (tolower(lo) - '0') + (tolower(hi) - '0'));
    }

    return output.str();
}

}