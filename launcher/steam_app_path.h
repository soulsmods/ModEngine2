#pragma once

#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

std::optional<fs::path> get_game_path(std::wstring_view app_id);

