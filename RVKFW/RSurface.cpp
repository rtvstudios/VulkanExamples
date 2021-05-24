
#include "RSurface.h"
#include "RInstance.h"
#include "RWindow.h"
#include "RLogger.h"
#include <GLFW/glfw3.h>

namespace rvkfw {

RSurface::RSurface(RInstance *vulInstance, RWindow *window): mVulInstance{ vulInstance } {
    if (glfwCreateWindowSurface(vulInstance->handle(), window->handle(), nullptr, &mSurface) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Creating surface failed");
        throw std::runtime_error("Creating surface failed");
    }
}

RSurface::~RSurface() {
    if (mSurface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(mVulInstance->handle(), mSurface, nullptr);
    }
}

}
