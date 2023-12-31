# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/assign
    REF boost-1.83.0
    SHA512 cdcab29f429528aff53c009e6ee822c70e763534561be97be5ceb806d58ea2906b19b3e81f667f27fb08a320385a9207122d5bdba704e314eda040377d63ea6a
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})
