
#ifndef Application_h
#define Application_h

#include <string>
#include <memory.h>

#include "RNonCopyable.h"

namespace rtvvulfw {

class RWindow;
class RInstance;
class RSurface;

class RApplication: private RNonCopyable {
public:
    virtual RWindow* createWindow(const std::string &title);

    virtual void run();

    RWindow* window() {
        return mWindow.get();
    }

    const char * tag() const {
        return "RApplication";
    }

protected:
    RApplication() = default;
    ~RApplication() {
        cleanup();
    }

    virtual void init();
    virtual void cleanup();
    virtual void mainLoop();

    std::shared_ptr<RWindow> mWindow;
    std::shared_ptr<RInstance> mVulInstance;
};

}

#endif

