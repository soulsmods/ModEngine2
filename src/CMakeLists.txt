set(CMAKE_CXX_STANDARD 17)
enable_language(ASM_MASM)
enable_testing()

# Proto file
get_filename_component(me2_proto "../ModEngine2RPC/mapserver.proto" ABSOLUTE)
get_filename_component(me2_proto_path "${me2_proto}" PATH)

# Generated sources
set(me2_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/mapserver.pb.cc")
set(me2_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/mapserver.pb.h")
set(me2_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/mapserver.grpc.pb.cc")
set(me2_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/mapserver.grpc.pb.h")
message(STATUS "Trying ${me2_proto}")
add_custom_command(
      OUTPUT "${me2_proto_srcs}" "${me2_proto_hdrs}" "${me2_grpc_srcs}" "${me2_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${me2_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${me2_proto}"
      DEPENDS "${me2_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

configure_file("modengine/version.cpp" "${CMAKE_CURRENT_BINARY_DIR}/version.cpp" @ONLY)

# Optick source
file(GLOB OPTICK_SRC "../dependencies/optick/*.cpp")

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})


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
        modengine/ext/grpc_server/grpc_server_extension.cpp
        modengine/util/hex_string.cpp
        ${me2_proto_srcs}
        ${me2_grpc_srcs}
        ${OPTICK_SRC})

# Detours currently has no CMake manifests: https://github.com/microsoft/Detours/pull/48.
find_path(VCPKG_INCLUDE_DIR NAMES detours/detver.h)
find_path(VCPKG_TOOLS NAMES tools/crashpad_handler.exe)

target_compile_definitions(modengine2 PRIVATE
        -DSPDLOG_WCHAR_TO_UTF8_SUPPORT)

# Disable optick GPU for now until D3D11 backend
target_compile_definitions(modengine2 PRIVATE OPTICK_ENABLE_GPU=0)

target_include_directories(modengine2 PUBLIC ../include)

target_include_directories(modengine2 PRIVATE
        ${DETOURS_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        "../dependencies/optick")

target_link_options(modengine2 PRIVATE "/export:DetourFinishHelperProcess,@1")

target_link_libraries(modengine2 PRIVATE
        ${DETOURS_LIBRARY}
        fmt::fmt-header-only
        spdlog::spdlog_header_only
        crashpad
        gRPC::grpc++
        protobuf::libprotobuf)

target_precompile_headers(modengine2 PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:<MINT.h$<ANGLE-R>>")
