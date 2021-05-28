
#ifndef RWindow_h
#define RWindow_h

#include "RObject.h"

namespace rvkfw {

class RWindow: public RObject {
public:
    RWindow() = default;

    void create(const std::string &title);

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

    int width() const;

    int height() const;

    void setSize(int width, int height);

    const char * tag() const override {
        return "RWindow";
    }

    void setResizable(bool v) {
        mResizable = v;
    }

    void destroy() override;

protected:
    std::string mTitle{ "Vulkan RWindow" };
    mutable int mWidth{ 800 };
    mutable int mHeight{ 600 };
    GLFWwindow* mWindow{ nullptr };
    bool mResizable{ false };
};

}

#endif

