#pragma once

#include <mutex>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>

namespace modengine {

using spdlog::level::level_enum;
using ExtensionLoggerLevel = spdlog::level::level_enum;

struct ExtensionLogger {
    virtual void log(ExtensionLoggerLevel level, const char* message, size_t message_len);
};

namespace detail {

template <typename Mutex>
class ExtensionLoggerSinkBase : public spdlog::sinks::base_sink<Mutex> {
public:
    explicit ExtensionLoggerSinkBase(ExtensionLogger *backend)
    {
        m_backend.reset(backend);
    }

    ExtensionLoggerSinkBase(const ExtensionLoggerSinkBase&) = delete;
    ExtensionLoggerSinkBase(ExtensionLoggerSinkBase&&) = delete;
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        m_backend->log(msg.level, msg.payload.data(), msg.payload.size());
    }

    void flush_() override
    {
        // Do nothing because statement executed in sink_it_().
    }

    void set_pattern_(const std::string& pattern) override
    {
        // Don't format log message.
    }

    void set_formatter_(std::unique_ptr<spdlog::formatter> sink_formatter) override
    {
        // Don't format log message.
    }

private:
    std::unique_ptr<ExtensionLogger> m_backend;
};

using ExtensionLoggerSink = ExtensionLoggerSinkBase<std::mutex>;

}
}