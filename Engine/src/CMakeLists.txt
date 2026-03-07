add_subdirectory(vendor)

set(${PROJECT_NAME}PublicHeaders
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/ShaderProgram.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/Texture2D.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/VertexArray.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/Pipeline.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/UniformBuffer.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/Cubemap.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/IndirectBuffer.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/Framebuffer.h
    ${PROJECT_SOURCE_DIR}/include/Envy/EnvyInstance.h
)

set(${PROJECT_NAME}Source
    core/Debug.cpp
    opengl/ShaderProgram.cpp
    opengl/Texture2D.cpp
    opengl/VertexArray.cpp
    opengl/Pipeline.cpp
    opengl/UniformBuffer.cpp
    opengl/Cubemap.cpp
    opengl/IndirectBuffer.cpp
    opengl/Framebuffer.cpp
    opengl/ResourceManager.cpp
    EnvyInstance.cpp
)

add_library(${PROJECT_NAME}
    ${${PROJECT_NAME}Source}
)

add_dependencies(${PROJECT_NAME}
    FMT_LIBRARY
    GLM_LIBRARY
    STB_IMAGE_WRITE
    STB_IMAGE_LOAD
    GLAD
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    STB_IMAGE_WRITE
    STB_IMAGE_LOAD
    GLM_LIBRARY
    FMT_LIBRARY
    GLAD
    $<IF:$<PLATFORM_ID:Windows>, opengl32 gdi32 user32 kernel32 shell32,  # Windows
                                 GL>                                      # Linux
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    ENVY_ROOT_DIR="${PROJECT_SOURCE_DIR}"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    GLM_ENABLE_EXPERIMENTAL
)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        ENVY_BUILD_STATIC
    )
else()
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE $<BUILD_INTERFACE:ENVY_BUILD_SHARED>
        INTERFACE $<INSTALL_INTERFACE:ENVY_BUILD_USE_SHARED>
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERIFY_INTERFACE_HEADER_SETS true
    PUBLIC_HEADER "${${PROJECT_NAME}PublicHeaders}"
)

if (BUILD_SHARED_LIBS)
    # Hide Envy's symbols by default so ENVY_API can expose them
    set_target_properties(${PROJECT_NAME} PROPERTIES
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
    )
endif()

# Build-type configurations
target_compile_options(${PROJECT_NAME} PRIVATE
    $<IF:$<CONFIG:Debug>,
        -O0 -g -fsanitize=address,
        -O3 -DNDEBUG -DENVY_RELEASE
    >
)

target_link_options(${PROJECT_NAME} PRIVATE
    $<IF:$<CONFIG:Debug>,
        -O0 -g -fsanitize=address,
        -O3
    >
)