cmake_minimum_required(VERSION 3.17)
project(modengine2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

set(version "2.0.1")


# Detours currently has no CMake manifests: https://github.com/microsoft/Detours/pull/48.
find_path(VCPKG_INCLUDE_DIR NAMES detours/detver.h)
find_library(DETOURS_LIBRARY detours REQUIRED)
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
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/utf-8;/experimental:external;/external:anglebrackets;/external:W0;/W4;/bigobj>")
    add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS _CRT_SECURE_NO_WARNINGS)
endif ()

add_subdirectory(third-party)
add_subdirectory(src)
add_subdirectory(launcher)

set(CPACK_PACKAGE_NAME "ModEngine")
set(CPACK_PACKAGE_VENDOR "https://github.com/soulsmods/modengine2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mod Engine 2 - Mod companion framework for Souls games")
set(CPACK_PACKAGE_VERSION "2.0.0.1")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "modengine2")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/dist/wix/resources/LICENSE.txt")
set(CPACK_WIX_LIGHT_EXTRA_FLAGS "-loc" "${CMAKE_SOURCE_DIR}/dist/wix/resources/messages.wxl")
set(CPACK_WIX_CANDLE_EXTRA_FLAGS
        "-dConfigWithCrashTelemetry.Path=${CMAKE_CURRENT_SOURCE_DIR}/dist/config/config-with-telemetry.toml"
        "-dConfig.Path=${CMAKE_CURRENT_SOURCE_DIR}/dist/config/config.toml"
        )
set(CPACK_WIX_PATCH_FILE "${CMAKE_CURRENT_SOURCE_DIR}/dist/wix/patch.xml")
set(CPACK_WIX_EXTRA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/dist/wix/fragments/ui.wxs" "${CMAKE_CURRENT_SOURCE_DIR}/dist/wix/fragments/modengine-settings.wxs")
set(CPACK_WIX_UI_REF "ModEngineUI_Custom")
set(CPACK_WIX_UPGRADE_GUID "1bf93396-5829-4275-8e34-01e443d54a98")

include(CMakePackageConfigHelpers)
include(CPack)

install(FILES dist/config/modengine.toml DESTINATION . RENAME config.toml.default)

install(TARGETS modengine2 EXPORT ModEngineTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
)

install(TARGETS modengine2_launcher
        RUNTIME DESTINATION bin)

install(DIRECTORY
        "${VCPKG_INCLUDE_DIR}/toml++"
        "${VCPKG_INCLUDE_DIR}/sol"
        "${VCPKG_INCLUDE_DIR}/luajit"
        DESTINATION include)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party/MINT/MINT.h"
        DESTINATION include)

install(FILES
        "${VCPKG_INCLUDE_DIR}/../lib/lua51.lib"
        DESTINATION lib)

install(DIRECTORY include DESTINATION .)
install(DIRECTORY dist/assets/debug_menu DESTINATION assets)

install(FILES
        "${VCPKG_TOOLS}/bin/lua51.dll"
        DESTINATION bin)

install(FILES
        "${VCPKG_TOOLS}/tools/crashpad_handler.exe"
        "${VCPKG_TOOLS}/bin/zlibd1.dll"
        DESTINATION crashpad)

install(FILES tools/scylla/InjectorCLIx64.exe
        tools/scylla/HookLibraryx64.dll
        dist/scyllahide/scylla_hide.ini
        DESTINATION tools/scyllahide)

install(EXPORT ModEngineTargets DESTINATION share/cmake/modengine)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/ModEngineConfig.cmake"
        INSTALL_DESTINATION share/cmake/modengine
)
set_property(TARGET modengine2 PROPERTY VERSION ${version})
set_property(TARGET modengine2 PROPERTY SOVERSION 3)
set_property(TARGET modengine2 PROPERTY
        INTERFACE_modengine2_MAJOR_VERSION 3)
set_property(TARGET modengine2 APPEND PROPERTY
        COMPATIBLE_modengine2_STRING ModEngine_MAJOR_VERSION
        )

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/ModEngineConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/ModEngineConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/ModEngineConfigVersion.cmake"
        DESTINATION share/cmake/ModEngine
)