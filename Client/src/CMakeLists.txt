add_executable(${PROJECT_NAME}
    main.cpp
    Renderer.cpp
    Application.cpp
    GLFWCallbacks.cpp
    Path.cpp
    Material.cpp
    Transform.cpp
    Camera.cpp
    meshes/Quad.cpp
)

add_dependencies(${PROJECT_NAME}
    LEnvyEngine
    GLFW_EXTERN
    GLAD
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${GLFW_INSTALL_DIR}/install/include
    ${ENVY_PUBLIC_INCLUDES}
)

target_link_directories(${PROJECT_NAME} PRIVATE
    $<IF:$<PLATFORM_ID:Windows>, ${GLFW_INSTALL_DIR}/install/lib,
                                 ${GLFW_INSTALL_DIR}/install/lib64>
    ${CMAKE_BINARY_DIR}/vendor
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    LEnvyEngine
    glfw3
    GLAD
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    CLIENT_ROOT_DIR="${PROJECT_SOURCE_DIR}"
)

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