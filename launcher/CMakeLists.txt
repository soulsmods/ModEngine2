set(CMAKE_CXX_STANDARD 17)
enable_language(ASM_MASM)
enable_testing()

set(VDF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../third-party/ValveFileVdf")

add_executable(modengine2_launcher launcher.cpp steam_app_path.cpp)

find_path(VCPKG_INCLUDE_DIR NAMES detours/detver.h)
find_package(cli11 CONFIG REQUIRED)

target_include_directories(modengine2_launcher PRIVATE
        ${VCPKG_INCLUDE_DIR}
        ${VDF_INCLUDE_DIR}
        ../include)

target_link_libraries(modengine2_launcher PRIVATE
        ${DETOURS_LIBRARY}
        fmt::fmt-header-only
        spdlog::spdlog_header_only
        crashpad)