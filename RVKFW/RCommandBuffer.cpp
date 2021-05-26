
#include "RCommandBuffer.h"
#include "RLogicalDevice.h"
#include "RSwapChain.h"
#include "RRenderPass.h"
#include "RCommandPool.h"
#include "RFramebuffer.h"

namespace rvkfw {

RCommandBuffer::RCommandBuffer(std::shared_ptr<RRenderPass> renderPass,
               std::shared_ptr<RSwapChain> swapChain,
               std::shared_ptr<RLogicalDevice> logicalDevice,
               std::shared_ptr<RFramebuffer> framebuffer,
               std::shared_ptr<RGraphicsPipeline> graphicsPipeline,
               std::shared_ptr<RCommandPool> commandPool) :
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
    auto renderPass = mRenderPass.lock();
    auto swapChain = mSwapChain.lock();
    auto commandPool = mCommandPool.lock();
    auto frameBuffer = mFramebuffer.lock();
    auto graphicsPipeline = mGraphicsPipeline.lock();

    if (!logicalDevice || !renderPass || !swapChain || !commandPool || !frameBuffer) {
        LOG_ERROR(tag(), "Creation failed logicalDevice:" << logicalDevice.get() <<
                  " renderPass:" << renderPass.get() <<
                  " swapChain:" << swapChain.get())
        throw std::runtime_error("RCommandBuffer creation failed, required objects are not available!");
    }

    mCommandBuffers.resize(swapChain->images().size());

    mAllocInfo.commandPool = commandPool->handle();
    mAllocInfo.commandBufferCount = (uint32_t) mCommandBuffers.size();

    if (vkAllocateCommandBuffers(logicalDevice->handle(), &mAllocInfo, mCommandBuffers.data()) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to allocate command buffers!");
        throw std::runtime_error("failed to allocate command buffers!");
    }

}

}
