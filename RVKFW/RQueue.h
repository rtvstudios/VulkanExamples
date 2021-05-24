
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
    RQueue() = default;
    ~RQueue();

    void create(std::shared_ptr<RLogicalDevice> logicalDevice, uint32_t queueIndex);

    const char * tag() const override {
        return "RQueue";
    }

protected:
    VkQueue mQueue{ VK_NULL_HANDLE };
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
};

}

#endif

