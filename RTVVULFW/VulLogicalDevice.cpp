
#include "VulLogicalDevice.h"
#include "VulPhysicalDevice.h"
#include "VulQueue.h"

namespace rtvvulfw {

VulLogicalDevice::VulLogicalDevice(VulPhysicalDevice *physicalDevice, uint32_t queueIndex) : mPhysicalDevice{physicalDevice} {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndex;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    if (vkCreateDevice(physicalDevice->handle(), &createInfo, nullptr, &mDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    mQueue = std::make_shared<VulQueue>(this, queueIndex);
}

VulLogicalDevice::~VulLogicalDevice() {
    if (mDevice != VK_NULL_HANDLE) {
        mQueue = nullptr;
        vkDestroyDevice(mDevice, nullptr);
    }
}

}
