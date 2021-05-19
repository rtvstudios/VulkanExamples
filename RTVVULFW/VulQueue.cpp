
#include "VulQueue.h"
#include "VulLogicalDevice.h"
#include "Logger.h"

namespace rtvvulfw {

VulQueue::VulQueue(VulLogicalDevice *logicalDevice, uint32_t queueIndex) : mLogicalDevice{ logicalDevice } {
    vkGetDeviceQueue(logicalDevice->handle(), queueIndex, 0, &mQueue);
    if (mQueue == VK_NULL_HANDLE) {
        LOG_ERROR(tag(), "Could not get queue handle");
        throw std::runtime_error("Could not get queue handle");
    }
}

VulQueue::~VulQueue() {
}

}
