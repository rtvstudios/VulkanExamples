
#ifndef Window_h
#define Window_h

#include "NonCopyable.h"

#include <string>
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class Window: private NonCopyable {
public:
    Window(const std::string &title, uint32_t width=800, uint32_t height=600);
    ~Window();

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
    std::string mTitle{ "Vulkan Window" };
    uint32_t mWidth{ 800 };
    uint32_t mHeight{ 600 };
    GLFWwindow* mWindow{ nullptr };
};

}

#endif

