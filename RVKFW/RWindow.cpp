
#include "Window.h"

namespace rtvvulfw {

Window::Window(const std::string &title, uint32_t width, uint32_t height) : mTitle{title},
        mWidth{ width },
        mHeight{ height } {
            
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
}

Window::~Window() {
    if (mWindow != nullptr) {
        glfwDestroyWindow(mWindow);
    }
    glfwTerminate();
}


}
