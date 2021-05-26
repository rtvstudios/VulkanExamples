
#include "RCommandPool.h"
#include "RLogicalDevice.h"
#include "RQueue.h"

namespace rvkfw {

RCommandPool::~RCommandPool() {
    if (auto logicalDevice = mLogicalDevice.lock()) {
        if (mCommandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(logicalDevice->handle(), mCommandPool, nullptr);
        }
    } else {
        LOG_ERROR(tag(), "Could not get logical device, already destroyed!");
    }
}

void RCommandPool::create() {
    if (mCreated.exchange(true)) {
        return;
    }
 
    auto logicalDevice = mLogicalDevice.lock();
    if (!logicalDevice) {
        LOG_ERROR(tag(), "Creation failed logicalDevice:" << logicalDevice.get());
        throw std::runtime_error("Framebuffer creation failed, required objects are not available!");
    }

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = logicalDevice->graphicsQueue()->index();
    poolInfo.flags = 0;

    if (vkCreateCommandPool(logicalDevice->handle(), &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create command pool!");
        throw std::runtime_error("failed to create command pool!");
    }
}

}
