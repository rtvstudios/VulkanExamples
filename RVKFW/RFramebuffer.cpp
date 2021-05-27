
#include "RFramebuffer.h"
#include "RRenderPass.h"
#include "RSwapChain.h"
#include "RLogicalDevice.h"

namespace rvkfw {

RFramebuffer::~RFramebuffer() {
    if (auto logicalDevice = mLogicalDevice.lock()) {
        for (auto framebuffer : mSwapChainFramebuffers) {
            vkDestroyFramebuffer(logicalDevice->handle(), framebuffer, nullptr);
        }
    } else {
        LOG_ERROR(tag(), "Could not get logical device, already destroyed!");
    }
}

void RFramebuffer::create() {
    if (mCreated.exchange(true)) {
        return;
    }

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    auto renderPass = mRenderPass.lock();
    ASSERT_NOT_NULL(renderPass);

    auto swapChain = mSwapChain.lock();
    ASSERT_NOT_NULL(swapChain);

    mSwapChainFramebuffers.resize(swapChain->imageViews().size());

    int i = 0;
    for (auto &imageView : swapChain->imageViews()) {
        VkImageView attachments[] = {
            imageView
        };
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->handle();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain->swapExtent().width;
        framebufferInfo.height = swapChain->swapExtent().height;
        framebufferInfo.layers = 1;
        if (vkCreateFramebuffer(logicalDevice->handle(), &framebufferInfo,
                                nullptr,
                                &mSwapChainFramebuffers[i++]) != VK_SUCCESS) {
            LOG_ERROR(tag(), "Failed to create framebuffer");
            throw std::runtime_error("failed to create framebuffer!");
        }
    }

    LOG_DEBUG(tag(), "Framebuffers created!");
}

}
