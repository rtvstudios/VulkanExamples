
#ifndef RApplication_h
#define RApplication_h

#include <string>
#include <memory.h>

#include "RObject.h"

namespace rvkfw {

class RWindow;
class RVkInstance;
class RSurface;
class RCommandPool;
class RSwapChain;
class RPhysicalDevice;
class RQueue;
class RLogicalDevice;
class RFramebuffer;

class RApplication: public RObject {
public:
    virtual bool create(const std::string &title);
    void run();

    const char * tag() const override {
        return "RApplication";
    }

    std::shared_ptr<RWindow> window() {
        return mWindow;
    }

    std::shared_ptr<RVkInstance> vkInstance() {
        return mVkInstance;
    }

    std::shared_ptr<RPhysicalDevice> physicalDevice() const;
    std::shared_ptr<RLogicalDevice> logicalDevice() const;
    std::shared_ptr<RSwapChain> swapChain() const;
    std::shared_ptr<RCommandPool> commandPool() const;

protected:
    RApplication();
    ~RApplication();

    virtual void draw() const;
    void destroy() override;

    std::shared_ptr<RWindow> mWindow;
    std::shared_ptr<RVkInstance> mVkInstance;

private:
    void preCreate() override;
    void mainLoop();
};

}

#endif

