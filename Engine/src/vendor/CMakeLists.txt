include(ExternalProject)

if (UNIX)
    set(LIB lib64)
elseif (WIN32)
    set(LIB lib)
endif ()

add_library(STB_IMAGE_WRITE STATIC
    stb/stb_image_write.cpp
)

add_library(STB_IMAGE_LOAD STATIC
    stb/stb_image.cpp
)

add_library(GLAD STATIC
    glad/src/glad.c
)
target_include_directories(GLAD PRIVATE glad/include)
# target_compile_options(GLAD PRIVATE -pg)
# target_link_options(GLAD PRIVATE -pg)

set(FMT_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/extern/fmt CACHE INTERNAL "")
ExternalProject_Add(
    FMT_EXTERN
    PREFIX ${PROJECT_SOURCE_DIR}/src/vendor/fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG befbc5fdb838040b3721d13b59e32edb122f224d
    UPDATE_DISCONNECTED FALSE
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${FMT_INSTALL_DIR}/install
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)
add_library(FMT_LIBRARY STATIC IMPORTED GLOBAL)
# target_compile_options(FMT_LIBRARY INTERFACE -pg)
# target_link_options(FMT_LIBRARY INTERFACE -pg)
set_target_properties(FMT_LIBRARY PROPERTIES
    IMPORTED_LOCATION                                                   # TODO
        ${FMT_INSTALL_DIR}/install/${LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}fmtd${CMAKE_STATIC_LIBRARY_SUFFIX}
    # INTERFACE_INCLUDE_DIRECTORIES
    INTERFACE_LINK_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/extern/fmt/install/${LIB}
)

set(GLM_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/extern/glm CACHE INTERNAL "")
ExternalProject_Add(
    GLM_EXTERN
    PREFIX ${PROJECT_SOURCE_DIR}/src/vendor/glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 8f6213d379a904f5ae910e09a114e066e25faf57
    UPDATE_DISCONNECTED FALSE
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${GLM_INSTALL_DIR}/install
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)
add_library(GLM_LIBRARY STATIC IMPORTED GLOBAL)
# target_compile_options(GLM_LIBRARY INTERFACE -pg)
# target_link_options(GLM_LIBRARY INTERFACE -pg)
set_target_properties(GLM_LIBRARY PROPERTIES
    IMPORTED_LOCATION
        ${GLM_INSTALL_DIR}/install/${LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}glm${CMAKE_STATIC_LIBRARY_SUFFIX})
    INTERFACE_LINK_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/extern/glm/install/${LIB}

set(ENVY_VENDOR_INCLUDES
    ${PROJECT_SOURCE_DIR}/src/vendor
    ${PROJECT_SOURCE_DIR}/src/vendor/glad/include
    ${FMT_INSTALL_DIR}/install/include
    ${GLM_INSTALL_DIR}/install/include
CACHE INTERNAL "")