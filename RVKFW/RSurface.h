
#ifndef VulSurface_h
#define VulSurface_h

#include "RNonCopyable.h"

#include <string>
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class RInstance;
class RWindow;

class RSurface: private RNonCopyable {
public:
    RSurface(RInstance *vulInstance, RWindow *window);
    ~RSurface();

    VkSurfaceKHR handle() {
        return mSurface;
    }

    const char * tag() const {
        return "RSurface";
    }

protected:
    RInstance *mVulInstance{ nullptr };
    VkSurfaceKHR mSurface{ VK_NULL_HANDLE };
};

}

#endif

