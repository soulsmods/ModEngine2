#pragma once

#include <filesystem>
#include <memory>

#include <spdlog/spdlog.h>

namespace modengine::logging {

std::shared_ptr<spdlog::logger> setup_logger(const std::filesystem::path& path, bool debug);

}