cmake_minimum_required(VERSION 3.17)
project(modengine2)

add_subdirectory(src)

include(CPack)

if (MODENGINE_DIST_SCYLLA)
    install(FILES tools/scylla/InjectorCLIx64.exe
                  tools/scylla/HookLibraryx64.dll
                  dist/scyllahide/scylla_hide.ini
            DESTINATION scyllahide)
endif()

install(FILES config/modengine.toml DESTINATION .)
install(TARGETS modengine2
        RUNTIME DESTINATION .
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
