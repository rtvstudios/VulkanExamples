
#ifndef RLogicalDevice_h
#define RLogicalDevice_h

#include <string>
#include <memory>
#include <vector>

#include "RObject.h"

namespace rvkfw {

class RPhysicalDevice;
class RQueue;
class RCommandPool;

class RLogicalDevice: public std::enable_shared_from_this<RLogicalDevice>, public RObject {
public:
    RLogicalDevice(std::shared_ptr<RPhysicalDevice> physicalDevice) : mPhysicalDevice{physicalDevice} {
    }

    ~RLogicalDevice();

    void preCreate() override;
    void create(uint32_t graphicsQueue, uint32_t presentQueue);
    void destroy() override;
    
    const char * tag() const override {
        return "RLogicalDevice";
    }

    VkDevice handle() const {
        return mDevice;
    }

    bool isCreated() const {
        return mDevice != VK_NULL_HANDLE;
    }

    std::shared_ptr<RQueue> graphicsQueue() const {
        return mGraphicsQueue;
    }

    std::shared_ptr<RQueue> presentQueue() const {
        return mPresentQueue;
    }
    
    std::shared_ptr<RCommandPool> commandPool() const {
        return mCommandPool;
    }

protected:
    VkDevice mDevice{ VK_NULL_HANDLE };

    std::weak_ptr<RPhysicalDevice> mPhysicalDevice;

    std::shared_ptr<RQueue> mGraphicsQueue;
    std::shared_ptr<RQueue> mPresentQueue;
    std::shared_ptr<RCommandPool> mCommandPool;
};

}

#endif

