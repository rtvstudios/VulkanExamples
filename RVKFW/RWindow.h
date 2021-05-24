
#ifndef Window_h
#define Window_h

#include "RNonCopyable.h"

#include <string>
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class RWindow: private RNonCopyable {
public:
    RWindow(const std::string &title, uint32_t width=800, uint32_t height=600);
    ~RWindow();

    bool shouldClose() {
        return glfwWindowShouldClose(mWindow);
    }

    const std::string &title() const {
        return mTitle;
    }

    GLFWwindow *handle() {
        return mWindow;
    }

    uint32_t width() const {
        return mWidth;
    }

    uint32_t height() const {
        return mHeight;
    }

protected:
    std::string mTitle{ "Vulkan RWindow" };
    uint32_t mWidth{ 800 };
    uint32_t mHeight{ 600 };
    GLFWwindow* mWindow{ nullptr };
};

}

#endif

