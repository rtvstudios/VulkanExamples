

#ifndef RRenderPass_h
#define RRenderPass_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RSwapChain;

class RRenderPass: public RObject {
public:
    RRenderPass() = default;
    ~RRenderPass();

    void create(std::shared_ptr<RLogicalDevice> logicalDevice,
                std::shared_ptr<RSwapChain> swapChain);

    const char * tag() const override {
        return "RRenderPass";
    }

    VkRenderPass handle() {
        return mRenderPass;
    }

protected:
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    VkRenderPass mRenderPass{ VK_NULL_HANDLE };
};

}

#endif
