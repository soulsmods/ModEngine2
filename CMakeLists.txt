cmake_minimum_required(VERSION 3.17)
project(modengine2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

find_library(DETOURS_LIBRARY detours REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(crashpad CONFIG REQUIRED)

set(MODENGINE_CRASH_REPORT_URL "http://localhost/" CACHE STRING "URL that minidump crash records are uploaded to")
add_compile_definitions(MODENGINE_CRASH_REPORT_URL="${MODENGINE_CRASH_REPORT_URL}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:
                -Wno-microsoft-cast;
                -Wno-c++98-compat -Wno-c++98-compat-pedantic;
                -Wall -Wextra -Wshadow;
                /GX;
                /clang:-isystem${VCPKG_INCLUDE_DIR}>)
    else()
        message(FATAL_ERROR "Clang frontend must be MSVC. Current frontend '${CMAKE_CXX_SIMULATE_ID}' is unsupported")
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL  "MSVC")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/utf-8;/experimental:external;/external:anglebrackets;/external:W0;/W4;/WX>")
    add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS _CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(src)
add_subdirectory(launcher)

set(CPACK_PACKAGE_NAME "ModEngine")
set(CPACK_PACKAGE_VENDOR "https://github.com/soulsmods/modengine2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mod Engine 2 - Mod companion framework for Souls games")
set(CPACK_PACKAGE_VERSION "2.0.0.1")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "modengine2")
set(CPACK_RESOURCE_FILE_LICENSE  "${CMAKE_CURRENT_SOURCE_DIR}/dist/wix/resources/LICENSE.txt")
set(CPACK_WIX_LIGHT_EXTRA_FLAGS "-loc" "${CMAKE_SOURCE_DIR}/dist/wix/resources/messages.wxl")
set(CPACK_WIX_CANDLE_EXTRA_FLAGS
        "-dConfigWithCrashTelemetry.Path=${CMAKE_CURRENT_SOURCE_DIR}/dist/config/config-with-telemetry.toml"
        "-dConfig.Path=${CMAKE_CURRENT_SOURCE_DIR}/dist/config/config.toml"
)
set(CPACK_WIX_PATCH_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/dist/wix/patch.xml")
set(CPACK_WIX_EXTRA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/dist/wix/fragments/ui.wxs" "${CMAKE_CURRENT_SOURCE_DIR}/dist/wix/fragments/modengine-settings.wxs")
set(CPACK_WIX_UI_REF "ModEngineUI_Custom")
set(CPACK_WIX_UPGRADE_GUID "1bf93396-5829-4275-8e34-01e443d54a98")

include(CPack)

install(FILES dist/config/modengine.toml DESTINATION . RENAME config.toml.default)

install(TARGETS modengine2
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION sdk/lib
        ARCHIVE DESTINATION sdk/lib
        INCLUDES DESTINATION sdk/include)

install(TARGETS modengine2_launcher
        RUNTIME DESTINATION bin)

install(DIRECTORY dist/assets/debug_menu DESTINATION dist/assets)

install(FILES
        "${VCPKG_TOOLS}/tools/crashpad_handler.exe"
        "${VCPKG_TOOLS}/bin/zlibd1.dll"
        DESTINATION crashpad)

install(FILES tools/scylla/InjectorCLIx64.exe
        tools/scylla/HookLibraryx64.dll
        dist/scyllahide/scylla_hide.ini
        DESTINATION tools/scyllahide)
