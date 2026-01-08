cmake_minimum_required(VERSION 3.26.0)
project(EnvyEngine CXX C)

add_subdirectory(vendor)

set(ENVY_VENDOR_INCLUDES
    ${PROJECT_SOURCE_DIR}/vendor
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${FMT_INSTALL_DIR}/install/include
    ${GLM_INSTALL_DIR}/install/include
CACHE INTERNAL "")

add_subdirectory(src)