
#include "RApplication.h"
#include "RWindow.h"
#include "RVkInstance.h"
#include "RLogger.h"
#include "RCreator.h"
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"
#include "RSwapChain.h"
#include "RRenderPass.h"

namespace rvkfw {

RApplication::~RApplication() {
    mVkInstance = nullptr;
    mWindow = nullptr;
}

void RApplication::create(const std::string &title) {
    if (mCreated.exchange(true)) {
        return;
    }
    
    mWindow = RCreator<RWindow>().create(title);
}

void RApplication::run() {
    if (!mWindow) {
        LOG_ERROR(tag(), "Window not created")
        throw std::logic_error("Window not created");
    }

    init();
    mainLoop();
    cleanup();
}

void RApplication::init() {
    LOG_DEBUG(tag(), "RApplication intialization started");

    mVkInstance = RCreator<RVkInstance>().create(mWindow->title(), mWindow);
    if (!mVkInstance->isCreated()) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    LOG_DEBUG(tag(), "RApplication intialization finished");
}

void RApplication::cleanup() {
    LOG_DEBUG(tag(), "RApplication cleaning up");
}

void RApplication::draw() const {
}

void RApplication::mainLoop() {
    LOG_DEBUG(tag(), "RApplication mainLoop started");

    while(!mWindow->shouldClose()) {
        glfwPollEvents();
        draw();
    }
    LOG_DEBUG(tag(), "RApplication mainLoop finished");
}

std::shared_ptr<RPhysicalDevice> RApplication::physicalDevice() {
    return mVkInstance->physicalDevice();
}

std::shared_ptr<RLogicalDevice> RApplication::logicalDevice() {
    return physicalDevice()->logicalDevice();
}

std::shared_ptr<RSwapChain> RApplication::swapChain() {
    return physicalDevice()->swapChain();
}

std::shared_ptr<RCommandPool> RApplication::commandPool() {
    return logicalDevice()->commandPool();
}

}
