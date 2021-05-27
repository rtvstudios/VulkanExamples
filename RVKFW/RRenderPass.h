

#ifndef RRenderPass_h
#define RRenderPass_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RSwapChain;

class RRenderPass: public RObject {
public:
    RRenderPass(std::weak_ptr<RLogicalDevice> logicalDevice,
                std::weak_ptr<RSwapChain> swapChain);
    ~RRenderPass();

    void create();

    const char * tag() const override {
        return "RRenderPass";
    }

    VkRenderPass handle() const {
        return mRenderPass;
    }

    std::vector<VkAttachmentDescription> &colorAttachments() {
        return mColorAttachments;
    }

    VkAttachmentReference &colorAttachmentRef() {
        return mColorAttachmentRef;
    }

    std::vector<VkSubpassDescription> &subpass() {
        return mSubpasses;
    }

    VkRenderPassCreateInfo &renderPassInfo() {
        return mRenderPassInfo;
    }

    VkSubpassDependency &dependency() {
        return mDependency;
    }

protected:
    VkRenderPass mRenderPass{ VK_NULL_HANDLE };

    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RSwapChain> mSwapChain;

    std::vector<VkAttachmentDescription> mColorAttachments;
    std::vector<VkSubpassDescription> mSubpasses;

    VkAttachmentReference mColorAttachmentRef{};
    VkRenderPassCreateInfo mRenderPassInfo{};
    VkSubpassDependency mDependency{};

};

}

#endif
