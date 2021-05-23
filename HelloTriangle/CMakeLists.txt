cmake_minimum_required(VERSION 3.10)

project(HelloTriangle)

file (GLOB HelloTriangle_SOURCES CONFIGURE_DEPENDS "*.cpp")
file (GLOB HelloTriangle_HEADERS CONFIGURE_DEPENDS "*.h")

set(SHADERS ColoredTriangle.vert ColoredTriangle.frag)
set(COMPILED_SHADERS ColoredTriangle.vert.spv ColoredTriangle.frag.spv)

add_executable(HelloTriangle ${SHADERS} ${COMPILED_SHADERS} ${HelloTriangle_SOURCES} ${HelloTriangle_HEADERS})

target_include_directories(HelloTriangle PUBLIC ${CUSTOM_INCLUDES} "../RVKFW")

target_link_libraries(HelloTriangle ${CUSTOM_LIBRARIES} "RVKFW")

foreach(SHADER ${SHADERS})
add_custom_command(OUTPUT ${SHADER}.spv
COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER} -g -o ${SHADER}.spv
DEPENDS ${SHADER}
COMMENT "Rebuilding ${SHADER}.spv" )
message(STATUS "Generating build commands for ${SHADER}.spv")
endforeach()


add_dependencies(HelloTriangle RVKFW)

