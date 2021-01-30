set(CMAKE_CXX_STANDARD 17)
enable_language(ASM_MASM)
enable_testing()

configure_file("modengine/version.cpp" "${CMAKE_CURRENT_BINARY_DIR}/version.cpp" @ONLY)

# Optick source
file(GLOB OPTICK_SRC "../dependencies/optick/*.cpp")

add_library(modengine2 SHARED
        main.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
        modengine/extension.cpp
        modengine/mod_engine.cpp
        modengine/game_info.cpp
        modengine/hook_set.cpp
        modengine/memory_scanner.cpp
        modengine/patch.cpp
        modengine/settings.cpp
        modengine/ext/base/base_extension.cpp
        modengine/ext/debug_menu/ds3/debug_menu_ds3.cpp
        modengine/ext/debug_menu/ds3/_DS3BootMenu.asm
        modengine/ext/debug_menu/ds3/_DS3Debug.asm
        modengine/ext/debug_menu/ds3/_DS3DebugDash.asm
        modengine/ext/debug_menu/ds3/_DS3GameProperties.asm
        modengine/ext/crash_handler/crash_handler_extension.cpp
        modengine/ext/mod_loader/archive_file_overrides.cpp
        modengine/ext/mod_loader/mod_loader_extension.cpp
        modengine/ext/profiling/profiling_extension.cpp
        modengine/ext/profiling/profiler_trampoline.asm
        modengine/ext/profiling/main_loop.cpp
        modengine/ext/profiling/thread_hooks.cpp
        modengine/ext/scylla/scyllahide_extension.cpp
        modengine/util/hex_string.cpp
        ${OPTICK_SRC})

# Detours currently has no CMake manifests: https://github.com/microsoft/Detours/pull/48.
find_path(VCPKG_INCLUDE_DIR NAMES detours/detver.h)
find_path(VCPKG_TOOLS NAMES tools/crashpad_handler.exe)

find_library(DETOURS_LIBRARY detours)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(crashpad CONFIG REQUIRED)

if (NOT MODENGINE_NO_DINPUT8)
    set_target_properties(modengine2 PROPERTIES OUTPUT_NAME "dinput8")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        target_compile_options(modengine2 PRIVATE $<$<COMPILE_LANGUAGE:CXX>:
                -Wno-microsoft-cast
                -Wno-c++98-compat -Wno-c++98-compat-pedantic
                -Wall -Wextra -Wshadow
                /GX
                /clang:-isystem${VCPKG_INCLUDE_DIR}>)
    else()
        message(FATAL_ERROR "Clang frontend must be MSVC. Current frontend '${CMAKE_CXX_SIMULATE_ID}' is unsupported")
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL  "MSVC")
    target_compile_options(modengine2 PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/utf-8 /experimental:external /external:anglebrackets /external:W0 /W4 /WX>)
    target_compile_definitions(modengine2 PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_definitions(modengine2 PRIVATE
        -DSPDLOG_WCHAR_TO_UTF8_SUPPORT)

# Disable optick GPU for now until D3D11 backend
target_compile_definitions(modengine2 PRIVATE OPTICK_ENABLE_GPU=0)

target_include_directories(modengine2 PRIVATE
        ${DETOURS_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        "../dependencies/optick")

target_link_libraries(modengine2 PRIVATE
        ${DETOURS_LIBRARY}
        fmt::fmt-header-only
        spdlog::spdlog_header_only
        crashpad)

target_precompile_headers(modengine2 PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:<MINT.h$<ANGLE-R>>")

install(FILES
        "${VCPKG_TOOLS}/tools/crashpad_handler.exe"
        "${VCPKG_TOOLS}/bin/zlibd1.dll"
        DESTINATION .)