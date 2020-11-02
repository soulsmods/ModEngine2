set(CMAKE_CXX_STANDARD 17)

add_library(modengine2 SHARED
        main.cpp
        modengine/mod_engine.cpp
        modengine/hook_set.cpp
        modengine/hooks/archive_file_overrides.cpp
        modengine/hooks/anti_debug.cpp
        modengine/hooks/direct_input.cpp modengine/game_type.cpp)

# Detours currently has no CMake manifests: https://github.com/microsoft/Detours/pull/48.
find_path(VCPKG_INCLUDE_DIR NAMES detours/detver.h)
find_library(DETOURS_LIBRARY detours)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        target_compile_options(modengine2 PRIVATE
                -Wno-microsoft-cast
                -Wno-c++98-compat -Wno-c++98-compat-pedantic
                -Wall -Wextra -Wshadow
                /GX
                /clang:-isystem${VCPKG_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "Clang frontend must be MSVC. Current frontend '${CMAKE_CXX_SIMULATE_ID}' is unsupported")
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL  "MSVC")
    target_compile_options(modengine2 PRIVATE /external:anglebrackets /W4 /WX)
endif()

target_include_directories(modengine2 PRIVATE
        ${DETOURS_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(modengine2 PRIVATE
        ${DETOURS_LIBRARY}
        fmt::fmt-header-only
        spdlog::spdlog_header_only)

target_precompile_headers(modengine2 PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:<MINT.h$<ANGLE-R>>")