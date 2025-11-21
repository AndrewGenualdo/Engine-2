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

message(STATUS ">>> EMSCRIPTEN: ${EMSCRIPTEN}")
# Detect if building with Emscripten
if (CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  message(STATUS "Building for WebAssembly")

  set(USE_WEBGL TRUE)

  # Disable platform-specific warnings and definitions
  add_definitions(-D__EMSCRIPTEN__)

  # Emscripten link settings
  set(EM_FLAGS
          "-sUSE_GLFW=3"
          "-sWASM=1"
          "-sFULL_ES3=1"
          "-sALLOW_MEMORY_GROWTH=1"
          "-sDISABLE_EXCEPTION_CATCHING=0"
          "--preload-file assets@/assets"
  )

  # Apply to all targets later
  string (JOIN " " EM_FLAGS_STR ${EM_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EM_FLAGS_STR}")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/libs)

include(external/cpm.cmake)

if (EMSCRIPTEN)
  set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
endif()

# add libraries
include(external/glfw.cmake)
include(external/imgui.cmake)
include(external/glm.cmake)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Sanitizers enabled for Debug build")

  # For memory issues
  add_compile_options(-fsanitize=address,leak -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,leak)
endif()

add_subdirectory(core)
add_subdirectory(.project)

