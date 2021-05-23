
#ifndef VulPhysicalDevice_h
#define VulPhysicalDevice_h

#include <string>
#include <memory>
#include <vector>

#include "NonCopyable.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <set>

namespace rtvvulfw {

class VulInstance;
class VulLogicalDevice;
class VulSurface;
class Window;
class VulSwapChain;

class VulPhysicalDevice: private NonCopyable {
public:
    static const std::vector<const char*> deviceExtensions;

    VulPhysicalDevice(VulInstance *instance, VulSurface *surface, Window *window);
    ~VulPhysicalDevice();

    bool isCreated() const {
        return mPhysicalDevice != VK_NULL_HANDLE;
    }

    VkPhysicalDevice handle() {
        return mPhysicalDevice;
    }

    const char * tag() const {
        return "VulPhysicalDevice";
    }

protected:

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
    
    std::string getAllExtensions(VkPhysicalDevice device) const;

    VulInstance *mInstance{ nullptr };
    VkResult mResult{ VK_NOT_READY };
    VulSurface *mSurface{ nullptr };
    Window *mWindow{ nullptr };
    VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
    std::shared_ptr<VulLogicalDevice> mGraphicDevice;
    std::shared_ptr<VulSwapChain> mSwapChain;
};

}

#endif

