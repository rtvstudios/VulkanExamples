
#include "RSurface.h"
#include "RVkInstance.h"
#include "RWindow.h"
#include "RLogger.h"
#include <GLFW/glfw3.h>

namespace rvkfw {

void RSurface::create() {
    if (mCreated.exchange(true)) {
        return;
    }

    auto instance = mInstance.lock();
    ASSERT_NOT_NULL(instance);

    auto window = mWindow.lock();
    ASSERT_NOT_NULL(window);

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
