cmake_minimum_required(VERSION 3.10)

project(Triangle)

file (GLOB Triangle_SOURCES CONFIGURE_DEPENDS "*.cpp")
file (GLOB Triangle_HEADERS CONFIGURE_DEPENDS "*.h")

set(SHADERS ColoredTriangle.vert ColoredTriangle.frag)
set(COMPILED_SHADERS ColoredTriangle.vert.spv ColoredTriangle.frag.spv)

add_executable(Triangle ${SHADERS} ${COMPILED_SHADERS} ${Triangle_SOURCES} ${Triangle_HEADERS})

target_include_directories(Triangle PUBLIC ${CUSTOM_INCLUDES} "../RVKFW")

target_link_libraries(Triangle ${CUSTOM_LIBRARIES} "RVKFW")

add_compile_definitions(SHADER_DIRECTORY="${CMAKE_CURRENT_BINARY_DIR}")

foreach(SHADER ${SHADERS})
add_custom_command(OUTPUT ${SHADER}.spv
COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER} -g -o ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv
DEPENDS ${SHADER}
COMMENT "Rebuilding ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv" )
message(STATUS "Generating build commands for ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv")
endforeach()


add_dependencies(Triangle RVKFW)

