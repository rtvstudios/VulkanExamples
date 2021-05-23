cmake_minimum_required(VERSION 3.10)

project(RVKFW VERSION 1.0.1 DESCRIPTION "Framework for creating Vulkan applications")

file (GLOB RVKFW_SOURCES CONFIGURE_DEPENDS "*.cpp")
file (GLOB RVKFW_HEADERS CONFIGURE_DEPENDS "*.h")

add_library(RVKFW SHARED ${RVKFW_HEADERS} ${RVKFW_SOURCES})

target_include_directories(RVKFW PUBLIC ${CUSTOM_INCLUDES})
target_link_libraries(RVKFW ${CUSTOM_LIBRARIES})

message(STATUS "CUSTOM_INCLUDES: ${CUSTOM_INCLUDES}")
message(STATUS "CUSTOM_LIBRARIES: ${CUSTOM_LIBRARIES}")

