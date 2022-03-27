set(CPACK_PACKAGE_NAME "ModEngine")
set(CPACK_PACKAGE_VENDOR "https://github.com/soulsmods/modengine2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mod Engine 2 - Mod companion framework for Souls games")
set(CPACK_PACKAGE_VERSION "2.0.0.1")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "modengine2")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installer/resources/LICENSE.txt")
set(CPACK_WIX_UPGRADE_GUID "1bf93396-5829-4275-8e34-01e443d54a98")
set(CPACK_WIX_UI_REF "WixUI_Mondo")
set(CPACK_COMPONENTS_ALL application sdk)
set(CPACK_COMPONENT_APPLICATION_REQUIRED)
set(CPACK_ALL_INSTALL_TYPES Standard Developer)
set(CPACK_COMPONENT_SDK_INSTALL_TYPES Developer Standard)
set(CPACK_COMPONENT_APPLICATION_INSTALL_TYPES Standard)

install(TARGETS modengine2
        EXPORT ModEngineTargets
        RUNTIME DESTINATION modengine2/bin COMPONENT application
        LIBRARY DESTINATION modengine2/lib COMPONENT sdk
        ARCHIVE DESTINATION modengine2/lib COMPONENT sdk
        INCLUDES DESTINATION modengine2/include
)

install(TARGETS modengine2_launcher
        RUNTIME DESTINATION .
        COMPONENT application)

install(DIRECTORY
        "${VCPKG_INCLUDE_DIR}/toml++"
        "${VCPKG_INCLUDE_DIR}/sol"
        "${VCPKG_INCLUDE_DIR}/fmt"
        "${VCPKG_INCLUDE_DIR}/spdlog"
        DESTINATION modengine2/include
        COMPONENT sdk)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party/MINT/MINT.h"
        "${VCPKG_INCLUDE_DIR}/lauxlib.h"
        "${VCPKG_INCLUDE_DIR}/lua.h"
        "${VCPKG_INCLUDE_DIR}/lua.hpp"
        "${VCPKG_INCLUDE_DIR}/luaconf.h"
        "${VCPKG_INCLUDE_DIR}/lualib.h"

        DESTINATION modengine2/include
        COMPONENT sdk)

install(FILES
        "${VCPKG_INCLUDE_DIR}/../lib/lua.lib"
        DESTINATION modengine2/lib
        COMPONENT sdk)

install(DIRECTORY include DESTINATION modengine2 COMPONENT sdk)
install(DIRECTORY installer/assets/debug_menu DESTINATION modengine2/assets COMPONENT application)

install(FILES
        "${VCPKG_TOOLS}/bin/lua.dll"
        DESTINATION modengine2/bin
        COMPONENT application)

install(FILES
        "${VCPKG_TOOLS}/tools/crashpad_handler.exe"
        "${VCPKG_TOOLS}/bin/zlib1.dll"
        DESTINATION modengine2/crashpad
        COMPONENT application)

install(FILES tools/scylla/InjectorCLIx64.exe
        tools/scylla/HookLibraryx64.dll
        installer/assets/scyllahide/scylla_hide.ini
        DESTINATION modengine2/tools/scyllahide
        COMPONENT application)

install(FILES
        installer/dist/config_darksouls3.toml
        installer/dist/config_eldenring.toml
        installer/dist/launchmod_darksouls3.bat
        installer/dist/launchmod_eldenring.bat
        DESTINATION .
        COMPONENT application)
install(DIRECTORY DESTINATION "mod")

install(EXPORT ModEngineTargets DESTINATION modengine2/share/cmake/modengine COMPONENT sdk)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/ModEngineConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/ModEngineConfigVersion.cmake"
        DESTINATION modengine2/share/cmake/ModEngine
        COMPONENT sdk)

include(CPackComponent)

cpack_add_install_type(Standard DISPLAY_NAME "Standard installation")
cpack_add_install_type(Developer DISPLAY_NAME "Developer installation")
cpack_add_component(application HIDDEN)

cpack_add_component(sdk
        DISPLAY_NAME "SDK"
        DESCRIPTION "SDK used to develop extensions for Mod Engine"
        DEPENDS application)


include(CMakePackageConfigHelpers)
include(CPack)
