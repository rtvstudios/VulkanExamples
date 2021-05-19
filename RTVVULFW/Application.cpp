
#include "Application.h"
#include "Window.h"
#include "VulInstance.h"

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
    mVulInstance = std::make_shared<VulInstance>(mWindow->title());
    if (!mVulInstance->isCreated()) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}

void Application::cleanup() {
    mVulInstance = nullptr;
    mWindow = nullptr;
}

void Application::mainLoop() {
    while(!mWindow->shouldClose()) {
        glfwPollEvents();
    }
}

}
