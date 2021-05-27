
#ifndef RSurface_h
#define RSurface_h

#include "RObject.h"

namespace rvkfw {

class RVkInstance;
class RWindow;

class RSurface: public RObject {
public:
    RSurface(std::weak_ptr<RVkInstance> instance, std::weak_ptr<RWindow> window) :
            mInstance{instance}, mWindow{window} {
    }

    ~RSurface();

    void create();
    void destroy() override;

    VkSurfaceKHR handle() {
        return mSurface;
    }

    const char * tag() const override {
        return "RSurface";
    }

protected:
    std::weak_ptr<RVkInstance> mInstance;
    std::weak_ptr<RWindow> mWindow;

    VkSurfaceKHR mSurface{ VK_NULL_HANDLE };
};

}

#endif

