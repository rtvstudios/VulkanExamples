
#ifndef VulInstance_h
#define VulInstance_h

#include <string>
#include <memory>
#include <vector>

#include "NonCopyable.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class VulPhysicalDevice;
class VulSurface;
class Window;

class VulInstance: private NonCopyable {
public:
    VulInstance(const std::string &appName, Window *window);
    ~VulInstance();

    bool isCreated() const {
        return mResult == VK_SUCCESS;
    }

    bool isValidationLayerAvailable() const;

    VkInstance handle() {
        return mInstance;
    }

    const char * tag() const {
        return "VulInstance";
    }

    VulSurface * surface() {
        return mSurface.get();
    }

protected:

    VkResult mResult{ VK_NOT_READY };
    VkInstance mInstance{ VK_NULL_HANDLE };
    VkApplicationInfo mAppInfo{};
    VkInstanceCreateInfo mCreateInfo{};

    std::shared_ptr<VulPhysicalDevice> mPhysicalDevice;
    std::shared_ptr<VulSurface> mSurface;

    std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };


};

}

#endif

