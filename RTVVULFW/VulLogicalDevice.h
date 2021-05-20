
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
    VulLogicalDevice(VulPhysicalDevice *physicalDevice, uint32_t queueIndex);
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

    VulQueue *queue() {
        return mQueue.get();
    }

protected:
    VkDevice mDevice{ VK_NULL_HANDLE };
    VulPhysicalDevice *mPhysicalDevice{ nullptr };
    std::shared_ptr<VulQueue> mQueue;
};

}

#endif

