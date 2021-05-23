cmake_minimum_required(VERSION 3.10)

project(VulkanExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CUSTOM_LIBRARIES "")
set(CUSTOM_INCLUDES "")

if (WIN32)
	message(FATAL_ERROR "Windows not supported yet")
elseif (APPLE)
	list(APPEND CUSTOM_INCLUDES "/usr/local/include")

	set(GLFW_LIBRARY_PATH "/usr/local/lib")
	set(VULKAN_LIBRARY_PATH "/usr/local/lib")

	list(APPEND CUSTOM_LIBRARIES "${GLFW_LIBRARY_PATH}/libglfw.3.4.dylib")
	list(APPEND CUSTOM_LIBRARIES "${VULKAN_LIBRARY_PATH}/libvulkan.1.2.176.dylib")
	list(APPEND CUSTOM_LIBRARIES "${VULKAN_LIBRARY_PATH}/libvulkan.1.dylib")
elseif (UNIX)
	message(FATAL_ERROR "Unix not supported yet")
endif()

add_compile_definitions(GLFW_INCLUDE_VULKAN)
add_compile_definitions(GLM_FORCE_RADIANS)
add_compile_definitions(GLM_FORCE_DEPTH_ZERO_TO)
add_compile_definitions(WITH_VALIDATION_LAYER)
add_compile_definitions(DEBUG)
add_compile_definitions(VK_VERSION_1_0)

add_subdirectory(RVKFW)
add_subdirectory(HelloTriangle)


