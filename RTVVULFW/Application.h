
#ifndef Application_h
#define Application_h

#include <string>
#include <memory.h>

#include "NonCopyable.h"

namespace rtvvulfw {

class Window;
class VulInstance;
class VulSurface;

class Application: private NonCopyable {
public:
    virtual Window* createWindow(const std::string &title);

    virtual void run();

    Window* window() {
        return mWindow.get();
    }

protected:
    Application() = default;
    ~Application() {
        cleanup();
    }

    virtual void init();
    virtual void cleanup();
    virtual void mainLoop();

    std::shared_ptr<Window> mWindow;
    std::shared_ptr<VulInstance> mVulInstance;
};

}

#endif

