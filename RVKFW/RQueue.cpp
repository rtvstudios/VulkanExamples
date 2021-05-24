
#include "RQueue.h"
#include "RLogicalDevice.h"
#include "RLogger.h"

namespace rvkfw {

void RQueue::create(std::shared_ptr<RLogicalDevice> logicalDevice, uint32_t queueIndex) {
    if (mCreated.exchange(true)) {
        return;
    }

    mLogicalDevice = logicalDevice;

    vkGetDeviceQueue(logicalDevice->handle(), queueIndex, 0, &mQueue);
    if (mQueue == VK_NULL_HANDLE) {
        LOG_ERROR(tag(), "Could not get queue handle");
        throw std::runtime_error("Could not get queue handle");
    }

    LOG_DEBUG(tag(), "Queue Created : " << queueIndex);
}

RQueue::~RQueue() {
}

}
