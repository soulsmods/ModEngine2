set(CMAKE_CXX_STANDARD 17)

add_library(modengine2 SHARED
        main.cpp
        modengine/mod_engine.cpp
        modengine/hook_set.cpp
        modengine/hooks/anti_debug.cpp
        modengine/hooks/direct_input.cpp)

# Detours currently has no CMake manifests: https://github.com/microsoft/Detours/pull/48.
find_library(DETOURS_LIBRARY detours)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_include_directories(modengine2 PRIVATE
        ${DETOURS_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(modengine2 PRIVATE
        ${DETOURS_LIBRARY}
        fmt::fmt-header-only
        spdlog::spdlog_header_only)

target_precompile_headers(modengine2 PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:<MINT.h$<ANGLE-R>>")