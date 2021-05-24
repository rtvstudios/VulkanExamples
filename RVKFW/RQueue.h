
#ifndef VulQueue_h
#define VulQueue_h

#include <string>
#include <memory>
#include <vector>

#include "RNonCopyable.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class RLogicalDevice;

class RQueue: private RNonCopyable {
public:
    RQueue(RLogicalDevice *logicalDevice, uint32_t queueIndex);
    ~RQueue();

    const char * tag() const {
        return "RQueue";
    }

protected:
    VkQueue mQueue{ VK_NULL_HANDLE };
    RLogicalDevice *mLogicalDevice{ nullptr };
};

}

#endif

