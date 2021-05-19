
#ifndef Application_h
#define Application_h

#include <string>
#include <memory.h>

#include "NonCopyable.h"

namespace rtvvulfw {

class Window;
class VulInstance;

class Application: private NonCopyable {
public:
    virtual std::shared_ptr<Window> createWindow(const std::string &title);

    virtual void run();

    std::shared_ptr<Window> window() {
        return mWindow;
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

