
#include "RWindow.h"

namespace rvkfw {

void RWindow::create(const std::string &title, uint32_t width, uint32_t height) {
    if (mCreated.exchange(true)) {
        return;
    }

    mWidth = width;
    mHeight = height;
    mTitle = title;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
}

RWindow::~RWindow() {
    if (mWindow != nullptr) {
        glfwDestroyWindow(mWindow);
    }
    glfwTerminate();
}


}
