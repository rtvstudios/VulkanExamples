
#ifndef VulSurface_h
#define VulSurface_h

#include "NonCopyable.h"

#include <string>
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class VulInstance;
class Window;

class VulSurface: private NonCopyable {
public:
    VulSurface(VulInstance *vulInstance, Window *window);
    ~VulSurface();

    VkSurfaceKHR handle() {
        return mSurface;
    }

    const char * tag() const {
        return "VulSurface";
    }

protected:
    VulInstance *mVulInstance{ nullptr };
    VkSurfaceKHR mSurface{ VK_NULL_HANDLE };
};

}

#endif

