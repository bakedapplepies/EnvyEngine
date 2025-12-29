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
    PREFIX ${FMT_INSTALL_DIR}
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG befbc5fdb838040b3721d13b59e32edb122f224d
    UPDATE_DISCONNECTED FALSE
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${FMT_INSTALL_DIR}/install
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)
add_library(FMT_LIBRARY STATIC IMPORTED)
# target_compile_options(FMT_LIBRARY INTERFACE -pg)
# target_link_options(FMT_LIBRARY INTERFACE -pg)
set_target_properties(FMT_LIBRARY PROPERTIES IMPORTED_LOCATION ${FMT_INSTALL_DIR}/install/${LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}fmt${CMAKE_STATIC_LIBRARY_SUFFIX})

set(GLM_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/extern/glm CACHE INTERNAL "")
ExternalProject_Add(
    GLM_EXTERN
    PREFIX ${GLM_INSTALL_DIR}
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 8f6213d379a904f5ae910e09a114e066e25faf57
    UPDATE_DISCONNECTED FALSE
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${GLM_INSTALL_DIR}/install
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)
add_library(GLM_LIBRARY STATIC IMPORTED)
# target_compile_options(GLM_LIBRARY INTERFACE -pg)
# target_link_options(GLM_LIBRARY INTERFACE -pg)
set_target_properties(GLM_LIBRARY PROPERTIES IMPORTED_LOCATION ${GLM_INSTALL_DIR}/install/${LIB}/${CMAKE_STATIC_LIBRARY_PREFIX}glm${CMAKE_STATIC_LIBRARY_SUFFIX})
