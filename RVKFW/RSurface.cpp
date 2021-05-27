
#include "RSurface.h"
#include "RVkInstance.h"
#include "RWindow.h"
#include "RLogger.h"
#include <GLFW/glfw3.h>

namespace rvkfw {

void RSurface::create(std::shared_ptr<RVkInstance> instance, std::shared_ptr<RWindow> window) {
    if (mCreated.exchange(true)) {
        return;
    }

    mInstance = instance;

    if (glfwCreateWindowSurface(instance->handle(), window->handle(), nullptr, &mSurface) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Creating surface failed");
        throw std::runtime_error("Creating surface failed");
    }
}

RSurface::~RSurface() {
    if (auto instance = mInstance.lock()) {
        if (mSurface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(instance->handle(), mSurface, nullptr);
        }
    } else {
        LOG_ERROR(tag(), "Could not get vkInstance, already destroyed!");
   }
}

}
