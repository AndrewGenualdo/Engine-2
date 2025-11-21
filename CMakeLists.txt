cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_POLICY_VERSION_MINIMUM  3.5)

# Include guards
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

project(Engine-2)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/libs)

include(external/cpm.cmake)

# add libraries
include(external/glfw.cmake)
include(external/imgui.cmake)
include(external/glm.cmake)
include(external/glew.cmake)
include(external/sdl.cmake)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Sanitizers enabled for Debug build")

  # For memory issues
  add_compile_options(-fsanitize=address,leak -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,leak)
endif()

add_subdirectory(core)
add_subdirectory(.project)

