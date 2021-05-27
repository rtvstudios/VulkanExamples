
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"
#include "RQueue.h"
#include "RLogger.h"
#include "RCommandPool.h"
#include "RFramebuffer.h"
#include "RVkInstance.h"

#include <set>

namespace rvkfw {

void RLogicalDevice::preCreate() {
    mGraphicsQueue = std::make_shared<RQueue>(shared_from_this());
    mPresentQueue = std::make_shared<RQueue>(shared_from_this());
    mCommandPool = std::make_shared<RCommandPool>(shared_from_this());
}

void RLogicalDevice::create(uint32_t graphicsQueue,
                            uint32_t presentQueue) {
    if (mCreated.exchange(true)) {
        return;
    }

    auto physicalDevice = mPhysicalDevice.lock();
    ASSERT_NOT_NULL(physicalDevice);

    float queuePriority = 1.0f;
    std::set<uint32_t> uniqueQueueFamilies{graphicsQueue, presentQueue};

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = queueCreateInfos.size();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = RPhysicalDevice::deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = RPhysicalDevice::deviceExtensions.data();

#ifdef WITH_VALIDATION_LAYER
    LOG_DEBUG(tag(), "Validation layer option enabled");
    auto instance = physicalDevice->instance();
    ASSERT_NOT_NULL(instance);
    
    if (instance->isValidationLayerAvailable()) {
        LOG_INFO(tag(), "Validation layer available");
        createInfo.enabledLayerCount = instance->validationLayers().size();
        createInfo.ppEnabledLayerNames = instance->validationLayers().data();
    } else {
        LOG_WARN(tag(), "Validation layer not available");
    }
#endif

    if (vkCreateDevice(physicalDevice->handle(), &createInfo, nullptr, &mDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    LOG_DEBUG(tag(), "Logical Device Created graphicsQueue:" << graphicsQueue << " presentQueue: " << presentQueue);

    mGraphicsQueue->create(graphicsQueue);
    mPresentQueue->create(presentQueue);

    mCommandPool->create();
}

void RLogicalDevice::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }

    mGraphicsQueue->destroy();
    mPresentQueue->destroy();
    mCommandPool->destroy();
    if (mDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(mDevice, nullptr);
        mDevice = VK_NULL_HANDLE;
    }
}

RLogicalDevice::~RLogicalDevice() {
    destroy();
}

}
