cmake_minimum_required(VERSION 3.17)
project(modengine2)

add_subdirectory(src)

include(CPack)

install(FILES config/modengine.toml DESTINATION .)
install(TARGETS modengine2
        RUNTIME DESTINATION .
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
