
#ifndef RSurface_h
#define RSurface_h

#include "RNonCopyable.h"

#include <string>
#include <GLFW/glfw3.h>

namespace rvkfw {

class RInstance;
class RWindow;

class RSurface: private RNonCopyable {
public:
    RSurface() = default;
    ~RSurface();

    void create(std::shared_ptr<RInstance> instance, std::shared_ptr<RWindow> window);

    VkSurfaceKHR handle() {
        return mSurface;
    }

    const char * tag() const {
        return "RSurface";
    }

protected:
    std::weak_ptr<RInstance> mInstance;
    VkSurfaceKHR mSurface{ VK_NULL_HANDLE };
};

}

#endif

