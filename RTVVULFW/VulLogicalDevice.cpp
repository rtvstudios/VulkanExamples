
#include "VulLogicalDevice.h"
#include "VulPhysicalDevice.h"
#include "VulQueue.h"
#include "Logger.h"

#include <set>

namespace rtvvulfw {

VulLogicalDevice::VulLogicalDevice(VulPhysicalDevice *physicalDevice, uint32_t graphicsQueue, uint32_t presentQueue) :
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

    createInfo.enabledExtensionCount = VulPhysicalDevice::deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = VulPhysicalDevice::deviceExtensions.data();

    if (vkCreateDevice(physicalDevice->handle(), &createInfo, nullptr, &mDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    LOG_DEBUG(tag(), "Logical Device Created graphicsQueue:" << graphicsQueue << " presentQueue: " << presentQueue);

    mGraphicsQueue = std::make_shared<VulQueue>(this, graphicsQueue);
    mPresentQueue = std::make_shared<VulQueue>(this, presentQueue);
}

VulLogicalDevice::~VulLogicalDevice() {
    mGraphicsQueue = nullptr;
    mPresentQueue = nullptr;
    if (mDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(mDevice, nullptr);
    }
}

}
