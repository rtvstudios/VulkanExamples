
#ifndef RApplication_h
#define RApplication_h

#include <string>
#include <memory.h>

#include "RObject.h"

namespace rvkfw {

class RWindow;
class RInstance;
class RSurface;

class RApplication: public RObject {
public:
    virtual void create(const std::string &title);

    virtual void run();

    std::shared_ptr<RWindow> window() {
        return mWindow;
    }

    const char * tag() const override {
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
    std::shared_ptr<RInstance> mVkInstance;
};

}

#endif

