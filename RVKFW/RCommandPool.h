
#ifndef RCommandPool_h
#define RCommandPool_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RShaderModule;
class RSwapChain;
class RRenderPass;

class RCommandPool: public RObject {
public:
    RCommandPool(std::shared_ptr<RLogicalDevice> logicalDevice) :
        mLogicalDevice{logicalDevice} {
    }
    ~RCommandPool();

    void create();

    const char * tag() const override {
        return "RCommandPool";
    }

    VkCommandPool handle() const {
        return mCommandPool;
    }

protected:
    std::weak_ptr<RLogicalDevice> mLogicalDevice;

    VkCommandPool mCommandPool{ VK_NULL_HANDLE };
};

}

#endif
