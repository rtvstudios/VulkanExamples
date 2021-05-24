
#include "RInstance.h"
#include "RPhysicalDevice.h"
#include "RLogger.h"
#include "RSurface.h"
#include "RCreator.h"

namespace rvkfw {

void RInstance::create(const std::string &appName, std::shared_ptr<RWindow> window) {
    if (mCreated.exchange(true)) {
        return;
    }

    mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    mAppInfo.pApplicationName = appName.c_str();
    mAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    mAppInfo.pEngineName = "No Engine";
    mAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    mAppInfo.apiVersion = VK_API_VERSION_1_0;

    mCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    mCreateInfo.pApplicationInfo = &mAppInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    mCreateInfo.enabledExtensionCount = glfwExtensionCount;
    mCreateInfo.ppEnabledExtensionNames = glfwExtensions;
    mCreateInfo.enabledLayerCount = 0;

#ifdef WITH_VALIDATION_LAYER
    LOG_DEBUG(tag(), "Validation layer option enabled");
    if (isValidationLayerAvailable()) {
        LOG_INFO(tag(), "Validation layer available");
        mCreateInfo.enabledLayerCount = mValidationLayers.size();
        mCreateInfo.ppEnabledLayerNames = mValidationLayers.data();
    } else {
        LOG_WARN(tag(), "Validation layer not available");
    }
#endif

    mResult = vkCreateInstance(&mCreateInfo, nullptr, &mInstance);

    if (isCreated()) {
        mSurface = RCreator::create<RSurface>(shared_from_this(), window);
        mPhysicalDevice = RCreator::create<RPhysicalDevice>(shared_from_this(), mSurface, window);
    } else {
        LOG_ERROR(tag(), "Could not create Vulkan Instance");
    }
}


RInstance::~RInstance() {
    mSurface = nullptr;
    mPhysicalDevice = nullptr;
    if (isCreated()) {
        vkDestroyInstance(mInstance, nullptr);
    }
}

bool RInstance::isValidationLayerAvailable() const {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (auto layerName : mValidationLayers) {
        for (const auto& layerProperties : availableLayers) {
            if (std::string(layerName) == layerProperties.layerName) {
                return true;
            }
        }
    }

    return false;
}

}
