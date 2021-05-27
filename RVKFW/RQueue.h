
#ifndef RQueue_h
#define RQueue_h

#include <string>
#include <memory>
#include <vector>

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;

class RQueue: public RObject {
public:
    RQueue(std::weak_ptr<RLogicalDevice> logicalDevice) : mLogicalDevice{logicalDevice} {
    }

    ~RQueue();

    void create(uint32_t queueIndex);
    void destroy() override;

    const char * tag() const override {
        return "RQueue";
    }

    uint32_t index() const {
        return mIndex;
    }

    VkQueue handle() const {
        return mQueue;
    }

protected:
    VkQueue mQueue{ VK_NULL_HANDLE };
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    uint32_t mIndex{ 0 };
};

}

#endif

