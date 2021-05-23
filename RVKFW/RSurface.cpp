
#include "VulSurface.h"
#include "VulInstance.h"
#include "Window.h"
#include "Logger.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

VulSurface::VulSurface(VulInstance *vulInstance, Window *window): mVulInstance{ vulInstance } {
    if (glfwCreateWindowSurface(vulInstance->handle(), window->handle(), nullptr, &mSurface) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Creating surface failed");
        throw std::runtime_error("Creating surface failed");
    }
}

VulSurface::~VulSurface() {
    if (mSurface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(mVulInstance->handle(), mSurface, nullptr);
    }
}

}
