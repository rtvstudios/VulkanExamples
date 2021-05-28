cmake_minimum_required(VERSION 3.10)

project(Rectangle)

file (GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
file (GLOB HEADERS CONFIGURE_DEPENDS "*.h")

set(SHADERS Shader.vert Shader.frag)
set(COMPILED_SHADERS Shader.vert.spv Shader.frag.spv)

add_executable(Rectangle ${SHADERS} ${COMPILED_SHADERS} ${HEADERS} ${SOURCES})

target_include_directories(Rectangle PUBLIC ${CUSTOM_INCLUDES} "../RVKFW")

target_link_libraries(Rectangle ${CUSTOM_LIBRARIES} "RVKFW")

add_compile_definitions(SHADER_DIRECTORY="${CMAKE_CURRENT_BINARY_DIR}")

foreach(SHADER ${SHADERS})
add_custom_command(OUTPUT ${SHADER}.spv
COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER} -g -o ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv
DEPENDS ${SHADER}
COMMENT "Rebuilding  ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv" )
message(STATUS "Generating build commands for  ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv")
endforeach()

add_dependencies(Rectangle RVKFW)
