
#include "RApplication.h"
#include "RWindow.h"
#include "RInstance.h"
#include "RLogger.h"

namespace rvkfw {

RWindow* RApplication::createWindow(const std::string &title) {
    mWindow = std::make_shared<RWindow>(title);
    return mWindow.get();
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

    mVulInstance = std::make_shared<RInstance>(mWindow->title(), mWindow.get());
    if (!mVulInstance->isCreated()) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    LOG_DEBUG(tag(), "RApplication intialization finished");
}

void RApplication::cleanup() {
    LOG_DEBUG(tag(), "RApplication cleaning up");
    mVulInstance = nullptr;
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
