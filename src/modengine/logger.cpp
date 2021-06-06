#include <modengine/extension_logger.h>

#include <string>

namespace modengine {

void ExtensionLogger::log(ExtensionLoggerLevel level, const char *message, size_t size)
{
    std::string_view message_str{message, size};
    spdlog::log(level, message_str);
}
}