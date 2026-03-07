include(FetchContent)

if (UNIX)
    set(LIB lib64)
elseif (WIN32)
    set(LIB lib)
endif ()

# ===================================================================
add_library(STB_IMAGE_WRITE STATIC
    stb/stb_image_write.cpp
)
target_include_directories(STB_IMAGE_WRITE 
    PUBLIC
        $<INSTALL_INTERFACE:include/vendor/stb>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/vendor/stb>
)

add_library(STB_IMAGE_LOAD STATIC
    stb/stb_image.cpp
)
target_include_directories(STB_IMAGE_LOAD
    PUBLIC
        $<INSTALL_INTERFACE:include/vendor/stb>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/vendor/stb>
)

add_library(GLAD STATIC
    glad/glad.c
)
target_include_directories(GLAD 
    PUBLIC
        $<INSTALL_INTERFACE:include/vendor>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/vendor>
)
# target_compile_options(GLAD PRIVATE -pg)
# target_link_options(GLAD PRIVATE -pg)

# ===================================================================
FetchContent_Declare(fmt_fetch
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG befbc5fdb838040b3721d13b59e32edb122f224d
)
FetchContent_MakeAvailable(fmt_fetch)
FetchContent_GetProperties(fmt_fetch
    SOURCE_DIR fmt_SOURCE_DIR
    BINARY_DIR fmt_BINARY_DIR
)
add_library(FMT_LIBRARY INTERFACE IMPORTED GLOBAL)
target_include_directories(FMT_LIBRARY INTERFACE
    $<BUILD_INTERFACE:${fmt_SOURCE_DIR}/include>
    # $<INSTALL_INTERFACE:${fmt_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)
target_link_libraries(FMT_LIBRARY INTERFACE
    $<IF:$<CONFIG:Debug>, ${fmt_BINARY_DIR}/libfmtd.a, ${fmt_BINARY_DIR}/libfmt.a>
)

# ===================================================================
FetchContent_Declare(glm_fetch
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 8f6213d379a904f5ae910e09a114e066e25faf57
)
FetchContent_MakeAvailable(glm_fetch)
FetchContent_GetProperties(glm_fetch
    SOURCE_DIR glm_SOURCE_DIR
    BINARY_DIR glm_BINARY_DIR
)
add_library(GLM_LIBRARY INTERFACE IMPORTED GLOBAL)
target_include_directories(GLM_LIBRARY INTERFACE
    $<BUILD_INTERFACE:${glm_SOURCE_DIR}>
    # $<INSTALL_INTERFACE:${glm_SOURCE_DIR}>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>>
)
target_link_libraries(GLM_LIBRARY INTERFACE
    ${glm_BINARY_DIR}/glm/libglm.a
)