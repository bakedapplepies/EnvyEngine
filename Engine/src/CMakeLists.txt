set(L${PROJECT_NAME}PublicHeaders
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/ShaderProgram.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/Texture2D.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/VertexArray.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/Pipeline.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/UniformBuffer.h
    ${PROJECT_SOURCE_DIR}/include/Envy/opengl/Cubemap.h
    ${PROJECT_SOURCE_DIR}/include/Envy/EnvyInstance.h
)

set(L${PROJECT_NAME}Source
    core/Debug.cpp
    opengl/ShaderProgram.cpp
    opengl/Texture2D.cpp
    opengl/VertexArray.cpp
    opengl/Pipeline.cpp
    opengl/UniformBuffer.cpp
    opengl/Cubemap.cpp
    opengl/ResourceManager.cpp
    EnvyInstance.cpp
)

add_library(L${PROJECT_NAME}
    ${L${PROJECT_NAME}Source}
)

add_library(L${PROJECT_NAME}Deps INTERFACE)

add_dependencies(L${PROJECT_NAME}Deps INTERFACE
    FMT_EXTERN
    STB_IMAGE_WRITE
    STB_IMAGE_LOAD
)

set(ENVY_PUBLIC_INCLUDES
    ${PROJECT_SOURCE_DIR}/include
    ${ENVY_VENDOR_INCLUDES}
CACHE PATH "Includes for EnvyEngine")

target_include_directories(L${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${ENVY_VENDOR_INCLUDES}
)

target_link_directories(L${PROJECT_NAME}Deps INTERFACE
    $<IF:$<PLATFORM_ID:Windows>, ${FMT_INSTALL_DIR}/install/lib,
                                 ${FMT_INSTALL_DIR}/install/lib64>
    $<IF:$<PLATFORM_ID:Windows>, ${GLM_INSTALL_DIR}/install/lib,
                                 ${GLM_INSTALL_DIR}/install/lib64>
)

target_link_libraries(L${PROJECT_NAME}Deps INTERFACE
    $<IF:$<CONFIG:Debug>, fmtd, fmt>
    STB_IMAGE_WRITE
    STB_IMAGE_LOAD
    glm
    $<IF:$<PLATFORM_ID:Windows>, opengl32 gdi32 user32 kernel32 shell32,
                                 GL>
)

target_link_libraries(L${PROJECT_NAME} PRIVATE L${PROJECT_NAME}Deps)
target_compile_definitions(L${PROJECT_NAME} PRIVATE
    GLM_ENABLE_EXPERIMENTAL
    ENVY_ROOT_DIR="${PROJECT_SOURCE_DIR}"
)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(L${PROJECT_NAME} PRIVATE
        ENVY_BUILD_STATIC
    )
else()
    target_compile_definitions(L${PROJECT_NAME}
        PRIVATE $<BUILD_INTERFACE:ENVY_BUILD_SHARED>
        INTERFACE $<INSTALL_INTERFACE:ENVY_BUILD_USE_SHARED>
    )
endif()

set_target_properties(L${PROJECT_NAME} PROPERTIES
    VERIFY_INTERFACE_HEADER_SETS true
    PUBLIC_HEADER "${L${PROJECT_NAME}PublicHeaders}"
)

if (BUILD_SHARED_LIBS)
    # Hide Envy's symbols by default so ENVY_API can expose them
    set_target_properties(L${PROJECT_NAME} PROPERTIES
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
    )
endif()

# Build-type configurations
target_compile_options(L${PROJECT_NAME} PRIVATE
    $<IF:$<CONFIG:Debug>,
        -O0 -g -fsanitize=address,
        -O3 -DNDEBUG -DENVY_RELEASE
    >
)

target_link_options(L${PROJECT_NAME} PRIVATE
    $<IF:$<CONFIG:Debug>,
        -O0 -g -fsanitize=address,
        -O3
    >
)

# if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#     target_compile_options(L${PROJECT_NAME} PRIVATE -O0 -g -fsanitize=address)
#     target_link_options(L${PROJECT_NAME} PRIVATE -g -fsanitize=address)

# elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
#     target_compile_options(L${PROJECT_NAME} PRIVATE -O3)
#     target_link_options(L${PROJECT_NAME} PRIVATE -O3)
#     target_compile_definitions(L${PROJECT_NAME} PRIVATE ENVY_RELEASE)

# elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
#     message("MinSizeRel config not supported yet!")
    
# elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
#     message("RelWithDebInfo config not supported yet!")

# endif()