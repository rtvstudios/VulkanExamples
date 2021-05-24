
#ifndef VulPhysicalDevice_h
#define VulPhysicalDevice_h

#include <string>
#include <memory>
#include <vector>

#include "RNonCopyable.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <set>

namespace rvkfw {

class RInstance;
class RLogicalDevice;
class RSurface;
class RWindow;
class RSwapChain;

class RPhysicalDevice: private RNonCopyable {
public:
    static const std::vector<const char*> deviceExtensions;

    RPhysicalDevice(RInstance *instance, RSurface *surface, RWindow *window);
    ~RPhysicalDevice();

    bool isCreated() const {
        return mPhysicalDevice != VK_NULL_HANDLE;
    }

    VkPhysicalDevice handle() {
        return mPhysicalDevice;
    }

    const char * tag() const {
        return "RPhysicalDevice";
    }

protected:

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
    
    std::string getAllExtensions(VkPhysicalDevice device) const;

    RInstance *mInstance{ nullptr };
    VkResult mResult{ VK_NOT_READY };
    RSurface *mSurface{ nullptr };
    RWindow *mWindow{ nullptr };
    VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
    std::shared_ptr<RLogicalDevice> mGraphicDevice;
    std::shared_ptr<RSwapChain> mSwapChain;
};

}

#endif

