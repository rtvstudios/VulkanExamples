
#include "VulInstance.h"

namespace rtvvulfw {

VulInstance::VulInstance(const std::string &appName) {
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

    mResult = vkCreateInstance(&mCreateInfo, nullptr, &mInstance);
}

VulInstance::~VulInstance() {
    if (isCreated()) {
        vkDestroyInstance(mInstance, nullptr);
    }
}

}
