cmake_minimum_required(VERSION 3.26.0)
project(EnvyClient CXX C)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Make sure binary directory is not the same as source directory
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# Detect switch in build type
if(EXISTS "${PROJECT_SOURCE_DIR}/.vscode/settings.json")
    message("Altering .vscode/settings.json")
    execute_process(
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/.vscode
        COMMAND_ECHO STDOUT
        COMMAND python compile_commands_change.py ${CMAKE_BUILD_TYPE}
    )
endif()

add_subdirectory(Engine)

if (${ENVY_BUILD_TEST})
    add_subdirectory(Client)
endif()
