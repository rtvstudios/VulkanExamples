
#ifndef RCommandBuffer_h
#define RCommandBuffer_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RShaderModule;
class RSwapChain;
class RRenderPass;
class RCommandPool;
class RFramebuffer;
class RGraphicsPipeline;

class RCommandBuffer: public RObject {
public:

    RCommandBuffer(std::weak_ptr<RRenderPass> renderPass,
                   std::weak_ptr<RSwapChain> swapChain,
                   std::weak_ptr<RLogicalDevice> logicalDevice,
                   std::weak_ptr<RFramebuffer> framebuffer,
                   std::weak_ptr<RGraphicsPipeline> graphicsPipeline,
                   std::weak_ptr<RCommandPool> commandPool);

    ~RCommandBuffer() = default;

    void create();
    void destroy() override;

    const char * tag() const override {
        return "RCommandBuffer";
    }

    const std::vector<VkCommandBuffer> &commandBuffers() const {
        return mCommandBuffers;
    }

    size_t imageCount() const;

    VkCommandBufferAllocateInfo &allocInfo() {
        return mAllocInfo;
    }

protected:
    std::weak_ptr<RRenderPass> mRenderPass;
    std::weak_ptr<RSwapChain> mSwapChain;
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RFramebuffer> mFramebuffer;
    std::weak_ptr<RCommandPool> mCommandPool;
    std::weak_ptr<RGraphicsPipeline> mGraphicsPipeline;

    VkCommandBufferAllocateInfo mAllocInfo{};

    std::vector<VkCommandBuffer> mCommandBuffers;

public:
    class Creator {
    public:
        Creator(std::weak_ptr<RRenderPass> renderPass,
                std::weak_ptr<RSwapChain> swapChain,
                std::weak_ptr<RLogicalDevice> logicalDevice,
                std::weak_ptr<RFramebuffer> framebuffer,
                std::weak_ptr<RGraphicsPipeline> graphicsPipeline,
                std::weak_ptr<RCommandPool> commandPool) :
            mObject{std::make_shared<RCommandBuffer>(renderPass, swapChain, logicalDevice,
                                                     framebuffer, graphicsPipeline, commandPool)} {
                mObject->preCreate();
        }

        VkCommandBufferAllocateInfo &allocInfo() {
            return mObject->allocInfo();
        }

        std::shared_ptr<RCommandBuffer> create() {
            mObject->create();
            return mObject;
        }

    private:
        std::shared_ptr<RCommandBuffer> mObject;

    };
        
};

}

#endif
