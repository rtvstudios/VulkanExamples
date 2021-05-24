
#include "RSurface.h"
#include "RInstance.h"
#include "RWindow.h"
#include "RLogger.h"
#include "RCreator.h"
#include <GLFW/glfw3.h>

namespace rvkfw {

void RSurface::create(std::shared_ptr<RInstance> instance, std::shared_ptr<RWindow> window) {
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
    if (mSurface != VK_NULL_HANDLE) {
        if (auto instance = mInstance.lock()) {
            vkDestroySurfaceKHR(instance->handle(), mSurface, nullptr);
        }
    }
}

}
