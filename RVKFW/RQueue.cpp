
#include "RQueue.h"
#include "RLogicalDevice.h"
#include "RLogger.h"

namespace rvkfw {

void RQueue::create(uint32_t queueIndex) {
    mIndex = queueIndex;

    if (mCreated.exchange(true)) {
        return;
    }

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

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
