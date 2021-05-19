
#ifndef VulInstance_h
#define VulInstance_h

#include <string>
#include <memory.h>

#include "NonCopyable.h"
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class VulInstance: private NonCopyable {
public:
    VulInstance(const std::string &appName);
    ~VulInstance();

    bool isCreated() const {
        return mResult == VK_SUCCESS;
    }

protected:
    VkResult mResult;
    VkInstance mInstance;
    VkApplicationInfo mAppInfo{};
    VkInstanceCreateInfo mCreateInfo{};
};

}

#endif

