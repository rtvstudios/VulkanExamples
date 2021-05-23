
#ifndef VulQueue_h
#define VulQueue_h

#include <string>
#include <memory>
#include <vector>

#include "NonCopyable.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class VulLogicalDevice;

class VulQueue: private NonCopyable {
public:
    VulQueue(VulLogicalDevice *logicalDevice, uint32_t queueIndex);
    ~VulQueue();

    const char * tag() const {
        return "VulQueue";
    }

protected:
    VkQueue mQueue{ VK_NULL_HANDLE };
    VulLogicalDevice *mLogicalDevice{ nullptr };
};

}

#endif

