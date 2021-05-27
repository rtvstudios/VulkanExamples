
#include "RPhysicalDevice.h"
#include "RVkInstance.h"
#include "RLogger.h"
#include "RLogicalDevice.h"
#include "RSurface.h"
#include "RWindow.h"
#include "RSwapChain.h"

#include <optional>
#include <sstream>

namespace rvkfw {

const std::vector<const char*> RPhysicalDevice::deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    //"VK_KHR_portability_subset"
};

void RPhysicalDevice::preCreate() {
    mLogicalDevice = std::make_shared<RLogicalDevice>(shared_from_this());
    mLogicalDevice->preCreate();

    mSwapChain = std::make_shared<RSwapChain>(shared_from_this(), mLogicalDevice,
                                              mSurface, mWindow);
    mSwapChain->preCreate();
}

void RPhysicalDevice::create() {
    if (mCreated.exchange(true)) {
        return;
    }

    auto instance = mInstance.lock();
    ASSERT_NOT_NULL(instance);

    auto surface = mSurface.lock();
    ASSERT_NOT_NULL(surface);

    auto window = mWindow.lock();
    ASSERT_NOT_NULL(window);

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance->handle(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        LOG_ERROR(tag(), "Failed to find GPU with Vulkan support");
        throw std::runtime_error("Failed to find GPU with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance->handle(), &deviceCount, devices.data());

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

    auto isDeviceWithGraphicsCapabilities = [&graphicsFamily, &presentFamily, this, surface](VkPhysicalDevice device) {

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
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface->handle(), &presentSupport);
            if (presentSupport) {
                presentFamily = i;
            }

            ++i;
        }

        auto swapChainSupport = RSwapChain::querySwapChainSupport(device, surface->handle());

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

    mLogicalDevice->create(graphicsFamily.value(), presentFamily.value());

    mSwapChain->create(graphicsFamily.value(),
                       presentFamily.value());
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

void RPhysicalDevice::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }
    mSwapChain->destroy();
    mLogicalDevice->destroy();
}

RPhysicalDevice::~RPhysicalDevice() {
    destroy();
}

}
