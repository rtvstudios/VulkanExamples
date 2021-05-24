
#include "RPhysicalDevice.h"
#include "RInstance.h"
#include "RLogger.h"
#include "RLogicalDevice.h"
#include "RSurface.h"
#include "RWindow.h"
#include "RSwapChain.h"

#include <optional>
#include <sstream>

namespace rvkfw {

const std::vector<const char*> RPhysicalDevice::deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

RPhysicalDevice::RPhysicalDevice(RInstance *instance, RSurface *surface, RWindow *window) :
    mInstance{ instance }, mSurface{ surface }, mWindow{ window} {

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
    std::optional<uint32_t> presentFamily;

    auto isDeviceWithGraphicsCapabilities = [&graphicsFamily, &presentFamily, this](VkPhysicalDevice device) {

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsFamily = i;
            }
            VkBool32 presentSupport = 0;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mInstance->surface()->handle(), &presentSupport);
            if (presentSupport) {
                presentFamily = i;
            }

            ++i;
        }

        auto swapChainSupport = RSwapChain::querySwapChainSupport(device, mSurface->handle());

        return  graphicsFamily.has_value() && presentFamily.has_value() &&
                checkDeviceExtensionSupport(device) && !swapChainSupport.formats.empty() &&
                !swapChainSupport.presentModes.empty();
    };

    if (mPhysicalDevice == VK_NULL_HANDLE) {
        LOG_INFO(tag(), "Could not find the best device taking the one with graphics capability");
        for (const auto& device : devices) {
            if (isDeviceWithGraphicsCapabilities(device)) {
                mPhysicalDevice = device;
                break;
            }
        }
    }

    if (mPhysicalDevice == VK_NULL_HANDLE) {
        LOG_ERROR(tag(), "Failed to find a suitable GPU");
        throw std::runtime_error("Failed to find a suitable GPU");
    }

    LOG_DEBUG(tag(), "Physical Device Created");
    
    LOG_DEBUG(tag(), "Extensions : " << getAllExtensions(mPhysicalDevice));

    mGraphicDevice = std::make_shared<RLogicalDevice>(this, graphicsFamily.value(), presentFamily.value());
    mSwapChain = std::make_shared<RSwapChain>(this, mGraphicDevice.get(), mSurface, mWindow);
    mSwapChain->create(graphicsFamily.value(), presentFamily.value());

}

std::string RPhysicalDevice::getAllExtensions(VkPhysicalDevice device) const {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::stringstream str;
    for (const auto &extension: availableExtensions) {
        str << extension.extensionName << ", ";
    }
    return str.str();
}

bool RPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)  const {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);

    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

RPhysicalDevice::~RPhysicalDevice() {
    mSwapChain = nullptr;
    mGraphicDevice = nullptr;
}

}
