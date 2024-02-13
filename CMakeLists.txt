cmake_minimum_required(VERSION 3.17)
project(modengine2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

set(version "2.2.0")
set(VCPKG_TOOLS "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows")

# Detours currently has no CMake manifests: https://github.com/microsoft/Detours/pull/48.
find_path(VCPKG_INCLUDE_DIR NAMES detours/detver.h)
find_library(DETOURS_LIBRARY detours REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(crashpad CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)

set(MODENGINE_CRASH_REPORT_URL "http://localhost/" CACHE STRING "URL that minidump crash records are uploaded to")
add_compile_definitions(MODENGINE_CRASH_REPORT_URL="${MODENGINE_CRASH_REPORT_URL}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wno-microsoft-cast;-Wno-c++98-compat;-Wno-c++98-compat-pedantic;/GX;/clang:--system-header-prefix=${VCPKG_INCLUDE_DIR};/clang:--system-header-prefix=${CMAKE_CURRENT_SOURCE_DIR}/third-party>")
    else ()
        message(FATAL_ERROR "Clang frontend must be MSVC. Current frontend '${CMAKE_CXX_SIMULATE_ID}' is unsupported")
    endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/experimental:external;/external:anglebrackets;/external:W0;/W4;/bigobj>")
    add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS _CRT_SECURE_NO_WARNINGS)
endif ()

add_subdirectory(third-party)
add_subdirectory(src)
add_subdirectory(launcher)

include(ModEngineInstaller)


configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/ModEngineConfig.cmake"
        INSTALL_DESTINATION share/cmake/modengine)
set_property(TARGET modengine2 PROPERTY VERSION ${version})
set_property(TARGET modengine2 PROPERTY SOVERSION 3)
set_property(TARGET modengine2 PROPERTY
        INTERFACE_modengine2_MAJOR_VERSION 3)
set_property(TARGET modengine2 APPEND PROPERTY
        COMPATIBLE_modengine2_STRING ModEngine_MAJOR_VERSION)


write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/ModEngineConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY AnyNewerVersion)
