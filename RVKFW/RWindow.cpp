
#include "RWindow.h"

namespace rvkfw {

void RWindow::create(const std::string &title) {
    if (mCreated.exchange(true)) {
        return;
    }

    mTitle = title;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, mResizable ? GLFW_TRUE : GLFW_FALSE);

    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
}

void RWindow::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }
    
    if (mWindow != nullptr) {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
    }
    mWindow = nullptr;
}

void RWindow::setSize(int width, int height) {
    mWidth = width;
    mHeight = height;
    if (mWindow) {
        glfwSetWindowSize(mWindow, mWidth, mHeight);
    }
}

int RWindow::width() const {
    if (mWindow) {
        glfwGetWindowSize(mWindow, &mWidth, &mHeight);
    }
    return mWidth;
}

int RWindow::height() const {
    if (mWindow) {
        glfwGetWindowSize(mWindow, &mWidth, &mHeight);
    }
    return mHeight;
}

RWindow::~RWindow() {
    destroy();
}

}
