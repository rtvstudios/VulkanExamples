
#ifndef VulLogicalDevice_h
#define VulLogicalDevice_h

#include <string>
#include <memory>
#include <vector>

#include "RNonCopyable.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class RPhysicalDevice;
class RQueue;

class RLogicalDevice: private RNonCopyable {
public:
    RLogicalDevice(RPhysicalDevice *physicalDevice, uint32_t graphicsQueue, uint32_t presentQueue);
    ~RLogicalDevice();

    const char * tag() const {
        return "RLogicalDevice";
    }

    VkDevice handle() {
        return mDevice;
    }

    bool isCreated() const {
        return mDevice != VK_NULL_HANDLE;
    }

    RQueue * graphicsQueue() {
        return mGraphicsQueue.get();
    }

    RQueue * presentQueue() {
        return mPresentQueue.get();
    }

protected:
    VkDevice mDevice{ VK_NULL_HANDLE };
    RPhysicalDevice *mPhysicalDevice{ nullptr };
    std::shared_ptr<RQueue> mGraphicsQueue;
    std::shared_ptr<RQueue> mPresentQueue;
};

}

#endif

