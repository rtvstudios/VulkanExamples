
project(GLFWWindow)

file (GLOB GLFWWindow_SOURCES CONFIGURE_DEPENDS "*.cpp")
file (GLOB GLFWWindow_HEADERS CONFIGURE_DEPENDS "*.h")

add_executable(GLFWWindow ${GLFWWindow_SOURCES} ${GLFWWindow_HEADERS})

target_include_directories(GLFWWindow PUBLIC ${CUSTOM_INCLUDES})

target_link_libraries(GLFWWindow ${CUSTOM_LIBRARIES})

