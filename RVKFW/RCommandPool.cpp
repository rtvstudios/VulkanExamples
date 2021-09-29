
#include "RCommandPool.h"
#include "RLogicalDevice.h"
#include "RQueue.h"

namespace rvkfw {

RCommandPool::RCommandPool(std::shared_ptr<RLogicalDevice> logicalDevice) :
    mLogicalDevice{logicalDevice} {

    mCreatePoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    mCreatePoolInfo.queueFamilyIndex = logicalDevice->graphicsQueue()->index();
    mCreatePoolInfo.flags = 0;
}

RCommandPool::~RCommandPool() {
    destroy();
}

void RCommandPool::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }

    if (auto logicalDevice = mLogicalDevice.lock()) {
        if (mCommandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(logicalDevice->handle(), mCommandPool, nullptr);
        }
        mCommandPool = VK_NULL_HANDLE;
    } else {
        LOG_ERROR(tag(), "Could not get logical device, already destroyed!");
    }
}

void RCommandPool::create() {
    if (mCreated.exchange(true)) {
        return;
    }
 
    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    if (!mCreatePoolInfo.queueFamilyIndex) {
        mCreatePoolInfo.queueFamilyIndex = logicalDevice->graphicsQueue()->index();
    }

    if (vkCreateCommandPool(logicalDevice->handle(), &mCreatePoolInfo, nullptr, &mCommandPool) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create command pool!");
        throw std::runtime_error("failed to create command pool!");
    }
}

}
