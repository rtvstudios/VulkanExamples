
#ifndef RLogicalDevice_h
#define RLogicalDevice_h

#include <string>
#include <memory>
#include <vector>

#include "RNonCopyable.h"
#include <GLFW/glfw3.h>

namespace rvkfw {

class RPhysicalDevice;
class RQueue;

class RLogicalDevice: public std::enable_shared_from_this<RLogicalDevice>, private RNonCopyable {
public:
    RLogicalDevice() = default;
    ~RLogicalDevice();

    void create(std::shared_ptr<RPhysicalDevice> physicalDevice, uint32_t graphicsQueue, uint32_t presentQueue);

    const char * tag() const {
        return "RLogicalDevice";
    }

    VkDevice handle() {
        return mDevice;
    }

    bool isCreated() const {
        return mDevice != VK_NULL_HANDLE;
    }

    std::shared_ptr<RQueue> graphicsQueue() {
        return mGraphicsQueue;
    }

    std::shared_ptr<RQueue> presentQueue() {
        return mPresentQueue;
    }

protected:
    VkDevice mDevice{ VK_NULL_HANDLE };

    std::weak_ptr<RPhysicalDevice> mPhysicalDevice;

    std::shared_ptr<RQueue> mGraphicsQueue;
    std::shared_ptr<RQueue> mPresentQueue;
};

}

#endif

