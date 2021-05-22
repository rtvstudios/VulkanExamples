
#ifndef VulLogicalDevice_h
#define VulLogicalDevice_h

#include <string>
#include <memory>
#include <vector>

#include "NonCopyable.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class VulPhysicalDevice;
class VulQueue;

class VulLogicalDevice: private NonCopyable {
public:
    VulLogicalDevice(VulPhysicalDevice *physicalDevice, uint32_t graphicsQueue, uint32_t presentQueue);
    ~VulLogicalDevice();

    const char * tag() const {
        return "VulLogicalDevice";
    }

    VkDevice handle() {
        return mDevice;
    }

    bool isCreated() const {
        return mDevice != VK_NULL_HANDLE;
    }

    VulQueue * graphicsQueue() {
        return mGraphicsQueue.get();
    }

    VulQueue * presentQueue() {
        return mPresentQueue.get();
    }

protected:
    VkDevice mDevice{ VK_NULL_HANDLE };
    VulPhysicalDevice *mPhysicalDevice{ nullptr };
    std::shared_ptr<VulQueue> mGraphicsQueue;
    std::shared_ptr<VulQueue> mPresentQueue;
};

}

#endif

