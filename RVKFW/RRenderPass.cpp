
#include "RRenderPass.h"
#include "RSwapChain.h"
#include "RLogger.h"
#include "RLogicalDevice.h"

namespace rvkfw {

RRenderPass::RRenderPass(std::shared_ptr<RLogicalDevice> logicalDevice,
                         std::shared_ptr<RSwapChain> swapChain) : mLogicalDevice{logicalDevice},
                        mSwapChain{swapChain} {

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChain->surfaceFormat().format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    mColorAttachments.push_back(colorAttachment);

    mColorAttachmentRef.attachment = 0;
    mColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &mColorAttachmentRef;
    mSubpasses.push_back(subpass);

    mRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
}

RRenderPass::~RRenderPass() {
    if (auto device = mLogicalDevice.lock()) {
        vkDestroyRenderPass(device->handle(), mRenderPass, nullptr);
    } else {
        LOG_ERROR(tag(), "Could not get logical device, already destroyed!");
    }
}

void RRenderPass::create() {
    if (mCreated.exchange(true)) {
        return;
    }

    auto logicalDevice = mLogicalDevice.lock();
    auto swapChain = mSwapChain.lock();
    if (!logicalDevice || !swapChain) {
        LOG_ERROR(tag(), "Creation failed logicalDevice:" << logicalDevice.get() <<
                  " swapChain:" << swapChain.get())
        throw std::runtime_error("RRenderPass creation failed, required objects are not available!");
    }

    mRenderPassInfo.attachmentCount = static_cast<decltype(mRenderPassInfo.attachmentCount)>(mColorAttachments.size());
    mRenderPassInfo.pAttachments = mColorAttachments.data();
    mRenderPassInfo.subpassCount = static_cast<decltype(mRenderPassInfo.subpassCount)>(mSubpasses.size());
    mRenderPassInfo.pSubpasses = mSubpasses.data();

    if (vkCreateRenderPass(logicalDevice->handle(), &mRenderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create render pass!");
        throw std::runtime_error("failed to create render pass!");
    }

    LOG_DEBUG(tag(), "Render pass object is created");
}

}
