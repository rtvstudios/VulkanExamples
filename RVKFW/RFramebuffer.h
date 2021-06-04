
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
    void destroy() override;
    
    const char * tag() const override {
        return "RFramebuffer";
    }

    const std::vector<VkFramebuffer> &frameBufferHandles() {
        return mSwapChainFramebuffers;
    }

protected:
    std::weak_ptr<RRenderPass> mRenderPass;
    std::weak_ptr<RSwapChain> mSwapChain;
    std::weak_ptr<RLogicalDevice> mLogicalDevice;

    std::vector<VkFramebuffer> mSwapChainFramebuffers;

public:
    class Creator {
    public:
        Creator(std::shared_ptr<RRenderPass> renderPass,
                     std::shared_ptr<RSwapChain> swapChain,
                     std::shared_ptr<RLogicalDevice> logicalDevice) :
            mObject{std::make_shared<RFramebuffer>(renderPass, swapChain, logicalDevice)} {
                mObject->preCreate();
        }

        std::shared_ptr<RFramebuffer> create() {
            mObject->create();
            return mObject;
        }

    private:
        std::shared_ptr<RFramebuffer> mObject;
    };
};

}

#endif
