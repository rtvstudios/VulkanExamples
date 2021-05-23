
#include "Application.h"
#include "Window.h"
#include "VulInstance.h"
#include "Logger.h"

namespace rtvvulfw {

Window* Application::createWindow(const std::string &title) {
    mWindow = std::make_shared<Window>(title);
    return mWindow.get();
}

void Application::run() {
    if (!mWindow) {
        throw std::logic_error("Create Window First");
    }

    init();
    mainLoop();
    cleanup();
}

void Application::init() {
    LOG_DEBUG(tag(), "Application intialization started");

    mVulInstance = std::make_shared<VulInstance>(mWindow->title(), mWindow.get());
    if (!mVulInstance->isCreated()) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    LOG_DEBUG(tag(), "Application intialization finished");
}

void Application::cleanup() {
    LOG_DEBUG(tag(), "Application cleaning up");
    mVulInstance = nullptr;
    mWindow = nullptr;
}

void Application::mainLoop() {
    LOG_DEBUG(tag(), "Application mainLoop started");

    while(!mWindow->shouldClose()) {
        glfwPollEvents();
    }
    LOG_DEBUG(tag(), "Application mainLoop finished");
}

}
