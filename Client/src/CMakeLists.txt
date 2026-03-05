add_subdirectory(vendor)

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
    GLFW_LIBRARY
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}/vendor
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    LEnvyEngine
    GLFW_LIBRARY
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