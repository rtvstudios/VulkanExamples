
#include "VulPhysicalDevice.h"
#include "VulInstance.h"
#include "Logger.h"
#include "VulLogicalDevice.h"

#include <optional>

namespace rtvvulfw {

VulPhysicalDevice::VulPhysicalDevice(VulInstance *instance) : mInstance{ instance } {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance->handle(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        LOG_ERROR(tag(), "Failed to find GPU with Vulkan support");
        throw std::runtime_error("Failed to find GPU with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(mInstance->handle(), &deviceCount, devices.data());

    auto isDeviceWithGeometricShader = [this](VkPhysicalDevice device) {
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
        if (isDeviceWithGeometricShader(device)) {
            mPhysicalDevice = device;
            break;
        }
    }

    std::optional<uint32_t> graphicsFamily;

    auto isDeviceWithGraphicsCapability = [&graphicsFamily](VkPhysicalDevice device) {

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsFamily = i;
            }
            ++i;
        }
        return  graphicsFamily.has_value();
    };

    if (mPhysicalDevice == VK_NULL_HANDLE) {
        LOG_INFO(tag(), "Could not find the best device taking the one with graphics capability");
        for (const auto& device : devices) {
            if (isDeviceWithGraphicsCapability(device)) {
                mPhysicalDevice = device;
                break;
            }
        }
    }

    if (mPhysicalDevice == VK_NULL_HANDLE) {
        LOG_ERROR(tag(), "Failed to find a suitable GPU");
        throw std::runtime_error("Failed to find a suitable GPU");
    }

    mGraphicDevice = std::make_shared<VulLogicalDevice>(this, graphicsFamily.value());

}

VulPhysicalDevice::~VulPhysicalDevice() {
}

}
