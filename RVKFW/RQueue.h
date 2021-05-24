
#ifndef RQueue_h
#define RQueue_h

#include <string>
#include <memory>
#include <vector>

#include "RNonCopyable.h"
#include <GLFW/glfw3.h>

namespace rvkfw {

class RLogicalDevice;

class RQueue: private RNonCopyable {
public:
    RQueue() = default;
    ~RQueue();

    void create(std::shared_ptr<RLogicalDevice> logicalDevice, uint32_t queueIndex);

    const char * tag() const {
        return "RQueue";
    }

protected:
    VkQueue mQueue{ VK_NULL_HANDLE };
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
};

}

#endif

