
#include "RApplication.h"
#include "RWindow.h"
#include "RVkInstance.h"
#include "RLogger.h"
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"
#include "RSwapChain.h"
#include "RRenderPass.h"

namespace rvkfw {

RApplication::RApplication() {
    preCreate();
}

RApplication::~RApplication() {
    mVkInstance = nullptr;
    mWindow = nullptr;
}

void RApplication::preCreate() {
    mWindow = std::make_shared<RWindow>();
    mWindow->preCreate();

    mVkInstance = std::make_shared<RVkInstance>(mWindow);
    mVkInstance->preCreate();
}

void RApplication::create(const std::string &title) {
    if (mCreated.exchange(true)) {
        return;
    }
    
    mWindow->create(title);
}

void RApplication::run() {
    if (!mWindow) {
        LOG_ERROR(tag(), "Window not created")
        throw std::logic_error("Window not created");
    }

    init();
    mainLoop();
    destroy();
}

void RApplication::init() {
    LOG_DEBUG(tag(), "RApplication intialization started");

    mVkInstance->create(mWindow->title());
    if (!mVkInstance->isCreated()) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    LOG_DEBUG(tag(), "RApplication intialization finished");
}

void RApplication::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }
    LOG_DEBUG(tag(), "RApplication destroying");

    mWindow->destroy();
    mVkInstance->destroy();
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

std::shared_ptr<RPhysicalDevice> RApplication::physicalDevice() const {
    return mVkInstance->physicalDevice();
}

std::shared_ptr<RLogicalDevice> RApplication::logicalDevice() const {
    return physicalDevice()->logicalDevice();
}

std::shared_ptr<RSwapChain> RApplication::swapChain() const {
    return physicalDevice()->swapChain();
}

std::shared_ptr<RCommandPool> RApplication::commandPool() const {
    return logicalDevice()->commandPool();
}

}
