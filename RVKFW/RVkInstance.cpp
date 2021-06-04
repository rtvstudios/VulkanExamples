
#include "RVkInstance.h"
#include "RPhysicalDevice.h"
#include "RLogger.h"
#include "RSurface.h"

namespace rvkfw {

RVkInstance::RVkInstance(std::weak_ptr<RWindow> window) : mWindow{window} {
    mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    mAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    mAppInfo.pEngineName = "No Engine";
    mAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    mAppInfo.apiVersion = VK_API_VERSION_1_0;

    mCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    mCreateInfo.pApplicationInfo = &mAppInfo;
}

void RVkInstance::preCreate() {
    mSurface = std::make_shared<RSurface>(shared_from_this(), mWindow);
    mSurface->preCreate();

    mPhysicalDevice = std::make_shared<RPhysicalDevice>(shared_from_this(), mSurface, mWindow);
    mPhysicalDevice->preCreate();
}

void RVkInstance::create(const std::string &appName) {
    auto window = mWindow.lock();

    if (mCreated.exchange(true)) {
        return;
    }

    mAppInfo.pApplicationName = appName.c_str();
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
        mSurface->create();
        mPhysicalDevice->create();
    } else {
        LOG_ERROR(tag(), "Could not create Vulkan Instance");
    }
}

void RVkInstance::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }
    mPhysicalDevice->destroy();
    mSurface->destroy();

    if (mInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(mInstance, nullptr);
    }
    mInstance = VK_NULL_HANDLE;
}

RVkInstance::~RVkInstance() {
    destroy();
}

bool RVkInstance::isValidationLayerAvailable() const {
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
