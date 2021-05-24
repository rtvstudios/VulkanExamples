
#include "RRenderPass.h"
#include "RSwapChain.h"
#include "RLogger.h"
#include "RLogicalDevice.h"

namespace rvkfw {

RRenderPass::~RRenderPass() {
    if (auto device = mLogicalDevice.lock()) {
        vkDestroyRenderPass(device->handle(), mRenderPass, nullptr);
    }
}

void RRenderPass::create(std::shared_ptr<RLogicalDevice> logicalDevice, std::shared_ptr<RSwapChain> swapChain) {
    if (mCreated.exchange(true)) {
        return;
    }

    mLogicalDevice = logicalDevice;

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChain->surfaceFormat().format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(logicalDevice->handle(), &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create render pass!");
        throw std::runtime_error("failed to create render pass!");
    }

    LOG_DEBUG(tag(), "Render pass object is created");
}

}
