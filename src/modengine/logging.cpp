#include "logging.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace modengine::logging {

static spdlog::sink_ptr create_file_sink(const std::filesystem::path& path)
{
    auto log_file_path = path / "modengine.log";
    auto log_file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_file_path.string(), 0, 30, false, 5);

    return log_file_sink;
}

static spdlog::sink_ptr create_console_sink()
{
    // Create debug console
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONIN$", "r", stdin);

    return std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
}

std::shared_ptr<spdlog::logger> setup_logger(const std::filesystem::path& path, bool debug)
{
    auto logger = std::make_shared<spdlog::logger>("modengine");

    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
    logger->sinks().push_back(create_file_sink(path));

    if (debug) {
        logger->set_level(spdlog::level::trace);
        logger->sinks().push_back(create_console_sink());
    }

    return logger;
}

}