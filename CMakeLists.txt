cmake_minimum_required(VERSION 3.17)
project(modengine2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

add_subdirectory(src)

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
