
#include "RApplication.h"
#include "RWindow.h"
#include "RInstance.h"
#include "RLogger.h"
#include "RCreator.h"

namespace rvkfw {

void RApplication::create(const std::string &title) {
    mWindow = RCreator::create<RWindow>(title);
}

void RApplication::run() {
    if (!mWindow) {
        throw std::logic_error("Create RWindow First");
    }

    init();
    mainLoop();
    cleanup();
}

void RApplication::init() {
    LOG_DEBUG(tag(), "RApplication intialization started");

    mVkInstance = RCreator::create<RInstance>(mWindow->title(), mWindow);
    if (!mVkInstance->isCreated()) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    LOG_DEBUG(tag(), "RApplication intialization finished");
}

void RApplication::cleanup() {
    LOG_DEBUG(tag(), "RApplication cleaning up");
    mVkInstance = nullptr;
    mWindow = nullptr;
}

void RApplication::mainLoop() {
    LOG_DEBUG(tag(), "RApplication mainLoop started");

    while(!mWindow->shouldClose()) {
        glfwPollEvents();
    }
    LOG_DEBUG(tag(), "RApplication mainLoop finished");
}

}
