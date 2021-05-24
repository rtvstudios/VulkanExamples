
#ifndef RFramebuffer_h
#define RFramebuffer_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RShaderModule;
class RSwapChain;
class RRenderPass;

class RFramebuffer: public RObject {
public:
    RFramebuffer(std::shared_ptr<RRenderPass> renderPass,
                 std::shared_ptr<RSwapChain> swapChain,
                 std::shared_ptr<RLogicalDevice> logicalDevice) :
        mRenderPass{renderPass},
        mSwapChain{swapChain},
        mLogicalDevice{logicalDevice} {
    }
    ~RFramebuffer();

    void create();

    const char * tag() const override {
        return "RFramebuffer";
    }

protected:
    std::weak_ptr<RRenderPass> mRenderPass;
    std::weak_ptr<RSwapChain> mSwapChain;
    std::weak_ptr<RLogicalDevice> mLogicalDevice;

    std::vector<VkFramebuffer> mSwapChainFramebuffers;
};

}

#endif
