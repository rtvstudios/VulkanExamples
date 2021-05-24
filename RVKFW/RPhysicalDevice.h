
#ifndef RPhysicalDevice_h
#define RPhysicalDevice_h

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

class RPhysicalDevice: public std::enable_shared_from_this<RPhysicalDevice>, private RNonCopyable {
public:
    static const std::vector<const char*> deviceExtensions;

    RPhysicalDevice() = default;
    ~RPhysicalDevice();

    void create(std::shared_ptr<RInstance> instance, std::shared_ptr<RSurface> surface, std::shared_ptr<RWindow> window);

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

    VkResult mResult{ VK_NOT_READY };
    VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };

    std::weak_ptr<RInstance> mInstance;
    std::weak_ptr<RSurface> mSurface;
    std::weak_ptr<RWindow> mWindow;

    std::shared_ptr<RLogicalDevice> mLogicalDevice;
    std::shared_ptr<RSwapChain> mSwapChain;
};

}

#endif

