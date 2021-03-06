vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO microsoft/Detours
    REF 7f33ae35077f2f298d9eb5b1446dc4d2eafaa46b
    SHA512 0f613cf31f4d027b71501a0e93b192676268434e9dc8e83ac1f0797fea22c2c5c25175a598562093690da795a7e5275b5130356939b54d45c7dcbfe5e6bc4c82
    HEAD_REF master
)

vcpkg_build_nmake(
    SOURCE_PATH ${SOURCE_PATH}
    PROJECT_SUBPATH "src"
    PROJECT_NAME "Makefile"
    OPTIONS "PROCESSOR_ARCHITECTURE=${VCPKG_TARGET_ARCHITECTURE}"
    NO_DEBUG
)

file(INSTALL ${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}/lib.${VCPKG_TARGET_ARCHITECTURE}/ DESTINATION ${CURRENT_PACKAGES_DIR}/lib)
file(INSTALL ${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}/lib.${VCPKG_TARGET_ARCHITECTURE}/ DESTINATION ${CURRENT_PACKAGES_DIR}/debug/lib)
file(INSTALL ${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}/include DESTINATION ${CURRENT_PACKAGES_DIR}/include RENAME detours)

file(INSTALL ${SOURCE_PATH}/LICENSE.md DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
