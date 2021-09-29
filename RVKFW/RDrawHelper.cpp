
#include "RDrawHelper.h"
#include "RLogicalDevice.h"
#include "RSwapChain.h"
#include "RCommandBuffer.h"
#include "RCommandPool.h"
#include "RQueue.h"
#include "RBufferObject.h"
#include "RImageBuffer.h"

namespace rvkfw {

RDrawHelper::~RDrawHelper() {
    destroy();
}

void RDrawHelper::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }

    if (auto logicalDevice = mLogicalDevice.lock()) {
        for (auto semaphore: mRenderFinishedSemaphores) {
            vkDestroySemaphore(logicalDevice->handle(), semaphore, nullptr);
        }

        for (auto semaphore: mImageAvailableSemaphores) {
            vkDestroySemaphore(logicalDevice->handle(), semaphore, nullptr);
        }

        for (auto fence: mInFlightFences) {
            vkDestroyFence(logicalDevice->handle(), fence, nullptr);
        }
    }

    mRenderFinishedSemaphores.clear();
    mImageAvailableSemaphores.clear();
    mInFlightFences.clear();
    mImagesInFlight.clear();
}

void RDrawHelper::create(int framesInFlight) {
    if (mCreated.exchange(true)) {
        return;
    }

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    auto swapChain = mSwapChain.lock();
    ASSERT_NOT_NULL(swapChain);

    mMaxFramesInFlight = framesInFlight;

    mImageAvailableSemaphores.resize(mMaxFramesInFlight);
    mRenderFinishedSemaphores.resize(mMaxFramesInFlight);
    mInFlightFences.resize(mMaxFramesInFlight);

    mImagesInFlight.resize(swapChain->images().size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < mMaxFramesInFlight; i++) {
        if (vkCreateSemaphore(logicalDevice->handle(), &semaphoreInfo, nullptr, &mImageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice->handle(), &semaphoreInfo, nullptr, &mRenderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(logicalDevice->handle(), &fenceInfo, nullptr, &mInFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void RDrawHelper::draw(const std::shared_ptr<RCommandBuffer> &commandBuffer,
                       std::function<void(uint32_t)> onDrawStart) const {
    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    auto swapChain = mSwapChain.lock();
    ASSERT_NOT_NULL(swapChain);

    vkWaitForFences(logicalDevice->handle(), 1, &mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(logicalDevice->handle(), swapChain->handle(),
                          UINT64_MAX, mImageAvailableSemaphores[mCurrentFrame],
                          VK_NULL_HANDLE, &imageIndex);

    if (mImagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(logicalDevice->handle(), 1, &mImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    mImagesInFlight[imageIndex] = mInFlightFences[mCurrentFrame];
    
    if (onDrawStart) {
        onDrawStart(imageIndex);
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {mImageAvailableSemaphores[mCurrentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->commandBuffers()[imageIndex];

    VkSemaphore signalSemaphores[] = {mRenderFinishedSemaphores[mCurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(logicalDevice->handle(), 1, &mInFlightFences[mCurrentFrame]);

    if (vkQueueSubmit(logicalDevice->graphicsQueue()->handle(), 1,
                      &submitInfo, mInFlightFences[mCurrentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain->handle()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(logicalDevice->presentQueue()->handle(), &presentInfo);

    mCurrentFrame = (mCurrentFrame + 1) % mMaxFramesInFlight;
}

VkCommandBuffer RDrawHelper::beginSingleTimeCommands() {
    auto commandPool = mCommandPool.lock();
    ASSERT_NOT_NULL(commandPool);

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool->handle();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice->handle(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void RDrawHelper::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    auto commandPool = mCommandPool.lock();
    ASSERT_NOT_NULL(commandPool);

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(logicalDevice->graphicsQueue()->handle(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(logicalDevice->graphicsQueue()->handle());

    vkFreeCommandBuffers(logicalDevice->handle(), commandPool->handle(), 1, &commandBuffer);
}

void RDrawHelper::copyBuffer(const std::shared_ptr<RBufferObject> &src,
                std::shared_ptr<RBufferObject> dst,
                uint32_t size) {

    auto commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, src->handle(), dst->handle(), 1, &copyRegion);
    endSingleTimeCommands(commandBuffer);
}

void RDrawHelper::transitionImageLayout(const std::shared_ptr<RImageBuffer> &image,
                           VkImageLayout oldLayout, VkImageLayout newLayout) {

    auto commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image->handle();
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = 0;

    vkCmdPipelineBarrier(commandBuffer,
                         0, 0,
                         0,
                         0, nullptr,
                         0, nullptr,
                         1, &barrier);

    endSingleTimeCommands(commandBuffer);
}

}
