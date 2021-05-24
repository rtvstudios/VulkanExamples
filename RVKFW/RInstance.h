
#ifndef VulInstance_h
#define VulInstance_h

#include <string>
#include <memory>
#include <vector>

#include "RNonCopyable.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class RPhysicalDevice;
class RSurface;
class RWindow;

class RInstance: private RNonCopyable {
public:
    RInstance(const std::string &appName, RWindow *window);
    ~RInstance();

    bool isCreated() const {
        return mResult == VK_SUCCESS;
    }

    bool isValidationLayerAvailable() const;

    VkInstance handle() {
        return mInstance;
    }

    const char * tag() const {
        return "RInstance";
    }

    RSurface * surface() {
        return mSurface.get();
    }

protected:

    VkResult mResult{ VK_NOT_READY };
    VkInstance mInstance{ VK_NULL_HANDLE };
    VkApplicationInfo mAppInfo{};
    VkInstanceCreateInfo mCreateInfo{};

    std::shared_ptr<RPhysicalDevice> mPhysicalDevice;
    std::shared_ptr<RSurface> mSurface;

    std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };


};

}

#endif

