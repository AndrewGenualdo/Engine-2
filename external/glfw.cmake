# GLFW
string(TIMESTAMP BEFORE "%s")

if (NOT EMSCRIPTEN)
	CPMAddPackage(
			NAME glfw
			URL "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip"
			OPTIONS
			"GLFW_BUILD_EXAMPLES OFF"
			"GLFW_BUILD_TESTS OFF"
			"GLFW_BUILD_DOCS OFF"
			"GLFW_INSTALL OFF"
	)
endif()

set(glfw_INCLUDE_DIR ${glfw_SOURCE_DIR}/include)
include_directories(${glfw_INCLUDE_DIR})

string(TIMESTAMP AFTER "%s")
math(EXPR DELTAglfw "${AFTER}-${BEFORE}")
message(STATUS "GLFW TIME: ${DELTAglfw}s")