
#ifndef Window_h
#define Window_h

#include "NonCopyable.h"

#include <string>
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class Window: private NonCopyable {
public:
    Window(const std::string &title, int width=800, int height=600);
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

protected:
    std::string mTitle{ "Vulkan Window" };
    int mWidth{ 800 };
    int mHeight{ 600 };
    GLFWwindow* mWindow{ nullptr };
};

}

#endif

