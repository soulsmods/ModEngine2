cmake_minimum_required(VERSION 3.17)
project(modengine2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
set(CMAKE_FIND_DEBUG_MODE TRUE)

find_library(DETOURS_LIBRARY detours REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(crashpad CONFIG REQUIRED)


if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:
                -Wno-microsoft-cast
                -Wno-c++98-compat -Wno-c++98-compat-pedantic
                -Wall -Wextra -Wshadow
                /GX
                /clang:-isystem${VCPKG_INCLUDE_DIR}>)
    else()
        message(FATAL_ERROR "Clang frontend must be MSVC. Current frontend '${CMAKE_CXX_SIMULATE_ID}' is unsupported")
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL  "MSVC")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/utf-8;/experimental:external;/external:anglebrackets;/external:W0;/W4;/WX>")
    add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS _CRT_SECURE_NO_WARNINGS)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL  "MSVC")
    add_compile_options(/utf-8)
endif()

add_subdirectory(src)
add_subdirectory(launcher)

include(CPack)

if("${MODENGINE_DIST_SCYLLA}" STREQUAL "1")
    install(FILES tools/scylla/InjectorCLIx64.exe
                  tools/scylla/HookLibraryx64.dll
                  dist/scyllahide/scylla_hide.ini
            DESTINATION modengine/tools/scyllahide)
endif()

install(FILES config/modengine.toml DESTINATION .)
install(DIRECTORY assets/debug_menu DESTINATION modengine/assets)

install(TARGETS modengine2
        RUNTIME DESTINATION .
        LIBRARY DESTINATION modengine/lib
        ARCHIVE DESTINATION modengine/lib)
