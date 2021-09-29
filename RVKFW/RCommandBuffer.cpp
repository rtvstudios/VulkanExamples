
#include "RCommandBuffer.h"
#include "RLogicalDevice.h"
#include "RSwapChain.h"
#include "RRenderPass.h"
#include "RCommandPool.h"
#include "RFramebuffer.h"

namespace rvkfw {

RCommandBuffer::RCommandBuffer(std::weak_ptr<RRenderPass> renderPass,
               std::weak_ptr<RSwapChain> swapChain,
               std::weak_ptr<RLogicalDevice> logicalDevice,
               std::weak_ptr<RFramebuffer> framebuffer,
               std::weak_ptr<RGraphicsPipeline> graphicsPipeline,
               std::weak_ptr<RCommandPool> commandPool) :
    mRenderPass{renderPass},
    mSwapChain{swapChain},
    mLogicalDevice{logicalDevice},
    mFramebuffer{framebuffer},
    mGraphicsPipeline{graphicsPipeline},
    mCommandPool{commandPool} {

    mAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    mAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
}

size_t RCommandBuffer::imageCount() const {
    if (auto swapChain = mSwapChain.lock()) {
        return swapChain->images().size();
    }
    return  0;
}

void RCommandBuffer::create() {
    if (mCreated.exchange(true)) {
        return;
    }

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    auto renderPass = mRenderPass.lock();
    ASSERT_NOT_NULL(renderPass);

    auto swapChain = mSwapChain.lock();
    ASSERT_NOT_NULL(swapChain);

    auto commandPool = mCommandPool.lock();
    ASSERT_NOT_NULL(commandPool);

    auto frameBuffer = mFramebuffer.lock();
    ASSERT_NOT_NULL(frameBuffer);

    auto graphicsPipeline = mGraphicsPipeline.lock();
    ASSERT_NOT_NULL(graphicsPipeline);

    mCommandBuffers.resize(swapChain->images().size());

    mAllocInfo.commandPool = commandPool->handle();
    mAllocInfo.commandBufferCount = (uint32_t) mCommandBuffers.size();

    if (vkAllocateCommandBuffers(logicalDevice->handle(), &mAllocInfo, mCommandBuffers.data()) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to allocate command buffers!");
        throw std::runtime_error("failed to allocate command buffers!");
    }

}

void RCommandBuffer::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }
}


}
