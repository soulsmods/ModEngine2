set(CMAKE_CXX_STANDARD 17)
enable_language(ASM_MASM)
enable_testing()

add_library(modengine2 SHARED
        main.cpp
        modengine/extension.cpp
        modengine/mod_engine.cpp
        modengine/game_info.cpp
        modengine/hook_set.cpp
        modengine/memory_scanner.cpp
        modengine/patch.cpp
        modengine/settings.cpp
        modengine/base/allocator_table.cpp
        modengine/base/archive_file_overrides.cpp
        modengine/base/dinput_hook.cpp
        modengine/base/scyllahide_inject.cpp
        modengine/base/winsockets_disable.cpp
        modengine/debugmenu/ds3/debug_menu_ds3.cpp
        modengine/debugmenu/ds3/_DS3BootMenu.asm
        modengine/debugmenu/ds3/_DS3Debug.asm
        modengine/debugmenu/ds3/_DS3DebugDash.asm
        modengine/debugmenu/ds3/_DS3GameProperties.asm)

# Detours currently has no CMake manifests: https://github.com/microsoft/Detours/pull/48.
find_path(VCPKG_INCLUDE_DIR NAMES detours/detver.h)
find_library(DETOURS_LIBRARY detours)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

if (NOT MODENGINE_NO_DINPUT8)
    set_target_properties(modengine2 PROPERTIES OUTPUT_NAME "dinput8")
endif()

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
    target_compile_options(modengine2 PRIVATE /experimental:external /external:anglebrackets /external:W0 /W4 /WX)
endif()

target_compile_definitions(modengine2 PRIVATE
        -DSPDLOG_WCHAR_TO_UTF8_SUPPORT)

target_include_directories(modengine2 PRIVATE
        ${DETOURS_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(modengine2 PRIVATE
        ${DETOURS_LIBRARY}
        fmt::fmt-header-only
        spdlog::spdlog_header_only)

target_precompile_headers(modengine2 PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:<MINT.h$<ANGLE-R>>")