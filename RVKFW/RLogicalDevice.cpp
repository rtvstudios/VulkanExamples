
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"
#include "RQueue.h"
#include "RLogger.h"

#include <set>

namespace rtvvulfw {

RLogicalDevice::RLogicalDevice(RPhysicalDevice *physicalDevice, uint32_t graphicsQueue, uint32_t presentQueue) :
        mPhysicalDevice{physicalDevice} {

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

    if (vkCreateDevice(physicalDevice->handle(), &createInfo, nullptr, &mDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    LOG_DEBUG(tag(), "Logical Device Created graphicsQueue:" << graphicsQueue << " presentQueue: " << presentQueue);

    mGraphicsQueue = std::make_shared<RQueue>(this, graphicsQueue);
    mPresentQueue = std::make_shared<RQueue>(this, presentQueue);
}

RLogicalDevice::~RLogicalDevice() {
    mGraphicsQueue = nullptr;
    mPresentQueue = nullptr;
    if (mDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(mDevice, nullptr);
    }
}

}
