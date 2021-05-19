
#include "VulPhysicalDevice.h"
#include "VulInstance.h"
#include "Logger.h"

namespace rtvvulfw {

VulPhysicalDevice::VulPhysicalDevice(VulInstance *instance) : mInstance{ instance } {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance->handle(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPU with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(mInstance->handle(), &deviceCount, devices.data());

    auto isDeviceSuitable = [this](VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        LOG_INFO(tag(), "DeviceProperties deviceType: " << deviceProperties.deviceType <<
                                    ", geometryShader: " << deviceFeatures.geometryShader <<
                                    ", tessellationShader: " << deviceFeatures.tessellationShader);

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
    };

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            mPhysicalDevice = device;
            break;
        }
    }

    if (mPhysicalDevice == VK_NULL_HANDLE && !devices.empty()) {
        LOG_INFO(tag(), "Could not find any suitable device taking the first available device");
        mPhysicalDevice = devices.at(0);
    }

    if (mPhysicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU");
    }

}

VulPhysicalDevice::~VulPhysicalDevice() {

}

}
