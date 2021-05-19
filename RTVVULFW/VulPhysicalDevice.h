
#ifndef VulPhysicalDevice_h
#define VulPhysicalDevice_h

#include <string>
#include <memory>
#include <vector>

#include "NonCopyable.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class VulInstance;
class VulLogicalDevice;

class VulPhysicalDevice: private NonCopyable {
public:
    VulPhysicalDevice(VulInstance *instance);
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
    VulInstance *mInstance{ nullptr };
    VkResult mResult{ VK_NOT_READY };
    VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
    std::shared_ptr<VulLogicalDevice> mGraphicDevice;
};

}

#endif

