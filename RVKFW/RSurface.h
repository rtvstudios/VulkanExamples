
#ifndef RSurface_h
#define RSurface_h

#include "RObject.h"

namespace rvkfw {

class RVkInstance;
class RWindow;

class RSurface: public RObject {
public:
    RSurface() = default;
    ~RSurface();

    void create(std::shared_ptr<RVkInstance> instance, std::shared_ptr<RWindow> window);

    VkSurfaceKHR handle() {
        return mSurface;
    }

    const char * tag() const override {
        return "RSurface";
    }

protected:
    std::weak_ptr<RVkInstance> mInstance;
    VkSurfaceKHR mSurface{ VK_NULL_HANDLE };
};

}

#endif

